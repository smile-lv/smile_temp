#include "led_key.h"



void LedInit(void)
{
	//打开这个玩意儿的时钟   将b组的时钟打开
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	
	//初始化GPIOB0~ 7为推挽输出
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


//num:1~2号灯  其他值为所有的
//0为亮  其他值为灭
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


//1~4个按键   返回SET为按下  RESET为没有按下去
//按键选择不对则返回RESET
uint8_t KeyControl(int num)
{
	if(num > 4 || num < 1)
		return RESET;
	if(GPIO_ReadInputDataBit(GPIOB,0x01 << (num + 11)) == RESET)
			return SET;
	return RESET;

}


