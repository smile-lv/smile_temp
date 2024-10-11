#ifndef __DHT11_H__
#define __DHT11_H__


#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"


void DHT11_Init(void);

void DHT11_ReadTempHum(uint8_t * temp,uint8_t * hum);





#endif


