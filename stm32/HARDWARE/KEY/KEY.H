#ifndef __KEY_H
#define __KEY_H	

#include "sys.h"

#define KEY1  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0)//读取按键1
#define KEY2  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)//读取按键2

#define KEY1_PRES 	1	//KEY1按下
#define KEY2_PRES	2	//KEY2按下

extern void KEY_Init(void);		//按键初始化
extern u8 KEY_Scan(u8);  		//按键扫描函数		
extern void KEY_INT_Init(void);
extern void KeyPlus_Init(void);	
extern u8 KeyPlus_Scan(void);
    
#endif
