#include "led_key.h"



void LedInit(void)
{
	//������������ʱ��   ��b���ʱ�Ӵ�
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	
	//��ʼ��GPIOB0~ 7Ϊ�������
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	GPIO_SetBits(GPIOB,GPIO_Pin_10 | GPIO_Pin_11);

}

//PA2 PA13 PA6 PA7
void KeyInit(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
}


//num:1~2�ŵ�  ����ֵΪ���е�
//0Ϊ��  ����ֵΪ��
void LedControl(int num,int value)
{
	if(num < 1 || num > 2)
		num = 0;
	if(num)
	{
		if(value)
		{
			GPIO_SetBits(GPIOB,0x01 << (9 + num));
		}
		else
		{
			GPIO_ResetBits(GPIOB,0x01 << (9 + num));
		}
	}
	else
	{
		if(value)
		{
			GPIO_SetBits(GPIOB,GPIO_Pin_10 | GPIO_Pin_11);
		}
		else
		{
			GPIO_ResetBits(GPIOB,GPIO_Pin_10 | GPIO_Pin_11);
		}
	}


}


//1~4������   ����SETΪ����  RESETΪû�а���ȥ
//����ѡ�񲻶��򷵻�RESET
uint8_t KeyControl(int num)
{
	if(num > 4 || num < 1)
		return RESET;
	if(GPIO_ReadInputDataBit(GPIOB,0x01 << (num + 11)) == RESET)
			return SET;
	return RESET;

}


