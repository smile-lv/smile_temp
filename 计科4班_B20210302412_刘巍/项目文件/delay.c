/*********************************************
*	文件名称：delay.c
*	作    者：李明轩
*	文件描述：测试STM32机械臂开发板的嘀嗒定时器   
*   修改时间：2021-09-09      
* 	注意事项：芯片主频72MHZ 
*********************************************/
#include "delay.h"


//系统定时器的初始化
void  delay_Init(void)
{
	//把72MHZ经过8分频 得到9MHZ  1000000us振荡9000000次  ---  1us振荡9次
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
}


//延时微秒 
void delay_us(uint16_t nus)
{
	SysTick->CTRL &= ~(1<<0); 			// 关闭定时器
	SysTick->LOAD = (9*nus) - 1; 		// 设置重装载值 value-1
	SysTick->VAL  = 0; 					// 清空当前计数值
	SysTick->CTRL |= (1<<0); 			// 开启定时器  开始倒数
	while ( (SysTick->CTRL & 0x00010000)== 0 );// 等待倒数完成
	SysTick->CTRL = 0;					// 关闭定时器
	SysTick->VAL  = 0; 					// 清空当前计数值
}

//延时毫秒 
void delay_ms(uint32_t nms)
{
	while(nms--)
	{
		delay_us(1000);
	}
}
