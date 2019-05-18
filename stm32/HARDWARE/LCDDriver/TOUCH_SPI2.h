#ifndef __touch_spi2_h__
#define __touch_spi2_h__

#include"sys.h"

u16 TPReadX(void);
u16 TPReadY(void);

void SPI_Config(void); 	
void TouchScan(void);
	
#endif
