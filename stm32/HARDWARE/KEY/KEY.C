#include "key.h"
#include "delay.h"

/*************************
* 独立按键端口初始化函数
*************************/
void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOE, ENABLE); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 			//按键key1端口配置
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//IO口速度为50MHz
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU;		//内部上拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; 			//按键key2端口配置
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/*************************
* 独立按键扫描函数
* mode选择是否支持连按
* 返回对应按键按下情况
*************************/
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//松开标志
	if(mode)
	{
		delay_ms(100);
		key_up=1;  //支持连按
	}		
	if(key_up&&(KEY1==0||KEY2==0))
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(KEY1==0)return KEY1_PRES;
		else if(KEY2==0)return KEY2_PRES;
	}
	else if(KEY1==1&&KEY2==1)
		key_up=1; 	    
 	return 0;// 无按键按下
}

/*************************
* 独立按键中断模式初始化函数
*************************/
void KEY_INT_Init(void)
{
	EXTI_InitTypeDef  EXTI_InitStructure;					//定义一个EXTI结构体变量
	NVIC_InitTypeDef  NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);		//使能IO复用功能，使用中断功能重要！！！

  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource13);//配置端口C的13引脚为中断源	  重要！！ 板上标号INT2
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource0); //配置端口E的0引脚为中断源	  重要！！ 板上标号INT1
	EXTI_InitStructure.EXTI_Line = EXTI_Line0 | EXTI_Line13;	//
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//中断模式为中断模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		//下降沿出发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;					//使能中断线
	EXTI_Init(&EXTI_InitStructure);								//根据参数初始化中断寄存器
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;		//设定中断源为PC13
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//中断占优先级为2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			//副优先级为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//使能中断
	NVIC_Init(&NVIC_InitStructure);							   	//根据参数初始化中断寄存器

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//设定中断源为PE0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//中断占优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;			//副优先级为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//使能中断
	NVIC_Init(&NVIC_InitStructure);							   	//根据参数初始化中断寄存器
}

/*************************
* 矩阵按键IO初始化函数
*************************/
void KeyPlus_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;	//行引脚作为输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;							//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;	//列引脚作为输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;								//上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure); 	
}

/*************************
* 矩阵按键扫描函数
*************************/
u8 KeyPlus_Scan(void)
{	
	PFout(1)=0;PFout(2)=1;PFout(3)=1;PFout(4)=1;//检测第一行
	if(!PFin(5)||!PFin(6)||!PFin(7)||!PFin(8))
	{
		delay_ms(10);
		if(!PFin(5))	return 16;
		if(!PFin(6))	return 15;
		if(!PFin(7))	return 14;
		if(!PFin(8))	return 13;
	}
	PFout(1)=1;PFout(2)=0;PFout(3)=1;PFout(4)=1;//检测第二行
	if(!PFin(5)||!PFin(6)||!PFin(7)||!PFin(8))
	{
		delay_ms(10);
		if(!PFin(5))	return 12;
		if(!PFin(6))	return 11;
		if(!PFin(7))	return 10;
		if(!PFin(8))	return 9;
	}
	PFout(1)=1;PFout(2)=1;PFout(3)=0;PFout(4)=1;//检测第三行
	if(!PFin(5)||!PFin(6)||!PFin(7)||!PFin(8))
	{
		delay_ms(10);
		if(!PFin(5))	return 8;
		if(!PFin(6))	return 7;
		if(!PFin(7))	return 6;
		if(!PFin(8))	return 5;
	}
	PFout(1)=1;PFout(2)=1;PFout(3)=1;PFout(4)=0;//检测第四行
	if(!PFin(5)||!PFin(6)||!PFin(7)||!PFin(8))
	{
		delay_ms(10);
		if(!PFin(5))	return 4;
		if(!PFin(6))	return 3;
		if(!PFin(7))	return 2;
		if(!PFin(8))	return 1;
	}
	return 0;
}

/*
Key2  中断15_10入口函数			
*/
void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line13)!= RESET)  	//判断是否发生中断，发生则中断标志置1
	{

	}
	EXTI_ClearITPendingBit(EXTI_Line13);		//清楚中断挂起位，重要！！			
}
/*
Key1 中断0入口函数			
*/
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0)!= RESET)	 //判断是否发生中断，发生则中断标志置1
	{

	}
	EXTI_ClearITPendingBit(EXTI_Line0);		     //清楚中断挂起位，重要！！			
}
