#include "sys.h"
#include "key.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "timer.h"
#include "ili9320.h"
#include "adda.h"
#include "io.h"
#include "control.h"
#include "touch_spi2.h"

int bugNum;
u8 uiMode;
extern u8 touchButton;

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
	
	KEY_Init();					//按键、LED等
	LED_Init();
	Relay_Init();				//PE1
	
	delay_init();				//初始化嘀嗒定时器
	uart_init(115200);			//与串口软件交互
	
	ADC1_Init();				//PA2、PA3
	DAC1_Init();				//PA4
	DAC1_SetVol(1234);
	
	PID_init();					//初始化pid参数
	
	SPI_Config();				//触摸屏spi2 pb12-15
	ili9320_Initializtion();	//显示屏初始化
    GUI_Main();
	
	TIM2_Encoder_Init(60000);	//PA0 PA1
	TIM5_Int_Init(999,7199);	//100ms
	TIM3_PWM_Init(9999,5);		//72000000/(5+1)/(11999+1)=1000hz 最大11999 PC6
	
	while(1)
	{
		int val;
		val = KEY_Scan(0);
		switch(val)
		{
			case KEY1_PRES:
			{
				D5Flip;
				if(motor.Set < 800)
				{
					motor.Set += 50;
					GUI_Word(190, 204, 4, motor.Set, 0, Black, White);
				}
				break;
			}
			case KEY2_PRES:
			{
				D2Flip;
				if(motor.Set > 0)
				{
					motor.Set -= 50;
					GUI_Word(190, 204, 4, motor.Set, 0, Black, White);
				}
				break;
			}
			default:break;
		}
		if(touchButton)
		{
			touchButton = 0;
			uiMode = 1 - uiMode;
			if(uiMode)
				GUI_Scope();
			else
				GUI_Main();
		}	
	}
}
