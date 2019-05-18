#include "timer.h"
#include "usart.h"
#include "adda.h"
#include "ili9320.h"
#include "scope.h"
#include "control.h"
#include "touch_spi2.h"

/*
**��ʱ�����жϵĹ���  
**ʹ��ʱ�������ȼ�������xxx�滻Ϊ2��3��4��5��6��7����
**��ʱ��1��8ʱ����APB2�µģ���Ҫ�޸�ʱ��ʹ�ܺ������жϺ������ֲ�ͬ����Ҫ�޸�
*/
//void TIMxxx_Int_Init(u16 arr,u16 psc)
//{
//    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIMxxx, ENABLE); //ʱ��ʹ��

//	TIM_TimeBaseStructure.TIM_Period = arr;			//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
//	TIM_TimeBaseStructure.TIM_Prescaler =psc;		//����������ΪTIMxxxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 	//����ʱ�ӷָ�:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
//	TIM_TimeBaseInit(TIMxxx, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMxxx��ʱ�������λ
// 
//	TIM_ClearITPendingBit(TIMxxx,TIM_IT_Update); //���TIMxxx�����жϱ�־����ֹ��ʱ��һ��ʼ���ͽ���һ��
//	TIM_ITConfig(TIMxxx,TIM_IT_Update,ENABLE ); 	//ʹ��ָ����TIMxxx�ж�,��������ж�
//	
//	NVIC_InitStructure.NVIC_IRQChannel = TIMxxx_IRQn;  //TIMxxx�ж�
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1 ;  //��ռ���ȼ�
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1 ;  //�����ȼ�
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
//	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

//	TIM_Cmd(TIMxxx, ENABLE);  //ʹ��TIMxxx����						 
//}

//void TIMxxx_IRQHandler(void)   //TIMxxx�ж�
//{
//	if (TIM_GetITStatus(TIMxxx, TIM_IT_Update) != RESET) 	 //���ָ����TIMxxx�жϷ������:TIMxxx�ж�Դ 
//	{
//		TIM_ClearITPendingBit(TIMxxx,TIM_IT_Update);  	 //���TIMxxx���жϴ�����λ:TIMxxx�ж�Դ 
//		
//	}
//}
void TIM5_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr;			//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc;		//����������ΪTIM5ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 	//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIM5��ʱ�������λ
 
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update); //���TIM5�����жϱ�־����ֹ��ʱ��һ��ʼ���ͽ���һ��
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE ); 	//ʹ��ָ����TIM5�ж�,��������ж�
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM5�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1 ;  //��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1 ;  //�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM5, ENABLE);  //ʹ��TIM5����						 
}





