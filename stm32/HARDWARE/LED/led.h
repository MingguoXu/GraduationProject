#ifndef __LED_H
#define __LED_H	

#include "sys.h"

#define D5_ON   GPIO_SetBits(GPIOD, GPIO_Pin_13)
#define D2_ON   GPIO_SetBits(GPIOG, GPIO_Pin_14)

#define D5_OFF   GPIO_ResetBits(GPIOD, GPIO_Pin_13)
#define D2_OFF   GPIO_ResetBits(GPIOG, GPIO_Pin_14)

#define D5Flip PDout(13) = !PDin(13);
#define D2Flip PGout(14) = !PGin(14);

void LED_Init(void);

#endif
