#include "scope.h"
#include "control.h"
#include "ili9320.h"

////������ƽ��С����λ�����ݷ�װ������

unsigned char DataScope_OutPut_Buffer[42] = {0};	   //���ڷ��ͻ�����

//����˵�����������ȸ�������ת��4�ֽ����ݲ�����ָ����ַ 
//����˵�����û�����ֱ�Ӳ����˺��� 
//target:Ŀ�굥��������
//buf:��д������
//beg:ָ��������ڼ���Ԫ�ؿ�ʼд��
//�����޷��� 
void Float2Byte(float *target,unsigned char *buf,unsigned char beg)
{
    unsigned char *point;
    point = (unsigned char*)target;	  //�õ�float�ĵ�ַ
    buf[beg]   = point[0];
    buf[beg+1] = point[1];
    buf[beg+2] = point[2];
    buf[beg+3] = point[3];
}
 
 
//����˵������������ͨ���ĵ����ȸ�������д�뷢�ͻ�����
//Data��ͨ������
//Channel��ѡ��ͨ����1-10��
//�����޷��� 
void DataScope_Get_Channel_Data(float Data,unsigned char Channel)
{
	if ( (Channel > 10) || (Channel == 0) ) return;  //ͨ����������10�����0��ֱ����������ִ�к���
	else
	{
		switch (Channel)
		{
		  case 1:  Float2Byte(&Data,DataScope_OutPut_Buffer,1); break;
		  case 2:  Float2Byte(&Data,DataScope_OutPut_Buffer,5); break;
		  case 3:  Float2Byte(&Data,DataScope_OutPut_Buffer,9); break;
		  case 4:  Float2Byte(&Data,DataScope_OutPut_Buffer,13); break;
		  case 5:  Float2Byte(&Data,DataScope_OutPut_Buffer,17); break;
		  case 6:  Float2Byte(&Data,DataScope_OutPut_Buffer,21); break;
		  case 7:  Float2Byte(&Data,DataScope_OutPut_Buffer,25); break;
		  case 8:  Float2Byte(&Data,DataScope_OutPut_Buffer,29); break;
		  case 9:  Float2Byte(&Data,DataScope_OutPut_Buffer,33); break;
		  case 10: Float2Byte(&Data,DataScope_OutPut_Buffer,37); break;
		}
	}	 
}


//����˵�������� DataScopeV1.0 ����ȷʶ���֡��ʽ
//Channel_Number����Ҫ���͵�ͨ������
//���ط��ͻ��������ݸ���
//����0��ʾ֡��ʽ����ʧ�� 
unsigned char DataScope_Data_Generate(unsigned char Channel_Number)
{
	if ( (Channel_Number > 10) || (Channel_Number == 0) ) { return 0; }  //ͨ����������10�����0��ֱ����������ִ�к���
	else
	{	
		DataScope_OutPut_Buffer[0] = '$';  //֡ͷ		
		switch(Channel_Number)   
		{ 
			case 1:   DataScope_OutPut_Buffer[5]  =  5; return  6;
			case 2:   DataScope_OutPut_Buffer[9]  =  9; return 10;
			case 3:   DataScope_OutPut_Buffer[13] = 13; return 14;
			case 4:   DataScope_OutPut_Buffer[17] = 17; return 18;
			case 5:   DataScope_OutPut_Buffer[21] = 21; return 22;
			case 6:   DataScope_OutPut_Buffer[25] = 25; return 26;
			case 7:   DataScope_OutPut_Buffer[29] = 29; return 30; 
			case 8:   DataScope_OutPut_Buffer[33] = 33; return 34; 
			case 9:   DataScope_OutPut_Buffer[37] = 37; return 38;
			case 10:  DataScope_OutPut_Buffer[41] = 41; return 42; 
		}	 
	}
	return 0;
}

////���ݷ�װ����Ϊ��������  ����ȥΪһ��42�ֳ�����ͷһ���֣�ÿͨ��4���֣����ʮ��ͨ�������һ��Ϊ����У�飩
extern u16 adc1Ch2Num, adc1Ch3Num;
void ScopeOnComputer(void)
{
	u8 t,len;
	DataScope_Get_Channel_Data((float)adc1Ch2Num,1);
	DataScope_Get_Channel_Data((float)adc1Ch3Num,2);
	DataScope_Get_Channel_Data((float)motor.Set,3);
	DataScope_Get_Channel_Data((float)motor.Out,4);
	len=DataScope_Data_Generate(4);						//�õ��˴ν��յ������ݳ���
	for(t=0;t<len;t++)
	{
		USART_SendData(USART1, DataScope_OutPut_Buffer[t]);//�򴮿�1��������
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	}
}

void Grid(int start_x, u16 color)
{
	int i=0, j=0;
	for(i = 0; i < 11; i++)
	{
		for(j = 0; j < 41; j++)
		{
			ili9320_SetPoint(start_x + i * 25, j * 5, color);
		}
	}
	for(i = 0; i < 9; i++)
	{
		for(j = 0; j < 51; j++)
		{
			ili9320_SetPoint(start_x + j * 5, i * 25, color);
		}
	}
}

u8 setWave[251], curWave[251], ptr;