/*
**���벶��
**Ĭ��һ�β��񲻻ᳬ��������ڣ�������Ҫ������ж��ڼ�¼�����ӵ���������
*/
//int TIMxxxCH1val=0,TIMxxxCH2val=0,TIMxxxCH3val=0,TIMxxxCH4val=0;		//���
//u8  TIMxxxCH1=0,TIMxxxCH2=0,TIMxxxCH3=0,TIMxxxCH4=0;					//���벶��״̬		    				
//u16 CAP_TIM_PERIOD;
//u16	TIMxxxCH1_Start,TIMxxxCH1_End,TIMxxxCH2_Start,TIMxxxCH2_End,TIMxxxCH3_Start,TIMxxxCH3_End,TIMxxxCH4_Start,TIMxxxCH4_End;	//���벶��ֵ
//void TIMxxx_Cap_Init(u16 arr,u16 psc)
//{	 
//	GPIO_InitTypeDef GPIO_InitStructure;
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	TIM_ICInitTypeDef  TIMxxx_ICInitStructure;
//   	NVIC_InitTypeDef NVIC_InitStructure;

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIMxxx, ENABLE);	//ʹ��TIMxxxʱ��
// 	RCC_APB2PeriphClockCmd( , ENABLE);  //ʹ��GPIOʱ��  RCC_APB2Periph_GPIOyyy
//	
////	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); 	//AFIO���ù���ģ��ʱ��
////	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE);		//ѡ����ȫ��ӳ��
//
//	GPIO_InitStructure.GPIO_Pin  = ;  	//GPIO_Pin_0
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//PA0 ����  
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init( , &GPIO_InitStructure);  //GPIOyyy
////	GPIO_ResetBits(GPIOA,GPIO_Pin_0);				
//	
//	//��ʼ����ʱ��xxx
//	CAP_TIM_PERIOD=arr;
//	TIM_TimeBaseStructure.TIM_Period = arr; 				//�趨�������Զ���װֵ 
//	TIM_TimeBaseStructure.TIM_Prescaler =psc; 				//Ԥ��Ƶ��   
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIMxxx���ϼ���ģʽ
//	TIM_TimeBaseInit(TIMxxx, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMxxx��ʱ�������λ
//  
//	//��ʼ��TIMxxx���벶�����
//	TIMxxx_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
//  	TIMxxx_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
//  	TIMxxx_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
//  	TIMxxx_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
//  	TIMxxx_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
//  	TIM_ICInit(TIMxxx, &TIMxxx_ICInitStructure);
//	TIMxxx_ICInitStructure.TIM_Channel = TIM_Channel_2;
//  	TIM_ICInit(TIMxxx, &TIMxxx_ICInitStructure);
//	TIMxxx_ICInitStructure.TIM_Channel = TIM_Channel_3;
//  	TIM_ICInit(TIMxxx, &TIMxxx_ICInitStructure);
//	TIMxxx_ICInitStructure.TIM_Channel = TIM_Channel_4;
//  	TIM_ICInit(TIMxxx, &TIMxxx_ICInitStructure);
//	
//	//�жϷ����ʼ��
//	NVIC_InitStructure.NVIC_IRQChannel = TIMxxx_IRQn;  //TIMxxx�ж�
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
//	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
//	
//	TIM_ClearITPendingBit(TIMxxx,TIM_IT_Update|TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4); //����жϱ�־λ
//	TIM_ITConfig(TIMxxx,TIM_IT_Update|TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4,ENABLE);//��������ж� ,����CC1\2\3\4IE�����ж�	
//	
//   	TIM_Cmd(TIMxxx,ENABLE ); 	//ʹ�ܶ�ʱ��xxx 
//}
//void TIMxxx_IRQHandler(void)
//{ 
//	if(TIM_GetITStatus(TIMxxx, TIM_IT_CC1) != RESET)//����1���������¼�
//	{	
//		if(TIMxxxCH1)		//����һ���½��� 		
//		{	 
//			TIMxxxCH1=0;
//			TIMxxxCH1_End=TIM_GetCounter(TIMxxx);
//			TIMxxxCH1val=TIMxxxCH1_End-TIMxxxCH1_Start;
//			if(TIMxxxCH1val<0)	TIMxxxCH1val+=CAP_TIM_PERIOD;
//			//������� ��Ҫʱ�������
//			TIM_OC1PolarityConfig(TIMxxx,TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���
//		}
//		else  								//��δ��ʼ,��һ�β���������
//		{
//			TIMxxxCH1=1;
//			TIMxxxCH1_Start=TIM_GetCounter(TIMxxx);
//			TIM_OC1PolarityConfig(TIMxxx,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���
//		}
//	}		
//	if(TIM_GetITStatus(TIMxxx, TIM_IT_CC2) != RESET)//����2���������¼�
//	{	
//		if(TIMxxxCH2)		//����һ���½��� 		
//		{	 
//			TIMxxxCH2=0;
//			TIMxxxCH2_End=TIM_GetCounter(TIMxxx);
//			TIMxxxCH2val=TIMxxxCH2_End-TIMxxxCH2_Start;
//			//������� ��Ҫʱ�������
//			TIM_OC2PolarityConfig(TIMxxx,TIM_ICPolarity_Rising); //CC2P=0 ����Ϊ�����ز���
//		}
//		else  								//��δ��ʼ,��һ�β���������
//		{
//			TIMxxxCH2=1;
//			TIMxxxCH2_Start=TIM_GetCounter(TIMxxx);
//			TIM_OC2PolarityConfig(TIMxxx,TIM_ICPolarity_Falling);		//CC2P=1 ����Ϊ�½��ز���
//		}
//	}
//	if(TIM_GetITStatus(TIMxxx, TIM_IT_CC3) != RESET)//����3���������¼�
//	{	
//		if(TIMxxxCH3)		//����һ���½��� 		
//		{	 
//			TIMxxxCH3=0;
//			TIMxxxCH3_End=TIM_GetCounter(TIMxxx);
//			TIMxxxCH3val=TIMxxxCH3_End-TIMxxxCH3_Start;
//			//������� ��Ҫʱ�������
//			TIM_OC3PolarityConfig(TIMxxx,TIM_ICPolarity_Rising); //CC3P=0 ����Ϊ�����ز���
//		}
//		else  								//��δ��ʼ,��һ�β���������
//		{
//			TIMxxxCH3=1;
//			TIMxxxCH3_Start=TIM_GetCounter(TIMxxx);
//			TIM_OC3PolarityConfig(TIMxxx,TIM_ICPolarity_Falling);		//CC3P=1 ����Ϊ�½��ز���
//		}
//	}
//	if(TIM_GetITStatus(TIMxxx, TIM_IT_CC4) != RESET)//����4���������¼�
//	{	
//		if(TIMxxxCH4)		//����һ���½��� 		
//		{	 
//			TIMxxxCH4=0;
//			TIMxxxCH4_End=TIM_GetCounter(TIMxxx);
//			TIMxxxCH4val=TIMxxxCH4_End-TIMxxxCH4_Start;
//			//������� ��Ҫʱ�������
//			TIM_OC4PolarityConfig(TIMxxx,TIM_ICPolarity_Rising); //CC4P=0 ����Ϊ�����ز���
//		}
//		else  								//��δ��ʼ,��һ�β���������
//		{
//			TIMxxxCH4=1;
//			TIMxxxCH4_Start=TIM_GetCounter(TIMxxx);
//			TIM_OC4PolarityConfig(TIMxxx,TIM_ICPolarity_Falling);		//CC4P=1 ����Ϊ�½��ز���
//		}
//	}
//	//�ǲ��Ƿֱ������һ�㣬���Ա������ж�Դͬʱ�������ж�©�������Ŀ��ܣ�
//    TIM_ClearITPendingBit(TIMxxx,TIM_IT_Update|TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4); //����жϱ�־λ
//}
int TIM3CH1val=0,TIM3CH2val=0,TIM3CH3val=0,TIM3CH4val=0;		//���
u8  TIM3CH1=0,TIM3CH2=0,TIM3CH3=0,TIM3CH4=0;					//���벶��״̬		    				
u16 CAP_TIM_PERIOD;
u16	TIM3CH1_Start,TIM3CH1_End,TIM3CH2_Start,TIM3CH2_End,TIM3CH3_Start,TIM3CH3_End,TIM3CH4_Start,TIM3CH4_End;	//���벶��ֵ
void TIM3_Cap_Init(u16 arr,u16 psc)
{	 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM3_ICInitStructure;
   	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ʹ��TIM3ʱ��
 	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE);  //ʹ��GPIOʱ��  RCC_APB2Periph_GPIOyyy
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); 	//AFIO���ù���ģ��ʱ��
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE);		//ѡ����ȫ��ӳ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;  	//GPIO_Pin_0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//PA0 ����  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOC, &GPIO_InitStructure);  //GPIOyyy
//	GPIO_ResetBits(GPIOA,GPIO_Pin_0);				
	
	//��ʼ����ʱ��3
	CAP_TIM_PERIOD=arr;
	TIM_TimeBaseStructure.TIM_Period = arr; 				//�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 				//Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM3���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIM3��ʱ�������λ
  
	//��ʼ��TIM3���벶�����
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
  	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
  	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
  	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
  	TIM3_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
  	TIM_ICInit(TIM3, &TIM3_ICInitStructure);
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_2;
  	TIM_ICInit(TIM3, &TIM3_ICInitStructure);
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_3;
  	TIM_ICInit(TIM3, &TIM3_ICInitStructure);
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_4;
  	TIM_ICInit(TIM3, &TIM3_ICInitStructure);
	
	//�жϷ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
	
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update|TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4); //����жϱ�־λ
	TIM_ITConfig(TIM3,TIM_IT_Update|TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4,ENABLE);//��������ж� ,����CC1\2\3\4IE�����ж�	
	
   	TIM_Cmd(TIM3,ENABLE ); 	//ʹ�ܶ�ʱ��3 
}
void TIM3_IRQHandler(void)
{ 
	if(TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)//����1���������¼�
	{	
		TIM3CH1val++;
		if(TIM3CH1)		//����һ���½��� 		
		{	 
			TIM3CH1=0;
			TIM3CH1_End=TIM_GetCounter(TIM3);
			TIM3CH1val=TIM3CH1_End-TIM3CH1_Start;
			if(TIM3CH1val<0)	TIM3CH1val+=CAP_TIM_PERIOD;
			//������� ��Ҫʱ�������
			TIM_OC1PolarityConfig(TIM3,TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���
		}
		else  								//��δ��ʼ,��һ�β���������
		{
			TIM3CH1=1;
			TIM3CH1_Start=TIM_GetCounter(TIM3);
			TIM_OC1PolarityConfig(TIM3,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���
		}
	}		
	if(TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET)//����2���������¼�
	{	
		if(TIM3CH2)		//����һ���½��� 		
		{	 
			TIM3CH2=0;
			TIM3CH2_End=TIM_GetCounter(TIM3);
			TIM3CH2val=TIM3CH2_End-TIM3CH2_Start;
			//������� ��Ҫʱ�������
			TIM_OC2PolarityConfig(TIM3,TIM_ICPolarity_Rising); //CC2P=0 ����Ϊ�����ز���
		}
		else  								//��δ��ʼ,��һ�β���������
		{
			TIM3CH2=1;
			TIM3CH2_Start=TIM_GetCounter(TIM3);
			TIM_OC2PolarityConfig(TIM3,TIM_ICPolarity_Falling);		//CC2P=1 ����Ϊ�½��ز���
		}
	}
	if(TIM_GetITStatus(TIM3, TIM_IT_CC3) != RESET)//����3���������¼�
	{	
		if(TIM3CH3)		//����һ���½��� 		
		{	 
			TIM3CH3=0;
			TIM3CH3_End=TIM_GetCounter(TIM3);
			TIM3CH3val=TIM3CH3_End-TIM3CH3_Start;
			//������� ��Ҫʱ�������
			TIM_OC3PolarityConfig(TIM3,TIM_ICPolarity_Rising); //CC3P=0 ����Ϊ�����ز���
		}
		else  								//��δ��ʼ,��һ�β���������
		{
			TIM3CH3=1;
			TIM3CH3_Start=TIM_GetCounter(TIM3);
			TIM_OC3PolarityConfig(TIM3,TIM_ICPolarity_Falling);		//CC3P=1 ����Ϊ�½��ز���
		}
	}
	if(TIM_GetITStatus(TIM3, TIM_IT_CC4) != RESET)//����4���������¼�
	{	
		if(TIM3CH4)		//����һ���½��� 		
		{	 
			TIM3CH4=0;
			TIM3CH4_End=TIM_GetCounter(TIM3);
			TIM3CH4val=TIM3CH4_End-TIM3CH4_Start;
			//������� ��Ҫʱ�������
			TIM_OC4PolarityConfig(TIM3,TIM_ICPolarity_Rising); //CC4P=0 ����Ϊ�����ز���
		}
		else  								//��δ��ʼ,��һ�β���������
		{
			TIM3CH4=1;
			TIM3CH4_Start=TIM_GetCounter(TIM3);
			TIM_OC4PolarityConfig(TIM3,TIM_ICPolarity_Falling);		//CC4P=1 ����Ϊ�½��ز���
		}
	}
	//�ǲ��Ƿֱ������һ�㣬���Ա������ж�Դͬʱ�������ж�©�������Ŀ��ܣ�
    TIM_ClearITPendingBit(TIM3,TIM_IT_Update|TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4); //����жϱ�־λ
}




/*
**������ģʽ
*/
//u16 TIMxxx_Encoder_Full;	//���������������¼
//u16 ENCODER_TIM_PERIOD;
//void TIMxxx_Encoder_Init(u16 arr)
//{
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
//	TIM_ICInitTypeDef TIM_ICInitStructure;  
//	GPIO_InitTypeDef GPIO_InitStructure;
//	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIMxxx, ENABLE);	//ʹ��TIMxxxʱ��
// 	RCC_APB2PeriphClockCmd( , ENABLE);  //ʹ��GPIOʱ��  RCC_APB2Periph_GPIOyyy
//	
////	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); 	//AFIO���ù���ģ��ʱ��
////	GPIO_PinRemapConfig(GPIO_FullRemap_TIMxxx,ENABLE);		//ѡ����ȫ��ӳ��
//	
//	GPIO_InitStructure.GPIO_Pin  = ;  	//GPIO_Pin_0
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//PA0 ����  
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init( , &GPIO_InitStructure);  //GPIOyyy
////	GPIO_ResetBits(GPIOA,GPIO_Pin_0);				

//	ENCODER_TIM_PERIOD = arr;//�����������
//	TIM_TimeBaseStructure.TIM_Period = arr; 		//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
//	TIM_TimeBaseStructure.TIM_Prescaler =0; 		//����������ΪTIMxxxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 	//����ʱ�ӷָ�:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
//	TIM_TimeBaseInit(TIMxxx, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMxxx��ʱ�������λ
//	
//	TIM_EncoderInterfaceConfig(TIMxxx, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ3
//	TIM_ICStructInit(&TIM_ICInitStructure);
//	TIM_ICInitStructure.TIM_ICFilter = 10;
//	TIM_ICInit(TIMxxx, &TIM_ICInitStructure);
//	
//	TIM_ClearFlag(TIMxxx, TIM_FLAG_Update);		 //���TIM�ĸ��±�־λ
//	TIM_ClearITPendingBit(TIMxxx,TIM_IT_Update); //���TIMxxx�����жϱ�־����ֹ��ʱ��һ��ʼ���ͽ���һ��
//	TIM_ITConfig(TIMxxx, TIM_IT_Update, ENABLE);

//	TIM_SetCounter(TIMxxx,arr/2);//CNT����
//	TIM_Cmd(TIMxxx, ENABLE); 
//}

//void TIMxxx_IRQHandler(void)   //TIMxxx�ж�
//{
//	if (TIM_GetITStatus(TIMxxx, TIM_IT_Update) != RESET) 	 //���ָ����TIMxxx�жϷ������:TIMxxx�ж�Դ 
//	{
//		TIM_ClearITPendingBit(TIMxxx,TIM_IT_Update);  	 //���TIMxxx���жϴ�����λ:TIMxxx�ж�Դ 
//		TIMxxx_Encoder_Full++;//����ۼӼ�������
//	}
//}
//int Read_Encoder(TIM_TypeDef* TIMx) //���������������ȡ�����ڲ���ת��arr/2;
//{
//	int Encoder_TIM;    
////	Encoder_TIM= (TIMx -> CNT)+ENCODER_TIM_PERIOD *TIMxxx_Encoder_Full;  
////	TIMxxx_Encoder_Full=0;
//	Encoder_TIM=(TIMx -> CNT)-ENCODER_TIM_PERIOD/2;
//	TIMx -> CNT =ENCODER_TIM_PERIOD/2;	
//	return Encoder_TIM;
//}
u16 TIM2_Encoder_Full;	//���������������¼
u16 ENCODER_TIM_PERIOD;
void TIM2_Encoder_Init(u16 arr)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
	TIM_ICInitTypeDef TIM_ICInitStructure;  
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//ʹ��TIM2ʱ��
 	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);  //ʹ��GPIOʱ��  RCC_APB2Periph_GPIOyyy
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); 	//AFIO���ù���ģ��ʱ��
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM2,ENABLE);		//ѡ����ȫ��ӳ��
	
	GPIO_InitStructure.GPIO_Pin  =GPIO_Pin_15;  	//GPIO_Pin_0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//PA15 ����  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);  //GPIOyyy
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
//	GPIO_ResetBits(GPIOA,GPIO_Pin_0);				

	ENCODER_TIM_PERIOD = arr;//�����������
	TIM_TimeBaseStructure.TIM_Period = arr; 		//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =0; 		//����������ΪTIM2ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 	//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIM2��ʱ�������λ
	
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ3
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 10;
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);		 //���TIM�ĸ��±�־λ
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update); //���TIM2�����жϱ�־����ֹ��ʱ��һ��ʼ���ͽ���һ��
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	TIM_SetCounter(TIM2,arr/2);//CNT����
	TIM_Cmd(TIM2, ENABLE); 
}

