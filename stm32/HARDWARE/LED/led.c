#include "led.h"
    
/*************************
* LED灯端口初始化函数
*************************/
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOG, ENABLE); //使能PD,PG端口时钟
                                           
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				//D5端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   		//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 		//IO口速度为50MHz
    GPIO_Init(GPIOD, &GPIO_InitStructure);	

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;				//D2端口配置
    GPIO_Init(GPIOG, &GPIO_InitStructure);
	
    GPIO_ResetBits(GPIOD, GPIO_Pin_13);						//PD13复位,D5灭
	GPIO_ResetBits(GPIOG, GPIO_Pin_14);						//PG14复位,D2灭
}
 
