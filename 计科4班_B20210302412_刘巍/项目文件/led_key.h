#ifndef __LED_KEY_H__
#define __LED_KEY_H__


#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

void LedInit(void);

void KeyInit(void);

//num:1~2号灯  其他值为所有的
//0为亮  其他值为灭
void LedControl(int num,int value);

//1~4个按键   返回SET为按下  RESET为没有按下去
//按键选择不对则返回RESET
uint8_t KeyControl(int num);



#endif


