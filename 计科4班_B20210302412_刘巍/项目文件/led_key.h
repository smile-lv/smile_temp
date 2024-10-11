#ifndef __LED_KEY_H__
#define __LED_KEY_H__


#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

void LedInit(void);

void KeyInit(void);

//num:1~2�ŵ�  ����ֵΪ���е�
//0Ϊ��  ����ֵΪ��
void LedControl(int num,int value);

//1~4������   ����SETΪ����  RESETΪû�а���ȥ
//����ѡ�񲻶��򷵻�RESET
uint8_t KeyControl(int num);



#endif


