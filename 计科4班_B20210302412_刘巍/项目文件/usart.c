#include "usart.h"
#include <stdio.h>

uint8_t RecvData = 'Q';




//初始化串口1    bound:传进来的波特率
void Usart1_Init(int bound)
{

	//打开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1,ENABLE);
	
	//初始化我们的GPIO
	//USART1_TX -> PA9(复用推挽)			USART1_RX -> PA10(浮空输入)
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);//USART1_TX -> PA9(复用推挽)
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA,&GPIO_InitStruct);//USART1_RX -> PA10(浮空输入)
	
	//初始化串口
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = bound;//波特率
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无流控
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//收发模式
	USART_InitStruct.USART_Parity = USART_Parity_No;//无校验
	USART_InitStruct.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;//8bit数据位
	USART_Init(USART1,&USART_InitStruct);
		
	//配置NVIC
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;//串口1的中断源
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	
	
	//打开串口接收中断
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	
	//清除一下标志位  如果是上电就弄  可以不用清除
	USART_ClearFlag(USART1,USART_FLAG_RXNE);
	USART_ClearITPendingBit(USART1,USART_IT_RXNE);	
	
	//开启串口
	USART_Cmd(USART1,ENABLE);
	
}


void USART_SendSomeByte(USART_TypeDef* USARTx,uint8_t * data,int n)
{
	for(int i = 0;i < n;i++)
	{
		USART_SendData(USARTx,data[i]);
		//卡死等待数据发送完毕
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE) == RESET);
	}
}






//串口1的中断函数
void USART1_IRQHandler(void)
{
	//static uint8_t buf[] = " sb250\r\n";
	//判断你的中断是不是有了
	if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET)
	{
		//从串口里面获取数据
		RecvData = USART_ReceiveData(USART1);
		//buf[0] = RecvData;
		//USART_SendSomeByte(USART1,buf,sizeof(buf) - 1);
		//printf("%csb250\r\n",RecvData + 1);//供调试使用
		//清中断
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);	
	
	}
}



//为了可以使用printf  我们需要重写int fputc(int /*c*/, FILE * /*stream*/)这个函数
int fputc(int c, FILE * stream)
{
	USART_SendData(USART1,c);
		//卡死等待数据发送完毕
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
	return c;
}








