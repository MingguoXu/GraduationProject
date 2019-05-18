#ifndef __timer_h
#define __timer_h

#include "sys.h"

/*
**定时器说明
时钟：定时器234567在APB1下，18在APB2下
通道：定时器5和8没有重映射，定时器2有两个部分重映射，具体对应参见原子中文开发手册第八章重映像
定时器1：
默认：TIM1_CH1->PA8; TIM1_CH2->PA9;  TIM1_CH3->PA10; TIM1_CH4->PA11; 
完全：TIM1_CH1->PE9; TIM1_CH2->PE11; TIM1_CH3->PE13; TIM1_CH4->PE14;

定时器8：
默认：TIM8_CH1->PC6; TIM8_CH2->PC7; TIM8_CH3->PC8; TIM8_CH4->PC9; 

定时器2：
默认：TIM2_CH1->PA0;  TIM2_CH2->PA1; TIM2_CH3->PA2; TIM2_CH4->PA3; 
部分1 TIM2_CH1->PA15; TIM2_CH2->PB3; TIM2_CH3->PA2; TIM2_CH4->PA3; 
部分2 TIM2_CH1->PA0;  TIM2_CH2->PA1; TIM2_CH3->PB10; TIM2_CH4->PB11; 
完全：TIM2_CH1->PA15; TIM2_CH2->PB3; TIM2_CH3->PB10; TIM2_CH4->PB11; 

定时器3：
默认：TIM3_CH1->PA6; TIM3_CH2->PA7; TIM3_CH3->PB0; TIM3_CH4->PB1; 
部分：TIM3_CH1->PB4; TIM3_CH2->PB5; TIM3_CH3->PB0; TIM3_CH4->PB1; 
完全：TIM3_CH1->PC6; TIM3_CH2->PC7; TIM3_CH3->PC8; TIM3_CH4->PC9; 

定时器4：
默认：TIM4_CH1->PB6;  TIM4_CH1->PB7;  TIM4_CH1->PB8;  TIM4_CH1->PB9; 
映射：TIM4_CH1->PD12; TIM4_CH1->PD13; TIM4_CH1->PD14; TIM4_CH1->PD15; //直接Remap

定时器5：
默认：TIM5_CH1->PA0;  TIM5_CH2->PA1; TIM5_CH3->PA2; TIM5_CH4->PA3; 
*/

void TIMxxx_Int_Init(u16 arr,u16 psc);	//基本定时功能

void TIMxxx_Cap_Init(u16 arr,u16 psc);	//输入捕获
void TIMxxx_Encoder_Init(u16 arr);		//编码器输入
int  Read_Encoder(TIM_TypeDef* TIMx);

void TIMxxx_PWM_Init(u16 arr,u16 psc);	//PWM输出
void TIMxxx_CMP_Init(u16 psc);			//输出比较
/*设置溢出值为65535，每次比较中断重新设置比较值		
	TIM_SetCompare1(TIM3, capture + TIM_GetCapture1(TIM3));//在原来的CCR1（即capture）基础上加5000，则再过5000，又会触发中断  
	//另外，有个问题，进入中断时，当ARR计数器快加到65535，而又不足5000时，不是会有数据多余，而产生波形的移动吗？  
    //回答：不用担心。例如进入中断是，ARR=65000，65000+5000=70000>65535，那么高位会舍去，即为70000-65536=4464  
	//等于是来了个循环，两次中断间ARR的增量还是5000。所以为了波形的稳定，初始化时，必须要有TIM_Period = 65535  
*/
void TIM5_Int_Init(u16 arr,u16 psc);
void TIM3_Cap_Init(u16 arr,u16 psc);
void TIM2_Encoder_Init(u16 arr);
void TIM3_PWM_Init(u16 arr,u16 psc);
void TIM4_CMP_Init(u16 psc);

#endif

//补充：对于C8芯片 一高级加三通用 无重映射 
/*
TIM1_CH1->PA8; TIM1_CH2->PA9; TIM1_CH3->PA10; TIM1_CH4->PA11; 
TIM2_CH1->PA0; TIM2_CH2->PA1; TIM2_CH3->PA2; TIM2_CH4->PA3; 
TIM3_CH1->PA6; TIM3_CH2->PA7; TIM3_CH3->PB0; TIM3_CH4->PB1; 
TIM4_CH1->PB6; TIM4_CH1->PB7; TIM4_CH1->PB8; TIM4_CH1->PB9; 
*/