void Wave(int start_x)
{
	int i;
	setWave[ptr] = motor.Set / 45;
	//curWave[ptr] = adc1Ch2Num / 20.48;
	curWave[ptr] = motor.Cur / 50;
	for(i = ptr + 1; i < 251; i++)
	{
		ili9320_SetPoint(start_x + i - ptr - 1, 200 - setWave[i], Yellow);
		ili9320_SetPoint(start_x + i - ptr - 1, 200 - curWave[i], Blue);
	}
	for(i = 0; i <= ptr; i++)
	{
		ili9320_SetPoint(start_x + i + 250 - ptr, 200 - setWave[i], Yellow);
		ili9320_SetPoint(start_x + i + 250 - ptr, 200 - curWave[i], Blue);
	}
	if(++ptr == 251)
		ptr = 0;
}

void ScopeOnScreen(void)
{
	int i;
	int start_x = 35;
	//������ϴλ�����
	for(i = ptr; i < 251; i++)
	{
		ili9320_SetPoint(start_x + i - ptr, 200 - setWave[i], Black);
		ili9320_SetPoint(start_x + i - ptr, 200 - curWave[i], Black);
	}
	for(i = 0; i < ptr; i++)
	{
		ili9320_SetPoint(start_x + i + 251 - ptr, 200 - setWave[i], Black);
		ili9320_SetPoint(start_x + i + 251 - ptr, 200 - curWave[i], Black);
	}
	Grid(start_x, Green);
	Wave(start_x);
}

u8 setWave2[51], curWave2[51], ptr2;

void Wave2(int start_x)
{
	int i;
	setWave2[ptr2] = motor.Set / 4;
	//curWave2[ptr2] = adc1Ch2Num / 20.48;
	curWave2[ptr2] = motor.Cur / 4;
	for(i = ptr2 + 1; i < 50; i++)
	{
		GUI_Line(start_x + (i - ptr2 - 1) * 5, 200 - setWave2[i], start_x + (i - ptr2) * 5, 200 - setWave2[i + 1], Yellow);
		GUI_Line(start_x + (i - ptr2 - 1) * 5, 200 - curWave2[i], start_x + (i - ptr2) * 5, 200 - curWave2[i + 1], Blue);
	}
	if(ptr2 <= 49)
	{
		GUI_Line(start_x + (49 - ptr2) * 5, 200 - setWave2[50], start_x + (50 - ptr2) * 5, 200 - setWave2[0], Yellow);
		GUI_Line(start_x + (49 - ptr2) * 5, 200 - curWave2[50], start_x + (50 - ptr2) * 5, 200 - curWave2[0], Blue);
	}
	for(i = 0; i < ptr2; i++)
	{
		GUI_Line(start_x + (i + 50 - ptr2) * 5, 200 - setWave2[i], start_x + (i + 51 - ptr2) * 5, 200 - setWave2[i + 1], Yellow);
		GUI_Line(start_x + (i + 50 - ptr2) * 5, 200 - curWave2[i], start_x + (i + 51 - ptr2) * 5, 200 - curWave2[i + 1], Blue);
	}
	if(++ptr2 == 51)
		ptr2 = 0;
}

void ScopeOnScreen2(void)
{
	int i;
	int start_x = 35;
	//������ϴλ����� Խ��Խ��Ϳ
	if(ptr2 == 0)
		ptr2 = 50;
	else
		ptr2--;
	
	for(i = ptr2 + 1; i < 50; i++)
	{
		GUI_Line(start_x + (i - ptr2 - 1) * 5, 200 - setWave2[i], start_x + (i - ptr2) * 5, 200 - setWave2[i + 1], Black);
		GUI_Line(start_x + (i - ptr2 - 1) * 5, 200 - curWave2[i], start_x + (i - ptr2) * 5, 200 - curWave2[i + 1], Black);
	}
	if(ptr2 <= 49)
	{
		GUI_Line(start_x + (49 - ptr2) * 5, 200 - setWave2[50], start_x + (50 - ptr2) * 5, 200 - setWave2[0], Black);
		GUI_Line(start_x + (49 - ptr2) * 5, 200 - curWave2[50], start_x + (50 - ptr2) * 5, 200 - curWave2[0], Black);
	}
	for(i = 0; i < ptr2; i++)
	{
		GUI_Line(start_x + (i + 50 - ptr2) * 5, 200 - setWave2[i], start_x + (i + 51 - ptr2) * 5, 200 - setWave2[i + 1], Black);
		GUI_Line(start_x + (i + 50 - ptr2) * 5, 200 - curWave2[i], start_x + (i + 51 - ptr2) * 5, 200 - curWave2[i + 1], Black);
	}
	if(++ptr2 == 51)
		ptr2 = 0;
//	for(i = ptr2; i < 50; i++)
//	{
//		GUI_Line(start_x + (i - ptr2) * 5, 200 - setWave2[i], start_x + (i - ptr2 + 1) * 5, 200 - setWave2[i + 1], Black);
//		GUI_Line(start_x + (i - ptr2) * 5, 200 - curWave2[i], start_x + (i - ptr2 + 1) * 5, 200 - curWave2[i + 1], Black);
//	}
//	for(i = 0; i < ptr2; i++)
//	{
//		GUI_Line(start_x + (i + 49 - ptr2) * 5, 200 - setWave2[i], start_x + (i + 50 - ptr2) * 5, 200 - setWave2[i], Black);
//		GUI_Line(start_x + (i + 49 - ptr2) * 5, 200 - curWave2[i], start_x + (i + 50 - ptr2) * 5, 200 - curWave2[i], Black);
//	}
	Grid(start_x, Green);
	Wave2(start_x);
}
