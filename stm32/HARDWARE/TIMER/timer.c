#include "timer.h"
#include "usart.h"
#include "adda.h"
#include "ili9320.h"
#include "scope.h"
#include "control.h"
#include "touch_spi2.h"

/*
**定时进入中断的功能  
**使用时设置优先级，查找xxx替换为2、3、4、5、6、7即可
**定时器1、8时钟是APB2下的，需要修改时钟使能函数；中断函数名字不同，需要修改
*/
//void TIMxxx_Int_Init(u16 arr,u16 psc)
//{
//    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIMxxx, ENABLE); //时钟使能

//	TIM_TimeBaseStructure.TIM_Period = arr;			//设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
//	TIM_TimeBaseStructure.TIM_Prescaler =psc;		//设置用来作为TIMxxx时钟频率除数的预分频值 
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 	//设置时钟分割:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
//	TIM_TimeBaseInit(TIMxxx, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMxxx的时间基数单位
// 
//	TIM_ClearITPendingBit(TIMxxx,TIM_IT_Update); //清除TIMxxx更新中断标志，防止定时器一初始化就进入一次
//	TIM_ITConfig(TIMxxx,TIM_IT_Update,ENABLE ); 	//使能指定的TIMxxx中断,允许更新中断
//	
//	NVIC_InitStructure.NVIC_IRQChannel = TIMxxx_IRQn;  //TIMxxx中断
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1 ;  //先占优先级
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1 ;  //从优先级
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
//	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

//	TIM_Cmd(TIMxxx, ENABLE);  //使能TIMxxx外设						 
//}

//void TIMxxx_IRQHandler(void)   //TIMxxx中断
//{
//	if (TIM_GetITStatus(TIMxxx, TIM_IT_Update) != RESET) 	 //检查指定的TIMxxx中断发生与否:TIMxxx中断源 
//	{
//		TIM_ClearITPendingBit(TIMxxx,TIM_IT_Update);  	 //清除TIMxxx的中断待处理位:TIMxxx中断源 
//		
//	}
//}
void TIM5_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr;			//设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc;		//设置用来作为TIM5时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 	//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIM5的时间基数单位
 
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update); //清除TIM5更新中断标志，防止定时器一初始化就进入一次
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE ); 	//使能指定的TIM5中断,允许更新中断
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM5中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1 ;  //先占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1 ;  //从优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM5, ENABLE);  //使能TIM5外设						 
}





