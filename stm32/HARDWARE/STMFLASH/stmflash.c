#include "stmflash.h"
#include "delay.h"


//////////////////////////////////////////////////////////////////////////////////	
//********************************************************************************
//V1.1�޸�˵��
//������STMFLASH_Write������ַƫ�Ƶ�һ��bug.
//////////////////////////////////////////////////////////////////////////////////
#define FLASH_SAVE_ADDR  0X0800E000 	//����FLASH �����ַ(����Ϊż��������ֵҪ���ڱ�������ռ��FLASH�Ĵ�С+0X08000000)
//����STM32��FLASH
void STMFLASH_Unlock(void)
{
	FLASH->KEYR=FLASH_KEY1;//д���������.
	FLASH->KEYR=FLASH_KEY2;
}
//flash����
void STMFLASH_Lock(void)
{
	FLASH->CR|=1<<7;//����
}
//�õ�FLASH״̬
u8 STMFLASH_GetStatus(void)
{	
	u32 res;		
	res=FLASH->SR; 
	if(res&(1<<0))return 1;		    //æ
	else if(res&(1<<2))return 2;	//��̴���
	else if(res&(1<<4))return 3;	//д��������
	return 0;						//�������
}
//�ȴ��������
//time:Ҫ��ʱ�ĳ���
//����ֵ:״̬.
u8 STMFLASH_WaitDone(u16 time)
{
	u8 res;
	do
	{
		res=STMFLASH_GetStatus();
		if(res!=1)break;//��æ,����ȴ���,ֱ���˳�.
		delay_us(1);
		time--;
	 }while(time);
	 if(time==0)res=0xff;//TIMEOUT
	 return res;
}
//����ҳ
//paddr:ҳ��ַ
//����ֵ:ִ�����
u8 STMFLASH_ErasePage(u32 paddr)
{
	u8 res=0;
	res=STMFLASH_WaitDone(0X5FFF);//�ȴ��ϴβ�������,>20ms    
	if(res==0)
	{ 
		FLASH->CR|=1<<1;//ҳ����
		FLASH->AR=paddr;//����ҳ��ַ 
		FLASH->CR|=1<<6;//��ʼ����		  
		res=STMFLASH_WaitDone(0X5FFF);//�ȴ���������,>20ms  
		if(res!=1)//��æ
		{
			FLASH->CR&=~(1<<1);//���ҳ������־.
		}
	}
	return res;
}
//��FLASHָ����ַд�����
//faddr:ָ����ַ(�˵�ַ����Ϊ2�ı���!!)
//dat:Ҫд�������
//����ֵ:д������
u8 STMFLASH_WriteHalfWord(u32 faddr, u16 dat)
{
	u8 res;	   	    
	res=STMFLASH_WaitDone(0XFF);	 
	if(res==0)//OK
	{
		FLASH->CR|=1<<0;//���ʹ��
		*(vu16*)faddr=dat;//д������
		res=STMFLASH_WaitDone(0XFF);//�ȴ��������
		if(res!=1)//�����ɹ�
		{
			FLASH->CR&=~(1<<0);//���PGλ.
		}
	} 
	return res;
} 
//��ȡָ����ַ�İ���(16λ����) 
//faddr:����ַ 
//����ֵ:��Ӧ����.
u16 STMFLASH_ReadHalfWord(u32 faddr)
{
	return *(vu16*)faddr; 
}
#if STM32_FLASH_WREN	//���ʹ����д   
//������д��
//WriteAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��   
void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
{ 			 		 
	u16 i;
	for(i=0;i<NumToWrite;i++)
	{
		STMFLASH_WriteHalfWord(WriteAddr,pBuffer[i]);
	    WriteAddr+=2;//��ַ����2.
	}  
} 
//��ָ����ַ��ʼд��ָ�����ȵ�����
//WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ2�ı���!!)
//pBuffer:����ָ��
//NumToWrite:����(16λ)��(����Ҫд���16λ���ݵĸ���.)
#if STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024 //�ֽ�
#else 
#define STM_SECTOR_SIZE	2048
#endif		 
u16 STMFLASH_BUF[STM_SECTOR_SIZE/2];//�����2K�ֽ�
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)	
{
	u32 secpos;	   //������ַ
	u16 secoff;	   //������ƫ�Ƶ�ַ(16λ�ּ���)
	u16 secremain; //������ʣ���ַ(16λ�ּ���)	   
 	u16 i;    
	u32 offaddr;   //ȥ��0X08000000��ĵ�ַ
	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))return;//�Ƿ���ַ
	STMFLASH_Unlock();						//����
	offaddr=WriteAddr-STM32_FLASH_BASE;		//ʵ��ƫ�Ƶ�ַ.
	secpos=offaddr/STM_SECTOR_SIZE;			//������ַ  0~127 for STM32F103RBT6
	secoff=(offaddr%STM_SECTOR_SIZE)/2;		//�������ڵ�ƫ��(2���ֽ�Ϊ������λ.)
	secremain=STM_SECTOR_SIZE/2-secoff;		//����ʣ��ռ��С   
	if(NumToWrite<=secremain)secremain=NumToWrite;//�����ڸ�������Χ
	while(1) 
	{	
		STMFLASH_Read(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(STMFLASH_BUF[secoff+i]!=0XFFFF)break;//��Ҫ����  	  
		}
		if(i<secremain)//��Ҫ����
		{
			STMFLASH_ErasePage(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);//�����������
			for(i=0;i<secremain;i++)//����
			{
				STMFLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//д����������  
		}else STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		if(NumToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;				//������ַ��1
			secoff=0;				//ƫ��λ��Ϊ0 	 
		   	pBuffer+=secremain;  	//ָ��ƫ��
			WriteAddr+=secremain*2;	//д��ַƫ��(16λ���ݵ�ַ,��Ҫ*2)	   
		   	NumToWrite-=secremain;	//�ֽ�(16λ)���ݼ�
			if(NumToWrite>(STM_SECTOR_SIZE/2))secremain=STM_SECTOR_SIZE/2;//��һ����������д����
			else secremain=NumToWrite;//��һ����������д����
		}	 
	};	
	STMFLASH_Lock();//����
}
#endif
//��ָ����ַ��ʼ����ָ�����ȵ�����
//ReadAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)   	
{
	u16 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//��ȡ2���ֽ�.
		ReadAddr+=2;//ƫ��2���ֽ�.	
	}
}