void TIM2_IRQHandler(void)   //TIM2�ж�
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) 	 //���ָ����TIM2�жϷ������:TIM2�ж�Դ 
	{
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  	 //���TIM2���жϴ�����λ:TIM2�ж�Դ 
		TIM2_Encoder_Full++;//����ۼӼ�������
	}
}
int Read_Encoder(TIM_TypeDef* TIMx) //���������������ȡ�����ڲ���ת��arr/2;
{
	int Encoder_TIM;    
//	Encoder_TIM= (TIMx -> CNT)+ENCODER_TIM_PERIOD *TIM2_Encoder_Full;  
//	TIM2_Encoder_Full=0;
	Encoder_TIM=(TIMx -> CNT)-ENCODER_TIM_PERIOD/2;
	TIMx -> CNT =ENCODER_TIM_PERIOD/2;	
	return Encoder_TIM;
}





/*
**PWM���
*/
//void TIMxxx_PWM_Init(u16 arr,u16 psc)
//{  
//	GPIO_InitTypeDef GPIO_InitStructure;
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	TIM_OCInitTypeDef  TIM_OCInitStructure;

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIMxxx, ENABLE);	//ʹ��TIMxxxʱ��
// 	RCC_APB2PeriphClockCmd( , ENABLE);  //ʹ��GPIOʱ��  RCC_APB2Periph_GPIOyyy
//	
////	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); 	//AFIO���ù���ģ��ʱ��
////	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE);		//ѡ����ȫ��ӳ��
//	
//	GPIO_InitStructure.GPIO_Pin  = ;  	//GPIO_Pin_0
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init( , &GPIO_InitStructure);  //GPIOyyy
////	GPIO_ResetBits(GPIOA,GPIO_Pin_0);				