/*
**输入捕获
**默认一次捕获不会超过溢出周期，否则需要在溢出中断在记录，叠加到输出结果上
*/
//int TIMxxxCH1val=0,TIMxxxCH2val=0,TIMxxxCH3val=0,TIMxxxCH4val=0;		//结果
//u8  TIMxxxCH1=0,TIMxxxCH2=0,TIMxxxCH3=0,TIMxxxCH4=0;					//输入捕获状态		    				
//u16 CAP_TIM_PERIOD;
//u16	TIMxxxCH1_Start,TIMxxxCH1_End,TIMxxxCH2_Start,TIMxxxCH2_End,TIMxxxCH3_Start,TIMxxxCH3_End,TIMxxxCH4_Start,TIMxxxCH4_End;	//输入捕获值
//void TIMxxx_Cap_Init(u16 arr,u16 psc)
//{	 
//	GPIO_InitTypeDef GPIO_InitStructure;
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	TIM_ICInitTypeDef  TIMxxx_ICInitStructure;
//   	NVIC_InitTypeDef NVIC_InitStructure;

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIMxxx, ENABLE);	//使能TIMxxx时钟
// 	RCC_APB2PeriphClockCmd( , ENABLE);  //使能GPIO时钟  RCC_APB2Periph_GPIOyyy
//	
////	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); 	//AFIO复用功能模块时钟
////	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE);		//选择完全重映射
//
//	GPIO_InitStructure.GPIO_Pin  = ;  	//GPIO_Pin_0
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//PA0 输入  
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init( , &GPIO_InitStructure);  //GPIOyyy
////	GPIO_ResetBits(GPIOA,GPIO_Pin_0);				
//	
//	//初始化定时器xxx
//	CAP_TIM_PERIOD=arr;
//	TIM_TimeBaseStructure.TIM_Period = arr; 				//设定计数器自动重装值 
//	TIM_TimeBaseStructure.TIM_Prescaler =psc; 				//预分频器   
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIMxxx向上计数模式
//	TIM_TimeBaseInit(TIMxxx, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMxxx的时间基数单位
//  
//	//初始化TIMxxx输入捕获参数
//	TIMxxx_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	选择输入端 IC1映射到TI1上
//  	TIMxxx_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
//  	TIMxxx_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
//  	TIMxxx_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
//  	TIMxxx_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
//  	TIM_ICInit(TIMxxx, &TIMxxx_ICInitStructure);
//	TIMxxx_ICInitStructure.TIM_Channel = TIM_Channel_2;
//  	TIM_ICInit(TIMxxx, &TIMxxx_ICInitStructure);
//	TIMxxx_ICInitStructure.TIM_Channel = TIM_Channel_3;
//  	TIM_ICInit(TIMxxx, &TIMxxx_ICInitStructure);
//	TIMxxx_ICInitStructure.TIM_Channel = TIM_Channel_4;
//  	TIM_ICInit(TIMxxx, &TIMxxx_ICInitStructure);
//	
//	//中断分组初始化
//	NVIC_InitStructure.NVIC_IRQChannel = TIMxxx_IRQn;  //TIMxxx中断
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
//	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
//	
//	TIM_ClearITPendingBit(TIMxxx,TIM_IT_Update|TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4); //清除中断标志位
//	TIM_ITConfig(TIMxxx,TIM_IT_Update|TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4,ENABLE);//允许更新中断 ,允许CC1\2\3\4IE捕获中断	
//	
//   	TIM_Cmd(TIMxxx,ENABLE ); 	//使能定时器xxx 
//}
//void TIMxxx_IRQHandler(void)
//{ 
//	if(TIM_GetITStatus(TIMxxx, TIM_IT_CC1) != RESET)//捕获1发生捕获事件
//	{	
//		if(TIMxxxCH1)		//捕获到一个下降沿 		
//		{	 
//			TIMxxxCH1=0;
//			TIMxxxCH1_End=TIM_GetCounter(TIMxxx);
//			TIMxxxCH1val=TIMxxxCH1_End-TIMxxxCH1_Start;
//			if(TIMxxxCH1val<0)	TIMxxxCH1val+=CAP_TIM_PERIOD;
//			//溢出处理 必要时自行添加
//			TIM_OC1PolarityConfig(TIMxxx,TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获
//		}
//		else  								//还未开始,第一次捕获上升沿
//		{
//			TIMxxxCH1=1;
//			TIMxxxCH1_Start=TIM_GetCounter(TIMxxx);
//			TIM_OC1PolarityConfig(TIMxxx,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获
//		}
//	}		
//	if(TIM_GetITStatus(TIMxxx, TIM_IT_CC2) != RESET)//捕获2发生捕获事件
//	{	
//		if(TIMxxxCH2)		//捕获到一个下降沿 		
//		{	 
//			TIMxxxCH2=0;
//			TIMxxxCH2_End=TIM_GetCounter(TIMxxx);
//			TIMxxxCH2val=TIMxxxCH2_End-TIMxxxCH2_Start;
//			//溢出处理 必要时自行添加
//			TIM_OC2PolarityConfig(TIMxxx,TIM_ICPolarity_Rising); //CC2P=0 设置为上升沿捕获
//		}
//		else  								//还未开始,第一次捕获上升沿
//		{
//			TIMxxxCH2=1;
//			TIMxxxCH2_Start=TIM_GetCounter(TIMxxx);
//			TIM_OC2PolarityConfig(TIMxxx,TIM_ICPolarity_Falling);		//CC2P=1 设置为下降沿捕获
//		}
//	}
//	if(TIM_GetITStatus(TIMxxx, TIM_IT_CC3) != RESET)//捕获3发生捕获事件
//	{	
//		if(TIMxxxCH3)		//捕获到一个下降沿 		
//		{	 
//			TIMxxxCH3=0;
//			TIMxxxCH3_End=TIM_GetCounter(TIMxxx);
//			TIMxxxCH3val=TIMxxxCH3_End-TIMxxxCH3_Start;
//			//溢出处理 必要时自行添加
//			TIM_OC3PolarityConfig(TIMxxx,TIM_ICPolarity_Rising); //CC3P=0 设置为上升沿捕获
//		}
//		else  								//还未开始,第一次捕获上升沿
//		{
//			TIMxxxCH3=1;
//			TIMxxxCH3_Start=TIM_GetCounter(TIMxxx);
//			TIM_OC3PolarityConfig(TIMxxx,TIM_ICPolarity_Falling);		//CC3P=1 设置为下降沿捕获
//		}
//	}
//	if(TIM_GetITStatus(TIMxxx, TIM_IT_CC4) != RESET)//捕获4发生捕获事件
//	{	
//		if(TIMxxxCH4)		//捕获到一个下降沿 		
//		{	 
//			TIMxxxCH4=0;
//			TIMxxxCH4_End=TIM_GetCounter(TIMxxx);
//			TIMxxxCH4val=TIMxxxCH4_End-TIMxxxCH4_Start;
//			//溢出处理 必要时自行添加
//			TIM_OC4PolarityConfig(TIMxxx,TIM_ICPolarity_Rising); //CC4P=0 设置为上升沿捕获
//		}
//		else  								//还未开始,第一次捕获上升沿
//		{
//			TIMxxxCH4=1;
//			TIMxxxCH4_Start=TIM_GetCounter(TIMxxx);
//			TIM_OC4PolarityConfig(TIMxxx,TIM_ICPolarity_Falling);		//CC4P=1 设置为下降沿捕获
//		}
//	}
//	//是不是分别清零好一点，可以避免多个中断源同时发生了中断漏掉其它的可能？
//    TIM_ClearITPendingBit(TIMxxx,TIM_IT_Update|TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4); //清除中断标志位
//}
int TIM3CH1val=0,TIM3CH2val=0,TIM3CH3val=0,TIM3CH4val=0;		//结果
u8  TIM3CH1=0,TIM3CH2=0,TIM3CH3=0,TIM3CH4=0;					//输入捕获状态		    				
u16 CAP_TIM_PERIOD;
u16	TIM3CH1_Start,TIM3CH1_End,TIM3CH2_Start,TIM3CH2_End,TIM3CH3_Start,TIM3CH3_End,TIM3CH4_Start,TIM3CH4_End;	//输入捕获值
void TIM3_Cap_Init(u16 arr,u16 psc)
{	 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM3_ICInitStructure;
   	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//使能TIM3时钟
 	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE);  //使能GPIO时钟  RCC_APB2Periph_GPIOyyy
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); 	//AFIO复用功能模块时钟
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE);		//选择完全重映射

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;  	//GPIO_Pin_0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//PA0 输入  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOC, &GPIO_InitStructure);  //GPIOyyy
//	GPIO_ResetBits(GPIOA,GPIO_Pin_0);				
	
	//初始化定时器3
	CAP_TIM_PERIOD=arr;
	TIM_TimeBaseStructure.TIM_Period = arr; 				//设定计数器自动重装值 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 				//预分频器   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM3向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIM3的时间基数单位
  
	//初始化TIM3输入捕获参数
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	选择输入端 IC1映射到TI1上
  	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
  	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
  	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  	TIM3_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
  	TIM_ICInit(TIM3, &TIM3_ICInitStructure);
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_2;
  	TIM_ICInit(TIM3, &TIM3_ICInitStructure);
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_3;
  	TIM_ICInit(TIM3, &TIM3_ICInitStructure);
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_4;
  	TIM_ICInit(TIM3, &TIM3_ICInitStructure);
	
	//中断分组初始化
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
	
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update|TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4); //清除中断标志位
	TIM_ITConfig(TIM3,TIM_IT_Update|TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4,ENABLE);//允许更新中断 ,允许CC1\2\3\4IE捕获中断	
	
   	TIM_Cmd(TIM3,ENABLE ); 	//使能定时器3 
}
void TIM3_IRQHandler(void)
{ 
	if(TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)//捕获1发生捕获事件
	{	
		TIM3CH1val++;
		if(TIM3CH1)		//捕获到一个下降沿 		
		{	 
			TIM3CH1=0;
			TIM3CH1_End=TIM_GetCounter(TIM3);
			TIM3CH1val=TIM3CH1_End-TIM3CH1_Start;
			if(TIM3CH1val<0)	TIM3CH1val+=CAP_TIM_PERIOD;
			//溢出处理 必要时自行添加
			TIM_OC1PolarityConfig(TIM3,TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获
		}
		else  								//还未开始,第一次捕获上升沿
		{
			TIM3CH1=1;
			TIM3CH1_Start=TIM_GetCounter(TIM3);
			TIM_OC1PolarityConfig(TIM3,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获
		}
	}		
	if(TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET)//捕获2发生捕获事件
	{	
		if(TIM3CH2)		//捕获到一个下降沿 		
		{	 
			TIM3CH2=0;
			TIM3CH2_End=TIM_GetCounter(TIM3);
			TIM3CH2val=TIM3CH2_End-TIM3CH2_Start;
			//溢出处理 必要时自行添加
			TIM_OC2PolarityConfig(TIM3,TIM_ICPolarity_Rising); //CC2P=0 设置为上升沿捕获
		}
		else  								//还未开始,第一次捕获上升沿
		{
			TIM3CH2=1;
			TIM3CH2_Start=TIM_GetCounter(TIM3);
			TIM_OC2PolarityConfig(TIM3,TIM_ICPolarity_Falling);		//CC2P=1 设置为下降沿捕获
		}
	}
	if(TIM_GetITStatus(TIM3, TIM_IT_CC3) != RESET)//捕获3发生捕获事件
	{	
		if(TIM3CH3)		//捕获到一个下降沿 		
		{	 
			TIM3CH3=0;
			TIM3CH3_End=TIM_GetCounter(TIM3);
			TIM3CH3val=TIM3CH3_End-TIM3CH3_Start;
			//溢出处理 必要时自行添加
			TIM_OC3PolarityConfig(TIM3,TIM_ICPolarity_Rising); //CC3P=0 设置为上升沿捕获
		}
		else  								//还未开始,第一次捕获上升沿
		{
			TIM3CH3=1;
			TIM3CH3_Start=TIM_GetCounter(TIM3);
			TIM_OC3PolarityConfig(TIM3,TIM_ICPolarity_Falling);		//CC3P=1 设置为下降沿捕获
		}
	}
	if(TIM_GetITStatus(TIM3, TIM_IT_CC4) != RESET)//捕获4发生捕获事件
	{	
		if(TIM3CH4)		//捕获到一个下降沿 		
		{	 
			TIM3CH4=0;
			TIM3CH4_End=TIM_GetCounter(TIM3);
			TIM3CH4val=TIM3CH4_End-TIM3CH4_Start;
			//溢出处理 必要时自行添加
			TIM_OC4PolarityConfig(TIM3,TIM_ICPolarity_Rising); //CC4P=0 设置为上升沿捕获
		}
		else  								//还未开始,第一次捕获上升沿
		{
			TIM3CH4=1;
			TIM3CH4_Start=TIM_GetCounter(TIM3);
			TIM_OC4PolarityConfig(TIM3,TIM_ICPolarity_Falling);		//CC4P=1 设置为下降沿捕获
		}
	}
	//是不是分别清零好一点，可以避免多个中断源同时发生了中断漏掉其它的可能？
    TIM_ClearITPendingBit(TIM3,TIM_IT_Update|TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4); //清除中断标志位
}




/*
**编码器模式
*/
//u16 TIMxxx_Encoder_Full;	//编码器计数溢出记录
//u16 ENCODER_TIM_PERIOD;
//void TIMxxx_Encoder_Init(u16 arr)
//{
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
//	TIM_ICInitTypeDef TIM_ICInitStructure;  
//	GPIO_InitTypeDef GPIO_InitStructure;
//	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIMxxx, ENABLE);	//使能TIMxxx时钟
// 	RCC_APB2PeriphClockCmd( , ENABLE);  //使能GPIO时钟  RCC_APB2Periph_GPIOyyy
//	
////	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); 	//AFIO复用功能模块时钟
////	GPIO_PinRemapConfig(GPIO_FullRemap_TIMxxx,ENABLE);		//选择完全重映射
//	
//	GPIO_InitStructure.GPIO_Pin  = ;  	//GPIO_Pin_0
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//PA0 输入  
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init( , &GPIO_InitStructure);  //GPIOyyy
////	GPIO_ResetBits(GPIOA,GPIO_Pin_0);				

//	ENCODER_TIM_PERIOD = arr;//用于溢出计算
//	TIM_TimeBaseStructure.TIM_Period = arr; 		//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
//	TIM_TimeBaseStructure.TIM_Prescaler =0; 		//设置用来作为TIMxxx时钟频率除数的预分频值 
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 	//设置时钟分割:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
//	TIM_TimeBaseInit(TIMxxx, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMxxx的时间基数单位
//	
//	TIM_EncoderInterfaceConfig(TIMxxx, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3
//	TIM_ICStructInit(&TIM_ICInitStructure);
//	TIM_ICInitStructure.TIM_ICFilter = 10;
//	TIM_ICInit(TIMxxx, &TIM_ICInitStructure);
//	
//	TIM_ClearFlag(TIMxxx, TIM_FLAG_Update);		 //清除TIM的更新标志位
//	TIM_ClearITPendingBit(TIMxxx,TIM_IT_Update); //清除TIMxxx更新中断标志，防止定时器一初始化就进入一次
//	TIM_ITConfig(TIMxxx, TIM_IT_Update, ENABLE);

//	TIM_SetCounter(TIMxxx,arr/2);//CNT置中
//	TIM_Cmd(TIMxxx, ENABLE); 
//}

//void TIMxxx_IRQHandler(void)   //TIMxxx中断
//{
//	if (TIM_GetITStatus(TIMxxx, TIM_IT_Update) != RESET) 	 //检查指定的TIMxxx中断发生与否:TIMxxx中断源 
//	{
//		TIM_ClearITPendingBit(TIMxxx,TIM_IT_Update);  	 //清除TIMxxx的中断待处理位:TIMxxx中断源 
//		TIMxxx_Encoder_Full++;//溢出累加计数处理
//	}
//}
//int Read_Encoder(TIM_TypeDef* TIMx) //不考虑溢出，即读取周期内不会转过arr/2;
//{
//	int Encoder_TIM;    
////	Encoder_TIM= (TIMx -> CNT)+ENCODER_TIM_PERIOD *TIMxxx_Encoder_Full;  
////	TIMxxx_Encoder_Full=0;
//	Encoder_TIM=(TIMx -> CNT)-ENCODER_TIM_PERIOD/2;
//	TIMx -> CNT =ENCODER_TIM_PERIOD/2;	
//	return Encoder_TIM;
//}
u16 TIM2_Encoder_Full;	//编码器计数溢出记录
u16 ENCODER_TIM_PERIOD;
void TIM2_Encoder_Init(u16 arr)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
	TIM_ICInitTypeDef TIM_ICInitStructure;  
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//使能TIM2时钟
 	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);  //使能GPIO时钟  RCC_APB2Periph_GPIOyyy
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); 	//AFIO复用功能模块时钟
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM2,ENABLE);		//选择完全重映射
	
	GPIO_InitStructure.GPIO_Pin  =GPIO_Pin_15;  	//GPIO_Pin_0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//PA15 输入  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);  //GPIOyyy
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
//	GPIO_ResetBits(GPIOA,GPIO_Pin_0);				

	ENCODER_TIM_PERIOD = arr;//用于溢出计算
	TIM_TimeBaseStructure.TIM_Period = arr; 		//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =0; 		//设置用来作为TIM2时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 	//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIM2的时间基数单位
	
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 10;
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);		 //清除TIM的更新标志位
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update); //清除TIM2更新中断标志，防止定时器一初始化就进入一次
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	TIM_SetCounter(TIM2,arr/2);//CNT置中
	TIM_Cmd(TIM2, ENABLE); 
}

