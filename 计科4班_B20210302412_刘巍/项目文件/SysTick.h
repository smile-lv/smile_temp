#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f10x.h"

void SysTick_Init(void);
void delay_us(uint32_t nTime);
void delay_ms(uint32_t mTime);


//��ȡ���ĳ����� SysTick �жϺ��� SysTick_Handler()���� 
void TimingDelay_Decrement(void);

#endif 
