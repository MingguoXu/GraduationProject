#include "EdpKit.h"

#include <string.h>
#include <stdlib.h>

void assert(int ecp)
{ 
	 ;
}



/*---------------------------------------------------------------------------*/
Buffer* NewBuffer(void)
{
    Buffer* buf = (Buffer*)malloc(sizeof(Buffer));
    buf->_data = (char*)malloc(sizeof(u8) * BUFFER_SIZE);
    buf->_write_pos = 0;
    buf->_read_pos = 0;
    buf->_capacity = BUFFER_SIZE;
    return buf;
}
void DeleteBuffer(Buffer** buf)
{
    char* pdata = (*buf)->_data;
    free(pdata);
    free(*buf);
    *buf = 0;
}
int CheckCapacity(Buffer* buf, u32 len)
{
    u32 cap_len = buf->_capacity;
    int flag = 0;
    char* pdata = NULL;

    while (cap_len - buf->_write_pos < len) /* remain len < len */
    {
        cap_len = cap_len << 1;
        if (++flag > 32)
            break;  /* overflow */
    }
    if (flag > 32)
        return -1;
    if (cap_len > buf->_capacity)
    {
        pdata = (char*)malloc(sizeof(u8) * cap_len);
        memcpy(pdata, buf->_data, buf->_write_pos);
        free(buf->_data);
        buf->_data = pdata;
        buf->_capacity = cap_len;
    }
    return 0;
}
/*---------------------------------------------------------------------------*/
int ReadByte(EdpPacket* pkg, u8* val)
{
    if (pkg->_read_pos+1 > pkg->_write_pos) 
        return -1;
    *val = pkg->_data[pkg->_read_pos];
    pkg->_read_pos += 1;
    return 0;
}
int ReadBytes(EdpPacket* pkg, u8** val, u32 count)
{
    if (pkg->_read_pos+count > pkg->_write_pos) 
        return -1;
    *val = (u8*)malloc(sizeof(u8) * count);
    memcpy(*val, pkg->_data + pkg->_read_pos, count);
    pkg->_read_pos += count;
    return 0;
}
int ReadUint16(EdpPacket* pkg, u16* val)
{
    u8 msb, lsb;
    if (pkg->_read_pos+2 > pkg->_write_pos) 
        return -1;
    msb = pkg->_data[pkg->_read_pos];
    pkg->_read_pos++;
    lsb = pkg->_data[pkg->_read_pos];
    pkg->_read_pos++;
    *val = (msb<<8) + lsb;
    return 0;
}
int ReadUint32(EdpPacket* pkg, u32* val)
{
    int i = 0;
    u32 tmpval = 0;
    if (pkg->_read_pos+4 > pkg->_write_pos) 
        return -1;
    while (i++ < 4) {
        tmpval = (tmpval << 8) | (pkg->_data[pkg->_read_pos]);
        pkg->_read_pos++;
    }
    *val = tmpval;
    return 0;
}
int ReadStr(EdpPacket* pkg, char** val)
{
    u16 len = 0;
    int rc = 0;
    /* read str len */
    rc = ReadUint16(pkg, &len);
    if (rc) 
        return rc;
    if (pkg->_read_pos+len > pkg->_write_pos) 
        return -1;
    /* copy str val */
    *val = (char*)malloc(sizeof(char) * (len + 1));
    memset(*val, 0, len+1);
    strncpy(*val, (const char *)(pkg->_data + pkg->_read_pos), len);
    pkg->_read_pos += len;
    return 0;
}
int ReadRemainlen(EdpPacket* pkg, u32* len_val)
{
    u32 multiplier = 1;
    u32 len_len = 0;
    u8 onebyte = 0;
    int rc;
    *len_val = 0;
    do {
        rc = ReadByte(pkg, &onebyte);
        if (rc) 
            return rc;

        *len_val += (onebyte & 0x7f) * multiplier;
        multiplier *= 0x80;

        len_len++;
        if (len_len > 4) {
            return -1;/*len of len more than 4;*/
        }
    } while((onebyte & 0x80) != 0);
    return 0;
}
/*---------------------------------------------------------------------------*/
int WriteByte(Buffer* buf, u8 byte)
{
    assert(buf->_read_pos == 0);
    if (CheckCapacity(buf, 1))
        return -1;
    buf->_data[buf->_write_pos] = byte;
    buf->_write_pos++;
    return 0;
}
int WriteBytes(Buffer* buf, const void* bytes, u32 count)
{
    assert(buf->_read_pos == 0);
    if (CheckCapacity(buf, count))
        return -1;
    memcpy(buf->_data + buf->_write_pos, bytes, count);
    buf->_write_pos += count;
    return 0;
}
int WriteUint16(Buffer* buf, u16 val)
{
    assert(buf->_read_pos == 0);
    return WriteByte(buf, MOSQ_MSB(val)) 
        || WriteByte(buf, MOSQ_LSB(val));
}
int WriteUint32(Buffer* buf, u32 val)
{
    assert(buf->_read_pos == 0);
    return WriteByte(buf, (val >> 24) & 0x00FF) 
        || WriteByte(buf, (val >> 16) & 0x00FF)
        || WriteByte(buf, (val >> 8) & 0x00FF) 
        || WriteByte(buf, (val) & 0x00FF);
}
int WriteStr(Buffer* buf, const char *str)
{
    u16 length = 0;
    assert(buf->_read_pos == 0);
    length = strlen(str);
    return WriteUint16(buf, length) 
        || WriteBytes(buf, str, length);
}
int WriteRemainlen(Buffer* buf, u32 len_val)
{
    u32 remaining_length = len_val;
    int remaining_count = 0;
    u8 byte = 0;

    assert(buf->_read_pos == 0);

    do {
        byte = remaining_length % 128;
        remaining_length = remaining_length / 128;
        /* If there are more digits to encode, set the top bit of this digit */
        if (remaining_length > 0) {
            byte = byte | 0x80;
        }
        buf->_data[buf->_write_pos++] = byte;
        remaining_count++;
    }while(remaining_length > 0 && remaining_count < 5);
    assert(remaining_count != 5);
    return 0;
}
/*---------------------------------------------------------------------------*/
/* get edp packet type, client should use this type to invoke Unpack??? function */
u8 EdpPacketType(EdpPacket* pkg)
{
    u8 mtype = 0x00;
    ReadByte(pkg, &mtype);
    return mtype;
}
/* connect1 (C->S): devid + apikey */
EdpPacket* PacketConnect1(const char* devid, const char* auth_key)
{
    EdpPacket* pkg = NewBuffer();
    u32 remainlen;
	/* msg type */
    WriteByte(pkg, CONNREQ);
	/* remain len */
	remainlen = (2+3)+1+1+2+(2+strlen(devid))+(2+strlen(auth_key));
	WriteRemainlen(pkg, remainlen);
	/* protocol desc */
	WriteStr(pkg, PROTOCOL_NAME);
	/* protocol version */
	WriteByte(pkg, PROTOCOL_VERSION);
	/* connect flag */
	WriteByte(pkg, 0x40);
	/* keep time */
	WriteUint16(pkg, 0x0080);
	/* DEVID */
	WriteStr(pkg, devid);
	/* auth key */
	WriteStr(pkg, auth_key);
    return pkg;
}
/* connect2 (C->S): userid + auth_info */
EdpPacket* PacketConnect2(const char* userid, const char* auth_info)
{
    EdpPacket* pkg = NULL;
    u32 remainlen;

    pkg = NewBuffer();
	/* msg type */
    WriteByte(pkg, CONNREQ);
	/* remain len */
	remainlen = (2+3)+1+1+2+2+(2+strlen(userid))+(2+strlen(auth_info));
	WriteRemainlen(pkg, remainlen);
	/* protocol desc */
	WriteStr(pkg, PROTOCOL_NAME);
	/* protocol version */
	WriteByte(pkg, PROTOCOL_VERSION);
	/* connect flag */
	WriteByte(pkg, 0xC0);
	/* keep time */
	WriteUint16(pkg, 0x0080);
    /* devid */
    WriteByte(pkg, 0x00);
    WriteByte(pkg, 0x00);
	/* USERID */
	WriteStr(pkg, userid);
	/* auth info */
	WriteStr(pkg, auth_info);
    return pkg;
}
/* push_data (C->S) */
EdpPacket* PacketPushdata(const char* dst_devid, const char* data, u32 data_len)
{
    EdpPacket* pkg = NULL;
    u32 remainlen;

    pkg = NewBuffer();
    /* msg type */
    WriteByte(pkg, PUSHDATA);
    /* remain len */
    remainlen = (2+strlen(dst_devid))+data_len;
    WriteRemainlen(pkg, remainlen);
    /* dst devid */
    WriteStr(pkg, dst_devid);
    /* data */
    WriteBytes(pkg, data, data_len);
    return pkg;
}