void TIM2_IRQHandler(void)   //TIM2中断
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) 	 //检查指定的TIM2中断发生与否:TIM2中断源 
	{
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  	 //清除TIM2的中断待处理位:TIM2中断源 
		TIM2_Encoder_Full++;//溢出累加计数处理
	}
}
int Read_Encoder(TIM_TypeDef* TIMx) //不考虑溢出，即读取周期内不会转过arr/2;
{
	int Encoder_TIM;    
//	Encoder_TIM= (TIMx -> CNT)+ENCODER_TIM_PERIOD *TIM2_Encoder_Full;  
//	TIM2_Encoder_Full=0;
	Encoder_TIM=(TIMx -> CNT)-ENCODER_TIM_PERIOD/2;
	TIMx -> CNT =ENCODER_TIM_PERIOD/2;	
	return Encoder_TIM;
}





/*
**PWM输出
*/
//void TIMxxx_PWM_Init(u16 arr,u16 psc)
//{  
//	GPIO_InitTypeDef GPIO_InitStructure;
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	TIM_OCInitTypeDef  TIM_OCInitStructure;

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIMxxx, ENABLE);	//使能TIMxxx时钟
// 	RCC_APB2PeriphClockCmd( , ENABLE);  //使能GPIO时钟  RCC_APB2Periph_GPIOyyy
//	
////	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); 	//AFIO复用功能模块时钟
////	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE);		//选择完全重映射
//	
//	GPIO_InitStructure.GPIO_Pin  = ;  	//GPIO_Pin_0
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init( , &GPIO_InitStructure);  //GPIOyyy
////	GPIO_ResetBits(GPIOA,GPIO_Pin_0);				