//	TIM_TimeBaseStructure.TIM_Period = arr; 		//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
//	TIM_TimeBaseStructure.TIM_Prescaler =psc; 		//����������ΪTIMxxxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 	//����ʱ�ӷָ�:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
//	TIM_TimeBaseInit(TIMxxx, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMxxx��ʱ�������λ
//	
//	//��ʼ��TIMxxx Channel234 PWMģʽ	 
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 				//ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
// 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	//�Ƚ����ʹ��
//	TIM_OCInitStructure.TIM_Pulse = 0;
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 		//�������:TIM����Ƚϼ��Ը�
//	TIM_OC1Init(TIMxxx, &TIM_OCInitStructure);  						//����Tָ���Ĳ�����ʼ������TIM
//	TIM_OC2Init(TIMxxx, &TIM_OCInitStructure);
//	TIM_OC3Init(TIMxxx, &TIM_OCInitStructure);
//	TIM_OC4Init(TIMxxx, &TIM_OCInitStructure);
//	TIM_CtrlPWMOutputs(TIMxxx,ENABLE);
//	TIM_OC1PreloadConfig(TIMxxx, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
//	TIM_OC2PreloadConfig(TIMxxx, TIM_OCPreload_Enable);  
//	TIM_OC3PreloadConfig(TIMxxx, TIM_OCPreload_Enable);  
//	TIM_OC4PreloadConfig(TIMxxx, TIM_OCPreload_Enable); 
//	TIM_ARRPreloadConfig(TIMxxx, ENABLE);
//	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3
//}
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ʹ��TIM3ʱ��
 	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE);  //ʹ��GPIOʱ��  RCC_APB2Periph_GPIOyyy
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); 	//AFIO���ù���ģ��ʱ��
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE);		//ѡ����ȫ��ӳ��
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;  	//GPIO_Pin_0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOC, &GPIO_InitStructure);  //GPIOyyy
	
	//�����˳������789Ϊ�����ã�ʵ�ڷ�ǿ��֢һ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOC,GPIO_Pin_7);				
	GPIO_SetBits(GPIOC,GPIO_Pin_8);
	GPIO_SetBits(GPIOC,GPIO_Pin_9);
	
	TIM_TimeBaseStructure.TIM_Period = arr; 		//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 		//����������ΪTIM3ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 	//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIM3��ʱ�������λ
	
	//��ʼ��TIM3 Channel PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 				//ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	//�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 		//�������:TIM����Ƚϼ��Ը�
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  						//����Tָ���Ĳ�����ʼ������TIM

	TIM_CtrlPWMOutputs(TIM3,ENABLE);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���

	TIM_ARRPreloadConfig(TIM3, ENABLE);
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3
	TIM_SetCompare1(TIM3,0);
}