/* push_data (S->C) */
int UnpackPushdata(EdpPacket* pkg, char** src_devid, char** data, u32* data_len)
{
    u32 remainlen = 0;
    if (ReadRemainlen(pkg, &remainlen))
        return ERR_UNPACK_PUSHD_REMAIN;
    if (ReadStr(pkg, src_devid))
        return ERR_UNPACK_PUSHD_DEVID;
    remainlen -= (2 + strlen(*src_devid));
    if (ReadBytes(pkg, (u8**)data, remainlen))
        return ERR_UNPACK_PUSHD_DATA;
    *data_len = remainlen;
    assert(pkg->_read_pos == pkg->_write_pos);
    return 0;
}

EdpPacket* PacketSaveJson(const char* dst_devid, char* json_obj, SaveDataType type)
{
    EdpPacket* pkg = NewBuffer();
    u32 remainlen;
    //char* json_out = cJSON_Print(json_obj);
    u32 json_len = strlen(json_obj);

    /* msg type */
    WriteByte(pkg, SAVEDATA);
    if(dst_devid)
    {
        /* remain len */
        remainlen = 1+(2+strlen(dst_devid))+1+(2+json_len);
        WriteRemainlen(pkg, remainlen);
        /* translate address flag */
        WriteByte(pkg, 0x80);
        /* dst devid */
        WriteStr(pkg, dst_devid);
    }
    else
    {
        /* remain len */
        remainlen = 1+1+(2+json_len);
        WriteRemainlen(pkg, remainlen);
        /* translate address flag */
        WriteByte(pkg, 0x00);
    }
    /* json flag */
    WriteByte(pkg, type);
    /* json */
    WriteStr(pkg, json_obj);
    //free(json_out);
    return pkg;
}