//	TIM_TimeBaseStructure.TIM_Period = arr; 		//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
//	TIM_TimeBaseStructure.TIM_Prescaler =psc; 		//设置用来作为TIMxxx时钟频率除数的预分频值 
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 	//设置时钟分割:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
//	TIM_TimeBaseInit(TIMxxx, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMxxx的时间基数单位
//	
//	//初始化TIMxxx Channel234 PWM模式	 
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 				//选择定时器模式:TIM脉冲宽度调制模式2
// 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	//比较输出使能
//	TIM_OCInitStructure.TIM_Pulse = 0;
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 		//输出极性:TIM输出比较极性高
//	TIM_OC1Init(TIMxxx, &TIM_OCInitStructure);  						//根据T指定的参数初始化外设TIM
//	TIM_OC2Init(TIMxxx, &TIM_OCInitStructure);
//	TIM_OC3Init(TIMxxx, &TIM_OCInitStructure);
//	TIM_OC4Init(TIMxxx, &TIM_OCInitStructure);
//	TIM_CtrlPWMOutputs(TIMxxx,ENABLE);
//	TIM_OC1PreloadConfig(TIMxxx, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
//	TIM_OC2PreloadConfig(TIMxxx, TIM_OCPreload_Enable);  
//	TIM_OC3PreloadConfig(TIMxxx, TIM_OCPreload_Enable);  
//	TIM_OC4PreloadConfig(TIMxxx, TIM_OCPreload_Enable); 
//	TIM_ARRPreloadConfig(TIMxxx, ENABLE);
//	TIM_Cmd(TIM3, ENABLE);  //使能TIM3
//}
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//使能TIM3时钟
 	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE);  //使能GPIO时钟  RCC_APB2Periph_GPIOyyy
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); 	//AFIO复用功能模块时钟
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE);		//选择完全重映射
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;  	//GPIO_Pin_0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOC, &GPIO_InitStructure);  //GPIOyyy
	
	//在这儿顺便设置789为我所用，实在非强迫症一贯作风
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOC,GPIO_Pin_7);				
	GPIO_SetBits(GPIOC,GPIO_Pin_8);
	GPIO_SetBits(GPIOC,GPIO_Pin_9);
	
	TIM_TimeBaseStructure.TIM_Period = arr; 		//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 		//设置用来作为TIM3时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 	//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIM3的时间基数单位
	
	//初始化TIM3 Channel PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 				//选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	//比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 		//输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  						//根据T指定的参数初始化外设TIM

	TIM_CtrlPWMOutputs(TIM3,ENABLE);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器

	TIM_ARRPreloadConfig(TIM3, ENABLE);
	TIM_Cmd(TIM3, ENABLE);  //使能TIM3
	TIM_SetCompare1(TIM3,0);
}




