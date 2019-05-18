#include "time.h"
#include "sys.h"
#include "esp8266.h"
#include "delay.h"
#include "onenetusart.h"
#include "edpkit.h"
#include "led.h"

#include <string.h>
#include <stdio.h>


EdpPacket *send_pkg;	//Э���
#define DEVID	"29674779"
#define APIKEY	"72MZsOW6eTDc8WiTCvpEDzNvfoA="

#define ESP8266_WIFI_NAME       "AT+CWJAP=\"gcc\",\"gcc12345\"\r\n"

#define ESP8266_OneNET_INFO		"AT+CIPSTART=\"TCP\",\"183.230.40.39\",876\r\n"
                                //"AT+CIPSTART=\"TCP\",\"101.200.58.175\",8000\r\n"
                                //"AT+CIPSTART=\"TCP\",\"183.230.40.39\",876\r\n"
#define Send_Erro_Times_Max 5  //�����ʧ�ܴ���������������WIFI
#define WIFI_Rec_Keywords "+IPD"

char esp8266_buf[300];
unsigned short esp8266_cnt = 0;

u8 ESP8266_Clear(_Bool i);
_Bool ESP8266_SendCmd(char *cmd, char *res ,unsigned int timeOut);
void ESP8266_SendData(char *data, unsigned short len);
u8 ESP8266_Setting(void);
_Bool OneNet_EDP_Connect(void);
void ESP8266_Init(void);
u8 ESP8266_Connect_Again(void);
void OneNET_FillBuf(char *buf);
void OneNET_SendData(void);
void ESP8266_Wait_RecData(void);
/*
*****************************************************************************************

��ջ���

*****************************************************************************************
*/
u8 ESP8266_Clear(_Bool i)
{
    if(i==1)
    {
        memset(esp8266_buf, 0, sizeof(esp8266_buf));
        esp8266_cnt = 0;
        return 1; 
    }
    else if(i==0)
    {
        if(strstr((const char *)esp8266_buf, (const char *)WIFI_Rec_Keywords) != NULL)
        {
            return 0;
        }
        memset(esp8266_buf, 0, sizeof(esp8266_buf));
        esp8266_cnt = 0;
        return 1;    
    }
    return 2;    
}
/*
*****************************************************************************************

��������

*****************************************************************************************
*/
_Bool ESP8266_SendCmd(char *cmd, char *res ,unsigned int timeOut)
{	
	Usart_SendString(USART2,cmd, strlen(cmd));
	
	while(--timeOut)
	{
        if(strstr((const char *)esp8266_buf, (const char *)res) != NULL)		//����������ؼ���
        {				
            ESP8266_Clear(0);									//��ջ���
            
            return 0;
        }
		delay_ms(1);
	}
    ESP8266_Clear(0);
	return 1;
}

