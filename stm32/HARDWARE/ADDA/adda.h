#ifndef __adda_H
#define __adda_H	

#include "sys.h"

extern void ADC1_Init(void);
extern u16 Get_Adc(u8 ch);
extern u16 Get_Adc_Average(u8 ch,u8 times);
extern u16 Get_Adc_AvgNX(u8 ch);

extern void DAC1_Init(void);
extern void DAC1_SetVol(u16 vol);

#endif