/*
**����Ƚϣ�Ƶ�ʡ�ռ�ձȿɱ䣬�򵱳�С��ʱ����ֱ�Ӳ�������IO�˿�
*/
//u16 CMP_ch1=100,CMP_ch2=200,CMP_ch3=300,CMP_ch4=400;
//void TIMxxx_CMP_Init(u16 psc)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	TIM_OCInitTypeDef  TIM_OCInitStructure;
//   	NVIC_InitTypeDef NVIC_InitStructure;

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIMxxx, ENABLE);	//ʹ��TIMxxxʱ��
// 	RCC_APB2PeriphClockCmd( , ENABLE);  //ʹ��GPIOʱ��  RCC_APB2Periph_GPIOyyy
//	
//	GPIO_InitStructure.GPIO_Pin  = ;  	//GPIO_Pin_0
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init( , &GPIO_InitStructure);  //GPIOyyy
////	GPIO_ResetBits(GPIOA,GPIO_Pin_0);				
//	
//	//��ʼ����ʱ��xxx
//	TIM_TimeBaseStructure.TIM_Period = 65535; 				//�趨�������Զ���װֵ 
//	TIM_TimeBaseStructure.TIM_Prescaler =psc; 				//Ԥ��Ƶ��   
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIMxxx���ϼ���ģʽ
//	TIM_TimeBaseInit(TIMxxx, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMxxx��ʱ�������λ
//  
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;  			//��תģʽ
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //ʹ�������ʹ��Ӧͨ�����ź�
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  		//��PWM��Ч�ź��йأ��˴�ֻ��ת������
//	TIM_OCInitStructure.TIM_Pulse = CMP_ch1; 			//ͨ��1�ıȽ�ֵ			
//	TIM_OC1Init(TIMxxx, &TIM_OCInitStructure);  			//ͨ��1��ʼ��
//	TIM_OC1PreloadConfig(TIMxxx, TIM_OCPreload_Disable);  //ʹ��Ԥװ�ؼĴ���  �˴�ʧ�ܣ�ͨ���ж�������һ���Ƚ�ֵ
// 
//	TIM_OCInitStructure.TIM_Pulse = CMP_ch2;  
//	TIM_OC2Init(TIMxxx, &TIM_OCInitStructure);  
//	TIM_OC2PreloadConfig(TIMxxx, TIM_OCPreload_Disable);  