//////////////////////////////////////////������///////////////////////////////////////////
//WriteAddr:��ʼ��ַ
//WriteData:Ҫд�������
void Test_Write(u32 WriteAddr,u16 WriteData)   	
{
	STMFLASH_Write(WriteAddr,&WriteData,1);//д��һ���� 
}


////////////////////////////////////////����Ҫʵ��////////////////////////////////////////

u16 XServo,YServo;
u16 PID_Parameter[23],Flash_Parameter[23];

/*************************
* ����ϵͳ��PID�������Ÿ��̶��㡢�����ʼ�Ƕ�
*************************/
void Flash_Write(void)
{
//	u8 i;
//	Flash_Parameter[0]=XPointPID.XPointKp*1000;		
//	Flash_Parameter[1]=XPointPID.XPointKi*1000;	
//	Flash_Parameter[2]=XPointPID.XPointKd*1000;	
//	Flash_Parameter[3]=XPointPID.RollKp*1000;
//	Flash_Parameter[4]=XPointPID.RollKi*1000;
//	Flash_Parameter[5]=XPointPID.RollKd*1000;
//	Flash_Parameter[6]=YPointPID.YPointKp*1000;
//	Flash_Parameter[7]=YPointPID.YPointKi*1000;
//	Flash_Parameter[8]=YPointPID.YPointKd*1000;
//	Flash_Parameter[9]=YPointPID.PitchKp*1000;
//	Flash_Parameter[10]=YPointPID.PitchKi*1000;
//	Flash_Parameter[11]=YPointPID.PitchKd*1000;
//	for(i=0;i<9;i++)
//	{
//		Flash_Parameter[12+i]=DefauPoint[i].x<<8|DefauPoint[i].y;	
//	}
//	Flash_Parameter[21]=XServo;
//	Flash_Parameter[22]=YServo;
//	STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)Flash_Parameter,23);	
}	

void Flash_WritePID(void)
{
	STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)Flash_Parameter,12);	
}

void Flash_Read(void)
{
//	u8 i;
//	STMFLASH_Read(FLASH_SAVE_ADDR,(u16*)PID_Parameter,23);
//	if(PID_Parameter[0]==65535&&PID_Parameter[22]==65535)
//	{
//		//��д��ֵ
//	}
//	else
//	{		
//		XPointPID.XPointKp=(float)PID_Parameter[0]/1000;
//		XPointPID.XPointKi=(float)PID_Parameter[1]/1000;
//		XPointPID.XPointKd=(float)PID_Parameter[2]/1000;
//		XPointPID.RollKp=(float)PID_Parameter[3]/1000;
//		XPointPID.RollKi=(float)PID_Parameter[4]/1000;
//		XPointPID.RollKd=(float)PID_Parameter[5]/1000;
//		YPointPID.YPointKp=(float)PID_Parameter[6]/1000;
//		YPointPID.YPointKi=(float)PID_Parameter[7]/1000;
//		YPointPID.YPointKd=(float)PID_Parameter[8]/1000;
//		YPointPID.PitchKp=(float)PID_Parameter[9]/1000;
//		YPointPID.PitchKi=(float)PID_Parameter[10]/1000;
//		YPointPID.PitchKd=(float)PID_Parameter[11]/1000;
//		for(i=0;i<9;i++)
//		{
//			DefauPoint[i].x=PID_Parameter[12+i]>>8;
//			DefauPoint[i].y=PID_Parameter[12+i];
//		}
//		XServo=PID_Parameter[21];
//		YServo=PID_Parameter[22];
//	}
}

void Flash_ReadPID(void)
{
	STMFLASH_Read(FLASH_SAVE_ADDR,(u16*)PID_Parameter,12);
	if(PID_Parameter[0]==65535&&PID_Parameter[1]==65535)
	{
		//��д��ֵ
	}
	else
	{		
//		XPointPID.XPointKp=(float)PID_Parameter[0]/1000;
//		XPointPID.XPointKi=(float)PID_Parameter[1]/1000;
//		XPointPID.XPointKd=(float)PID_Parameter[2]/1000;
//		XPointPID.RollKp=(float)PID_Parameter[3]/1000;
//		XPointPID.RollKi=(float)PID_Parameter[4]/1000;
//		XPointPID.RollKd=(float)PID_Parameter[5]/1000;
//		YPointPID.YPointKp=(float)PID_Parameter[6]/1000;
//		YPointPID.YPointKi=(float)PID_Parameter[7]/1000;
//		YPointPID.YPointKd=(float)PID_Parameter[8]/1000;
//		YPointPID.PitchKp=(float)PID_Parameter[9]/1000;
//		YPointPID.PitchKi=(float)PID_Parameter[10]/1000;
//		YPointPID.PitchKd=(float)PID_Parameter[11]/1000;
	}
}	






