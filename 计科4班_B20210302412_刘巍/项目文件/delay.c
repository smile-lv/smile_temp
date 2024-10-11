/*********************************************
*	�ļ����ƣ�delay.c
*	��    �ߣ�������
*	�ļ�����������STM32��е�ۿ��������શ�ʱ��   
*   �޸�ʱ�䣺2021-09-09      
* 	ע�����оƬ��Ƶ72MHZ 
*********************************************/
#include "delay.h"


//ϵͳ��ʱ���ĳ�ʼ��
void  delay_Init(void)
{
	//��72MHZ����8��Ƶ �õ�9MHZ  1000000us��9000000��  ---  1us��9��
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
}


//��ʱ΢�� 
void delay_us(uint16_t nus)
{
	SysTick->CTRL &= ~(1<<0); 			// �رն�ʱ��
	SysTick->LOAD = (9*nus) - 1; 		// ������װ��ֵ value-1
	SysTick->VAL  = 0; 					// ��յ�ǰ����ֵ
	SysTick->CTRL |= (1<<0); 			// ������ʱ��  ��ʼ����
	while ( (SysTick->CTRL & 0x00010000)== 0 );// �ȴ��������
	SysTick->CTRL = 0;					// �رն�ʱ��
	SysTick->VAL  = 0; 					// ��յ�ǰ����ֵ
}

//��ʱ���� 
void delay_ms(uint32_t nms)
{
	while(nms--)
	{
		delay_us(1000);
	}
}