/*
**输出比较，频率、占空比可变，或当成小定时器，直接操作其它IO端口
*/
//u16 CMP_ch1=100,CMP_ch2=200,CMP_ch3=300,CMP_ch4=400;
//void TIMxxx_CMP_Init(u16 psc)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	TIM_OCInitTypeDef  TIM_OCInitStructure;
//   	NVIC_InitTypeDef NVIC_InitStructure;

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIMxxx, ENABLE);	//使能TIMxxx时钟
// 	RCC_APB2PeriphClockCmd( , ENABLE);  //使能GPIO时钟  RCC_APB2Periph_GPIOyyy
//	
//	GPIO_InitStructure.GPIO_Pin  = ;  	//GPIO_Pin_0
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init( , &GPIO_InitStructure);  //GPIOyyy
////	GPIO_ResetBits(GPIOA,GPIO_Pin_0);				
//	
//	//初始化定时器xxx
//	TIM_TimeBaseStructure.TIM_Period = 65535; 				//设定计数器自动重装值 
//	TIM_TimeBaseStructure.TIM_Prescaler =psc; 				//预分频器   
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIMxxx向上计数模式
//	TIM_TimeBaseInit(TIMxxx, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMxxx的时间基数单位
//  
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;  			//翻转模式
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //使能输出，使对应通道有信号
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  		//与PWM有效信号有关，此处只翻转，不管
//	TIM_OCInitStructure.TIM_Pulse = CMP_ch1; 			//通道1的比较值			
//	TIM_OC1Init(TIMxxx, &TIM_OCInitStructure);  			//通道1初始化
//	TIM_OC1PreloadConfig(TIMxxx, TIM_OCPreload_Disable);  //使能预装载寄存器  此处失能，通过中断设置下一个比较值
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
//	TIM_ARRPreloadConfig(TIMxxx, ENABLE);		//影子寄存器设置
//	TIM_CtrlPWMOutputs(TIMxxx,ENABLE);		//不太明白  此处要不要表面都行
//	
//	//中断分组初始化
//	NVIC_InitStructure.NVIC_IRQChannel = TIMxxx_IRQn;  //TIM3中断
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
//	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
//	
//	TIM_ClearITPendingBit(TIMxxx,TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4); //清除中断标志位
//	TIM_ITConfig(TIMxxx,TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4,ENABLE);//允许CC1\2\3\4IE捕获中断	
//	
//   	TIM_Cmd(TIMxxx,ENABLE ); 	//使能定时器xxx 
//}
//void TIMxxx_IRQHandler(void)
//{ 
//	if(TIM_GetITStatus(TIMxxx, TIM_IT_CC1) != RESET)//捕获1发生捕获事件
//	{	
//		TIM_SetCompare1(TIMxxx,TIM_GetCapture1(TIMxxx)+CMP_ch1);
//	}		
//	if(TIM_GetITStatus(TIMxxx, TIM_IT_CC2) != RESET)//捕获2发生捕获事件
//	{	
//		TIM_SetCompare2(TIMxxx,TIM_GetCapture2(TIMxxx)+CMP_ch2);
//	}
//	if(TIM_GetITStatus(TIMxxx, TIM_IT_CC3) != RESET)//捕获3发生捕获事件
//	{	
//		TIM_SetCompare3(TIMxxx,TIM_GetCapture3(TIMxxx)+CMP_ch3);
//	}
//	if(TIM_GetITStatus(TIMxxx, TIM_IT_CC4) != RESET)//捕获4发生捕获事件
//	{	
//		TIM_SetCompare4(TIMxxx,TIM_GetCapture4(TIMxxx)+CMP_ch4);
//	}
//	//是不是分别清零好一点，可以避免多个中断源同时发生了中断漏掉其它的可能？
//    TIM_ClearITPendingBit(TIMxxx,TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4); //清除中断标志位
//}