EdpPacket* PacketSaveBin(const char* dst_devid, char* desc_obj, unsigned int bin_len)
{
    EdpPacket* pkg = NewBuffer();
    unsigned int remainlen = 0;
    unsigned int desc_len = 0;

    /* check arguments */
    //desc_out = cJSON_Print(desc_obj);
    desc_len = strlen(desc_obj);

	/* msg type */
	WriteByte(pkg, SAVEDATA);
    if(dst_devid)
    {
        /* remain len */
        remainlen = 1+(2+strlen(dst_devid))+1+(2+desc_len)+(4+bin_len);
        WriteRemainlen(pkg, remainlen);
        /* translate address flag */
        WriteByte(pkg, 0x80);
        /* dst devid */
        WriteStr(pkg, dst_devid);
    }
    else
    {
        /* remain len */
        remainlen = 1+1+(2+desc_len)+(4+bin_len);
        WriteRemainlen(pkg, remainlen);
        /* translate address flag */
        WriteByte(pkg, 0x00);
    }
    /* bin flag */
    WriteByte(pkg, 0x02);
    /* desc */ 
    WriteStr(pkg, desc_obj);
    //free(desc_out);
    /* bin data */
    WriteUint32(pkg, bin_len);
    //WriteBytes(pkg, bin_data, bin_len); //内存不足，先发包头，再单独发送图片数据
    return pkg;
}

/* sava_data (C->S) */
EdpPacket* PacketSavedataSimpleString(const char* dst_devid, const char* input)
{
    EdpPacket* pkg = NULL;
    u32 remainlen = 0;
    u32 input_len = 0;

    pkg = NewBuffer();
    input_len = strlen(input);
    /* msg type */
    WriteByte(pkg, SAVEDATA);
    if (dst_devid)
    {
        /* remain len */
        remainlen = 1+(2+strlen(dst_devid))+1+(2+input_len);
        WriteRemainlen(pkg, remainlen);
        /* translate address flag */
        WriteByte(pkg, 0x80);
        /* dst devid */
        WriteStr(pkg, dst_devid);
    }
    else
    {
        /* remain len */
        remainlen = 1+1+(2+input_len);
        WriteRemainlen(pkg, remainlen);
        /* translate address flag */
        WriteByte(pkg, 0x00);
    }
    /* json flag */
    WriteByte(pkg, kTypeString);
    /* json */ 
    WriteStr(pkg, input);

    return pkg;
}

EdpPacket* PacketCmdResp(const char* cmdid, u16 cmdid_len,
			 const char* resp, u32 resp_len)
{
    EdpPacket* send_pkg = NULL;
    unsigned remainlen = 0;

    send_pkg = NewBuffer();
    /* 6 = 2 + 4 = len(cmdid_len) + len(resp_len) */
    remainlen = cmdid_len + resp_len + (resp_len ? 6 : 2);
    WriteByte(send_pkg, CMDRESP);
    WriteRemainlen(send_pkg, remainlen);
    WriteUint16(send_pkg, cmdid_len);
    WriteBytes(send_pkg, cmdid, cmdid_len);
    if (resp_len){
	WriteUint32(send_pkg, resp_len);
	WriteBytes(send_pkg, resp, resp_len);
    }
    return send_pkg;
}

int UnpackCmdReq(EdpPacket* pkg, char** cmdid, u16* cmdid_len, 
		   char** req, u32* req_len)
{
    u32 remainlen;
    int rc;
    if (ReadRemainlen(pkg, &remainlen))
	return ERR_UNPACK_CMDREQ;

    rc = ReadUint16(pkg, cmdid_len);
    if (rc) 
        return rc;
    if (ReadBytes(pkg, (u8**)cmdid, *cmdid_len))
        return ERR_UNPACK_CMDREQ;

    rc = ReadUint32(pkg, req_len);
    if (rc) 
        return rc;
    if (ReadBytes(pkg, (u8**)req, *req_len))
        return ERR_UNPACK_CMDREQ;

    assert(pkg->_read_pos == pkg->_write_pos);
    return 0;
}
