#ifndef __USART_H__
#define __USART_H__

#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "misc.h"


//��ʼ������1    bound:�������Ĳ�����
void Usart1_Init(int bound);
void USART_SendSomeByte(USART_TypeDef* USARTx,uint8_t * data,int n);


#endif