/*
*****************************************************************************************

Clientģʽ�·�������

*****************************************************************************************
*/
void ESP8266_SendData(char *data, unsigned short len)
{
//	static unsigned char n=0;//n����,��������
	char cmdBuf[32];
	unsigned int j=1000;
    memset(cmdBuf,0,sizeof(cmdBuf));    		    //��ս��ջ���
    
    sprintf(cmdBuf, "AT+CIPSEND=%d\r\n", len);		//��������
    if(!ESP8266_SendCmd(cmdBuf, ">",500))			//�յ���>��ʱ���Է�������
    {
        Usart_SendString(USART2, data, len);		//�����豸������������
        while(j--)
        {
            if(strstr((const char *)esp8266_buf,"SEND OK") != NULL)
            {
//                printf("OneNET_Send_Data_OK!%d\r\n",n);//n����
                ESP8266_Clear(0);	                
                return;
            }
            delay_ms(1);
        }
        printf("OneNET_Send_Erro1!\r\n");
    }
    ESP8266_Clear(0);
}
/*
*****************************************************************************************

��ʼ��ESP8266
ʧ�ܣ�1���ɹ���0
*****************************************************************************************
*/
u8 ESP8266_Setting(void)
{
    u8 times=10,i;   
    ESP8266_Clear(1);
    
    printf("1.ģʽ\r\n");
    i=times;
    while(ESP8266_SendCmd("AT+CWMODE=1\r\n", "OK",500))
    {
        i--;
        if(i==0)return 1;
    }       
    
    printf("2.����\r\n");
    i=times;
    while(ESP8266_SendCmd("AT+RST\r\n", "OK",500))
    {
        i--;
        if(i==0)return 1;
    }
    delay_ms(500);

    printf("3.�ػ���\r\n");
    i=times;
    while(ESP8266_SendCmd("ATE0\r\n", "OK",500))
    {
        i--;
        if(i==0)return 1;
    }            
    
    printf("4.�Ͽ�IP����\r\n");
    i=times;
    while(ESP8266_SendCmd("AT+CWQAP\r\n", "OK",500))
    {
        i--;
        if(i==0)return 1;
    }            
    
    printf("5.����WIFI\r\n");
    i=times;
    while(ESP8266_SendCmd(ESP8266_WIFI_NAME, "GOT IP",5000))
    {
        i--;
        if(i==0)return 1;
    }

    printf("6.����IP\r\n");
    i=times;
    while(ESP8266_SendCmd(ESP8266_OneNET_INFO, "CONNECT",5000))
    {
        i--;
        if(i==0)return 1;
    }

    printf("7.ESP��ʼ��OK\r\n");
    ESP8266_Clear(1);
    return 0;
}
/*
*****************************************************************************************

ESP��������
�ɹ���0
ʧ�ܣ�1
*****************************************************************************************
*/
u8 ESP8266_Connect_Again(void)
{
    u8 i;
    char data[50];
    
    memset(data,0,sizeof(data));
    printf("CIPSTART\r\n");
    i=2;
    while(ESP8266_SendCmd(ESP8266_OneNET_INFO, "CONNECT",5000))
    {
        i--;
        if(i==0)return 1;
    }
    while(!OneNet_EDP_Connect())delay_ms(500);		//����OneNET    
    printf("ESP8266 Connect Again OK\r\n");   
    return 0;
}
/*
*****************************************************************************************

��onenet��������
���ز�����	1-�ɹ�	0-ʧ��
*****************************************************************************************
*/
_Bool OneNet_EDP_Connect(void)
{

	char *dataPtr;
	_Bool status=0;
	printf("OneNet_DevLink\r\n""DEVID: %s,APIKEY: %s\r\n", DEVID, APIKEY);

	send_pkg = PacketConnect1(DEVID, APIKEY);						//����devid �� apikey��װЭ���
	
	ESP8266_Clear(0);												//��ջ���
	ESP8266_SendData(send_pkg->_data, send_pkg->_write_pos);		//�ϴ�ƽ̨
	
	delay_ms(100);
	if(strstr(esp8266_buf,WIFI_Rec_Keywords)!=NULL)
    {
        dataPtr = strchr((char *)esp8266_buf, ':');
        if(dataPtr != NULL)
        {
            dataPtr++;
            printf("����ɹ�\r\n");
            ESP8266_Clear(1);
            status=1;
        }
        else
        {
            printf("����ʧ��\r\n");
            ESP8266_Clear(1);
            status=0;
        }
    }
	DeleteBuffer(&send_pkg);										//ɾ��
	
	return status;
}
/*
*****************************************************************************************

�������ó�ʼ����������������

*****************************************************************************************
*/
void ESP8266_Init(void)
{		
	Usart2_Init(115200);				
	
	while(ESP8266_Setting());
    while(!OneNet_EDP_Connect())delay_ms(500);		//����OneNET
}
/*
*****************************************************************************************

ESP �������

*****************************************************************************************
*/

