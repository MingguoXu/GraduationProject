#include "led.h"
    
/*************************
* LED�ƶ˿ڳ�ʼ������
*************************/
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOG, ENABLE); //ʹ��PD,PG�˿�ʱ��
                                           
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				//D5�˿�����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   		//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 		//IO���ٶ�Ϊ50MHz
    GPIO_Init(GPIOD, &GPIO_InitStructure);	

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;				//D2�˿�����
    GPIO_Init(GPIOG, &GPIO_InitStructure);
	
    GPIO_ResetBits(GPIOD, GPIO_Pin_13);						//PD13��λ,D5��
	GPIO_ResetBits(GPIOG, GPIO_Pin_14);						//PG14��λ,D2��
}
 
