#ifndef _ESP8266_H_
#define _ESP8266_H_

#include "stm32f10x.h"

extern _Bool WIFI_Reset_Flag;
extern  char esp8266_buf[];

void ESP8266_Init(void);

void ESP8266_Wait_RecData(void);

void OneNET_SendData(void);

#endif