int test;
void OneNET_FillBuf(char *buf)
{
	char text[16];
	memset(text, 0, sizeof(text));
	strcpy(buf, "{");//װ���ַ�����������
	sprintf(text, "\"Temp\":%lf",(float)test);
	strcat(buf, text);
	strcat(buf, "}");
}
/*
*****************************************************************************************

ESP �������ݺ���

*****************************************************************************************
*/
void OneNET_SendData(void)
{
	char buf[300];
    
    memset(buf,0,sizeof(buf));
    
	OneNET_FillBuf(buf);														//��װ������
	
	send_pkg = PacketSaveJson(NULL, buf, kTypeSimpleJsonWithoutTime);			//���-Type3
		
	ESP8266_SendData(send_pkg->_data, send_pkg->_write_pos);					//�ϴ����ݵ�ƽ̨
		
	DeleteBuffer(&send_pkg);													//ɾ��
	
}
/*
************************************************************

����״̬���ݺ���

************************************************************
*/
void ESP8266_Wait_RecData(void)
{
    char *p,*p1;
    u8 i=0;
    static u16 j=0;   
    if((p=strstr((const char *)esp8266_buf,(const char *)WIFI_Rec_Keywords)) != NULL)  
    {
        delay_ms(100);  
        while(i<4)
        {
            if(*p=='\0')
            {
                *p='x';
                i++;
            }
            p++; 
        }
        printf("%s\r\n",p);
        if(strstr(p,"{")!=NULL)
        {
//            if((p1=strstr(p,"LED1:"))!=NULL)
//            {
//                p1+=5;
//                if(*p1=='0')
//                {
//                    LED1_OFF;
//                }
//                else if(*p1=='1')
//                {
//                    LED1_ON;
//                }
//            }
//            if((p1=strstr(p,"LED2:"))!=NULL)
//            {
//                p1+=5;
//                if(*p1=='0')
//                {
//                    LED2_OFF;
//                }
//                else if(*p1=='1')
//                {
//                    LED2_ON;
//                }        
//            }
//            if((p1=strstr(p,"LED3:"))!=NULL)
//            {
//                p1+=5;
//                if(*p1=='0')
//                {
//                    LED3_OFF;
//                }
//                else if(*p1=='1')
//                {
//                    LED3_ON;
//                }        
//            }
//            if((p1=strstr(p,"LED4:"))!=NULL)
//            {
//                p1+=5;
//                if(*p1=='0')
//                {
//                    LED4_OFF;
//                }
//                else if(*p1=='1')
//                {
//                    LED4_ON;
//                }        
//            } 
//			if((p1=strstr(p,"Beep:"))!=NULL)
//            {
//                p1+=5;
//                if(*p1=='0')
//                {
//                    Beep_OFF;
//                }
//                else if(*p1=='1')
//                {
//                    Beep_ON;
//                }        
//            } 
//			if((p1=strstr(p,"Set:"))!=NULL)
//            {
//                p1+=4;
//				setWeight = 0;
//				printf("%s",p1);
//				while(*p1!='}')
//				{
//					setWeight*=10;
//					setWeight+=(*p1-48);
//					p1++;
//				}
//            } 
        }
        ESP8266_Clear(1);//ǿ����շ��ͳɹ����ص�����        
    }
    if(j++>=100)
    {
        j=0;
        if(!ESP8266_SendCmd("AT+CIPSTATUS\r\n","STATUS:4",500))
        {
            ESP8266_Connect_Again();
        }         
    }
}

/*
*****************************************************************************************

����2�շ��ж�

*****************************************************************************************
*/
void USART2_IRQHandler(void)
{
	static u8 data;
    static u16 Size_ESP_Buf=sizeof(esp8266_buf);
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //�����ж�
	{
		if(esp8266_cnt >= Size_ESP_Buf)	
			esp8266_cnt = 0; //��ֹ���
		data=USART_ReceiveData(USART2);
		esp8266_buf[esp8266_cnt++] = data;
        esp8266_buf[esp8266_cnt]='\0';
		
//		USART_SendData(USART1,data);//ͨ������USARTx���͵�������
//		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==Bit_RESET);	
//        USART_ClearFlag(USART1,USART_FLAG_TC);
	}

}

