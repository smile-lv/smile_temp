/*********************************************
*	�ļ����ƣ�delay.h
*	��    �ߣ�������
*	�ļ�����������STM32��е�ۿ��������શ�ʱ��   
*   �޸�ʱ�䣺2021-09-09      
* 	ע�����оƬ��Ƶ72MHZ 
*********************************************/
#ifndef _DELAY_H
#define _DELAY_H

#include "stm32f10x.h"


void delay_Init(void);          //ϵͳ��ʱ���ĳ�ʼ��
void delay_us(uint16_t nus);    //��ʱ΢��  
void delay_ms(uint32_t nms);    //��ʱ����



#endif
