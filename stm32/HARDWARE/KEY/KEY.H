#ifndef __KEY_H
#define __KEY_H	

#include "sys.h"

#define KEY1  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0)//��ȡ����1
#define KEY2  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)//��ȡ����2

#define KEY1_PRES 	1	//KEY1����
#define KEY2_PRES	2	//KEY2����

extern void KEY_Init(void);		//������ʼ��
extern u8 KEY_Scan(u8);  		//����ɨ�躯��		
extern void KEY_INT_Init(void);
extern void KeyPlus_Init(void);	
extern u8 KeyPlus_Scan(void);
    
#endif