//	TIM_OCInitStructure.TIM_Pulse = CMP_ch3;  
//	TIM_OC3Init(TIMxxx, &TIM_OCInitStructure);  
//	TIM_OC3PreloadConfig(TIMxxx, TIM_OCPreload_Disable);  

//	TIM_OCInitStructure.TIM_Pulse = CMP_chxxx;  
//	TIM_OC4Init(TIMxxx, &TIM_OCInitStructure);  
//	TIM_OC4PreloadConfig(TIMxxx, TIM_OCPreload_Disable);  
//	
//	TIM_ARRPreloadConfig(TIMxxx, ENABLE);		//Ӱ�ӼĴ�������
//	TIM_CtrlPWMOutputs(TIMxxx,ENABLE);		//��̫����  �˴�Ҫ��Ҫ���涼��
//	
//	//�жϷ����ʼ��
//	NVIC_InitStructure.NVIC_IRQChannel = TIMxxx_IRQn;  //TIM3�ж�
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
//	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
//	
//	TIM_ClearITPendingBit(TIMxxx,TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4); //����жϱ�־λ
//	TIM_ITConfig(TIMxxx,TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4,ENABLE);//����CC1\2\3\4IE�����ж�	
//	
//   	TIM_Cmd(TIMxxx,ENABLE ); 	//ʹ�ܶ�ʱ��xxx 
//}
//void TIMxxx_IRQHandler(void)
//{ 
//	if(TIM_GetITStatus(TIMxxx, TIM_IT_CC1) != RESET)//����1���������¼�
//	{	
//		TIM_SetCompare1(TIMxxx,TIM_GetCapture1(TIMxxx)+CMP_ch1);
//	}		
//	if(TIM_GetITStatus(TIMxxx, TIM_IT_CC2) != RESET)//����2���������¼�
//	{	
//		TIM_SetCompare2(TIMxxx,TIM_GetCapture2(TIMxxx)+CMP_ch2);
//	}
//	if(TIM_GetITStatus(TIMxxx, TIM_IT_CC3) != RESET)//����3���������¼�
//	{	
//		TIM_SetCompare3(TIMxxx,TIM_GetCapture3(TIMxxx)+CMP_ch3);
//	}
//	if(TIM_GetITStatus(TIMxxx, TIM_IT_CC4) != RESET)//����4���������¼�
//	{	
//		TIM_SetCompare4(TIMxxx,TIM_GetCapture4(TIMxxx)+CMP_ch4);
//	}
//	//�ǲ��Ƿֱ������һ�㣬���Ա������ж�Դͬʱ�������ж�©�������Ŀ��ܣ�
//    TIM_ClearITPendingBit(TIMxxx,TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4); //����жϱ�־λ
//}

