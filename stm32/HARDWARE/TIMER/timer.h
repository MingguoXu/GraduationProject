#ifndef __timer_h
#define __timer_h

#include "sys.h"

/*
**��ʱ��˵��
ʱ�ӣ���ʱ��234567��APB1�£�18��APB2��
ͨ������ʱ��5��8û����ӳ�䣬��ʱ��2������������ӳ�䣬�����Ӧ�μ�ԭ�����Ŀ����ֲ�ڰ�����ӳ��
��ʱ��1��
Ĭ�ϣ�TIM1_CH1->PA8; TIM1_CH2->PA9;  TIM1_CH3->PA10; TIM1_CH4->PA11; 
��ȫ��TIM1_CH1->PE9; TIM1_CH2->PE11; TIM1_CH3->PE13; TIM1_CH4->PE14;

��ʱ��8��
Ĭ�ϣ�TIM8_CH1->PC6; TIM8_CH2->PC7; TIM8_CH3->PC8; TIM8_CH4->PC9; 

��ʱ��2��
Ĭ�ϣ�TIM2_CH1->PA0;  TIM2_CH2->PA1; TIM2_CH3->PA2; TIM2_CH4->PA3; 
����1 TIM2_CH1->PA15; TIM2_CH2->PB3; TIM2_CH3->PA2; TIM2_CH4->PA3; 
����2 TIM2_CH1->PA0;  TIM2_CH2->PA1; TIM2_CH3->PB10; TIM2_CH4->PB11; 
��ȫ��TIM2_CH1->PA15; TIM2_CH2->PB3; TIM2_CH3->PB10; TIM2_CH4->PB11; 

��ʱ��3��
Ĭ�ϣ�TIM3_CH1->PA6; TIM3_CH2->PA7; TIM3_CH3->PB0; TIM3_CH4->PB1; 
���֣�TIM3_CH1->PB4; TIM3_CH2->PB5; TIM3_CH3->PB0; TIM3_CH4->PB1; 
��ȫ��TIM3_CH1->PC6; TIM3_CH2->PC7; TIM3_CH3->PC8; TIM3_CH4->PC9; 

��ʱ��4��
Ĭ�ϣ�TIM4_CH1->PB6;  TIM4_CH1->PB7;  TIM4_CH1->PB8;  TIM4_CH1->PB9; 
ӳ�䣺TIM4_CH1->PD12; TIM4_CH1->PD13; TIM4_CH1->PD14; TIM4_CH1->PD15; //ֱ��Remap

��ʱ��5��
Ĭ�ϣ�TIM5_CH1->PA0;  TIM5_CH2->PA1; TIM5_CH3->PA2; TIM5_CH4->PA3; 
*/

void TIMxxx_Int_Init(u16 arr,u16 psc);	//������ʱ����

void TIMxxx_Cap_Init(u16 arr,u16 psc);	//���벶��
void TIMxxx_Encoder_Init(u16 arr);		//����������
int  Read_Encoder(TIM_TypeDef* TIMx);

void TIMxxx_PWM_Init(u16 arr,u16 psc);	//PWM���
void TIMxxx_CMP_Init(u16 psc);			//����Ƚ�
/*�������ֵΪ65535��ÿ�αȽ��ж��������ñȽ�ֵ		
	TIM_SetCompare1(TIM3, capture + TIM_GetCapture1(TIM3));//��ԭ����CCR1����capture�������ϼ�5000�����ٹ�5000���ֻᴥ���ж�  
	//���⣬�и����⣬�����ж�ʱ����ARR��������ӵ�65535�����ֲ���5000ʱ�����ǻ������ݶ��࣬���������ε��ƶ���  
    //�ش𣺲��õ��ġ���������ж��ǣ�ARR=65000��65000+5000=70000>65535����ô��λ����ȥ����Ϊ70000-65536=4464  
	//���������˸�ѭ���������жϼ�ARR����������5000������Ϊ�˲��ε��ȶ�����ʼ��ʱ������Ҫ��TIM_Period = 65535  
*/
void TIM5_Int_Init(u16 arr,u16 psc);
void TIM3_Cap_Init(u16 arr,u16 psc);
void TIM2_Encoder_Init(u16 arr);
void TIM3_PWM_Init(u16 arr,u16 psc);
void TIM4_CMP_Init(u16 psc);

#endif

//���䣺����C8оƬ һ�߼�����ͨ�� ����ӳ�� 
/*
TIM1_CH1->PA8; TIM1_CH2->PA9; TIM1_CH3->PA10; TIM1_CH4->PA11; 
TIM2_CH1->PA0; TIM2_CH2->PA1; TIM2_CH3->PA2; TIM2_CH4->PA3; 
TIM3_CH1->PA6; TIM3_CH2->PA7; TIM3_CH3->PB0; TIM3_CH4->PB1; 
TIM4_CH1->PB6; TIM4_CH1->PB7; TIM4_CH1->PB8; TIM4_CH1->PB9; 
*/

