#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f10x.h"

void SysTick_Init(void);
void delay_us(uint32_t nTime);
void delay_ms(uint32_t mTime);


//获取节拍程序，在 SysTick 中断函数 SysTick_Handler()调用 
void TimingDelay_Decrement(void);

#endif 