u16 CMP_ch1=100,CMP_ch2=200,CMP_ch3=300,CMP_ch4=400;
void TIM4_CMP_Init(u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
   	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//ʹ��TIM4ʱ��
 	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);  //ʹ��GPIOʱ��  RCC_APB2Periph_GPIOyyy
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;  	//GPIO_Pin_0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOB, &GPIO_InitStructure);  //GPIOyyy
//	GPIO_ResetBits(GPIOA,GPIO_Pin_0);				
	
	//��ʼ����ʱ��4
	TIM_TimeBaseStructure.TIM_Period = 65535; 				//�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 				//Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM4���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIM4��ʱ�������λ
  
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;  			//��תģʽ
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //ʹ�������ʹ��Ӧͨ�����ź�
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  		//��PWM��Ч�ź��йأ��˴�ֻ��ת������
	TIM_OCInitStructure.TIM_Pulse = CMP_ch1; 			//ͨ��1�ıȽ�ֵ			
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  			//ͨ��1��ʼ��
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Disable);  //ʹ��Ԥװ�ؼĴ���  �˴�ʧ�ܣ�ͨ���ж�������һ���Ƚ�ֵ
 
	TIM_OCInitStructure.TIM_Pulse = CMP_ch2;  
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Disable);  

	TIM_OCInitStructure.TIM_Pulse = CMP_ch3;  
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Disable);  

	TIM_OCInitStructure.TIM_Pulse = CMP_ch4;  
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);  
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Disable);  
	
	TIM_ARRPreloadConfig(TIM4, ENABLE);		//Ӱ�ӼĴ�������
	TIM_CtrlPWMOutputs(TIM4,ENABLE);		//��̫����  �˴�Ҫ��Ҫ���涼��
	
	//�жϷ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
	
	TIM_ClearITPendingBit(TIM4,TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4); //����жϱ�־λ
	TIM_ITConfig(TIM4,TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4,ENABLE);//����CC1\2\3\4IE�����ж�	
	
   	TIM_Cmd(TIM4,ENABLE ); 	//ʹ�ܶ�ʱ��4 
}
void TIM4_IRQHandler(void)
{ 
	if(TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET)//����1���������¼�
	{	
		TIM_SetCompare1(TIM4,TIM_GetCapture1(TIM4)+CMP_ch1);
	}		
	if(TIM_GetITStatus(TIM4, TIM_IT_CC2) != RESET)//����2���������¼�
	{	
		TIM_SetCompare2(TIM4,TIM_GetCapture2(TIM4)+CMP_ch2);
	}
	if(TIM_GetITStatus(TIM4, TIM_IT_CC3) != RESET)//����3���������¼�
	{	
		TIM_SetCompare3(TIM4,TIM_GetCapture3(TIM4)+CMP_ch3);
	}
	if(TIM_GetITStatus(TIM4, TIM_IT_CC4) != RESET)//����4���������¼�
	{	
		TIM_SetCompare4(TIM4,TIM_GetCapture4(TIM4)+CMP_ch4);
	}
	//�ǲ��Ƿֱ������һ�㣬���Ա������ж�Դͬʱ�������ж�©�������Ŀ��ܣ�
    TIM_ClearITPendingBit(TIM4,TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4); //����жϱ�־λ
}