u16 CMP_ch1=100,CMP_ch2=200,CMP_ch3=300,CMP_ch4=400;
void TIM4_CMP_Init(u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
   	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//使能TIM4时钟
 	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);  //使能GPIO时钟  RCC_APB2Periph_GPIOyyy
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;  	//GPIO_Pin_0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOB, &GPIO_InitStructure);  //GPIOyyy
//	GPIO_ResetBits(GPIOA,GPIO_Pin_0);				
	
	//初始化定时器4
	TIM_TimeBaseStructure.TIM_Period = 65535; 				//设定计数器自动重装值 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 				//预分频器   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM4向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIM4的时间基数单位
  
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;  			//翻转模式
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //使能输出，使对应通道有信号
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  		//与PWM有效信号有关，此处只翻转，不管
	TIM_OCInitStructure.TIM_Pulse = CMP_ch1; 			//通道1的比较值			
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  			//通道1初始化
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Disable);  //使能预装载寄存器  此处失能，通过中断设置下一个比较值
 
	TIM_OCInitStructure.TIM_Pulse = CMP_ch2;  
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Disable);  

	TIM_OCInitStructure.TIM_Pulse = CMP_ch3;  
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Disable);  

	TIM_OCInitStructure.TIM_Pulse = CMP_ch4;  
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);  
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Disable);  
	
	TIM_ARRPreloadConfig(TIM4, ENABLE);		//影子寄存器设置
	TIM_CtrlPWMOutputs(TIM4,ENABLE);		//不太明白  此处要不要表面都行
	
	//中断分组初始化
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
	
	TIM_ClearITPendingBit(TIM4,TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4); //清除中断标志位
	TIM_ITConfig(TIM4,TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4,ENABLE);//允许CC1\2\3\4IE捕获中断	
	
   	TIM_Cmd(TIM4,ENABLE ); 	//使能定时器4 
}
void TIM4_IRQHandler(void)
{ 
	if(TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET)//捕获1发生捕获事件
	{	
		TIM_SetCompare1(TIM4,TIM_GetCapture1(TIM4)+CMP_ch1);
	}		
	if(TIM_GetITStatus(TIM4, TIM_IT_CC2) != RESET)//捕获2发生捕获事件
	{	
		TIM_SetCompare2(TIM4,TIM_GetCapture2(TIM4)+CMP_ch2);
	}
	if(TIM_GetITStatus(TIM4, TIM_IT_CC3) != RESET)//捕获3发生捕获事件
	{	
		TIM_SetCompare3(TIM4,TIM_GetCapture3(TIM4)+CMP_ch3);
	}
	if(TIM_GetITStatus(TIM4, TIM_IT_CC4) != RESET)//捕获4发生捕获事件
	{	
		TIM_SetCompare4(TIM4,TIM_GetCapture4(TIM4)+CMP_ch4);
	}
	//是不是分别清零好一点，可以避免多个中断源同时发生了中断漏掉其它的可能？
    TIM_ClearITPendingBit(TIM4,TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4); //清除中断标志位
}



extern u8 uiMode;
u16 adc1Ch2Num, adc1Ch3Num;

void TIM5_IRQHandler(void)   //TIM5中断
{
	static int coun=0;
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET) 	 //检查指定的TIM5中断发生与否:TIM5中断源 
	{
		TouchScan();
		
		coun=Read_Encoder(TIM2);
		if(coun<0)
		{
			GUI_Text(0,220,"-",1,Black, White);			//显示负号
			coun=-coun;									//转为正数
		}else 
			GUI_Text(0,220," ",1,Black, White);			//去掉负号
		GUI_Word(8, 220, 5, coun, 0, Black, White);		//显示整数部分    
		
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
		
		//通过串口1发送到上位机
		ScopeOnComputer();
		if(uiMode) //波形界面打印波形
			ScopeOnScreen2();
		TIM_ClearITPendingBit(TIM5,TIM_IT_Update);  	 //清除TIM5的中断待处理位:TIM5中断源 
	}
}


