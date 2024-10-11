/*********************************************
*	文件名称：delay.h
*	作    者：李明轩
*	文件描述：测试STM32机械臂开发板的嘀嗒定时器   
*   修改时间：2021-09-09      
* 	注意事项：芯片主频72MHZ 
*********************************************/
#ifndef _DELAY_H
#define _DELAY_H

#include "stm32f10x.h"


void delay_Init(void);          //系统定时器的初始化
void delay_us(uint16_t nus);    //延时微秒  
void delay_ms(uint32_t nms);    //延时毫秒



#endif