extern u8 uiMode;
u16 adc1Ch2Num, adc1Ch3Num;

void TIM5_IRQHandler(void)   //TIM5�ж�
{
	static int coun=0;
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET) 	 //���ָ����TIM5�жϷ������:TIM5�ж�Դ 
	{
		TouchScan();
		
		coun=Read_Encoder(TIM2);
		if(coun<0)
		{
			GUI_Text(0,220,"-",1,Black, White);			//��ʾ����
			coun=-coun;									//תΪ����
		}else 
			GUI_Text(0,220," ",1,Black, White);			//ȥ������
		GUI_Word(8, 220, 5, coun, 0, Black, White);		//��ʾ��������    
		
		adc1Ch2Num = Get_Adc_AvgNX(2);
		GUI_Word(90, 204, 4, adc1Ch2Num, 0, Black, White);
		adc1Ch3Num = Get_Adc_AvgNX(3);
		GUI_Word(90, 222, 4, adc1Ch3Num, 0, Black, White);
		if(coun > 800)
			coun = 800;
		motor.Cur = coun;
		PID();
		MotorOutLimit();
		MotorMove();
		GUI_Word(190, 204, 4, motor.Set, 0, Black, White);
		GUI_Word(190, 222, 4, motor.Out, 0, Black, White);
		
		//ͨ������1���͵���λ��
		ScopeOnComputer();
		if(uiMode) //���ν����ӡ����
			ScopeOnScreen2();
		TIM_ClearITPendingBit(TIM5,TIM_IT_Update);  	 //���TIM5���жϴ�����λ:TIM5�ж�Դ 
	}
}


