#ifndef __io_H
#define __io_H	

#include "sys.h"

#define Relay_ON   GPIO_SetBits(GPIOE, GPIO_Pin_1)

#define Relay_OFF   GPIO_ResetBits(GPIOE, GPIO_Pin_1)

void Relay_Init(void);

#endif
