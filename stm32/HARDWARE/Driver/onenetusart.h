#ifndef _onenetusart_h_
#define _onenetusart_h_


#include "stm32f10x.h"
#include "stdio.h"

#define USART_DEBUG		USART1		//���Դ�ӡ��ʹ�õĴ�����

void Usart2_Init(unsigned int baud);

void Usart_SendString(USART_TypeDef *USARTx,char *str, unsigned short len);

//void UsartPrintf(USART_TypeDef *USARTx, char *fmt,...);

#endif
