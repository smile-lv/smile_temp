#include "usart.h"
#include <stdio.h>

uint8_t RecvData = 'Q';




//��ʼ������1    bound:�������Ĳ�����
void Usart1_Init(int bound)
{

	//��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1,ENABLE);
	
	//��ʼ�����ǵ�GPIO
	//USART1_TX -> PA9(��������)			USART1_RX -> PA10(��������)
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);//USART1_TX -> PA9(��������)
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA,&GPIO_InitStruct);//USART1_RX -> PA10(��������)
	
	//��ʼ������
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = bound;//������
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//������
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�շ�ģʽ
	USART_InitStruct.USART_Parity = USART_Parity_No;//��У��
	USART_InitStruct.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;//8bit����λ
	USART_Init(USART1,&USART_InitStruct);
		
	//����NVIC
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;//����1���ж�Դ
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	
	
	//�򿪴��ڽ����ж�
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	
	//���һ�±�־λ  ������ϵ��Ū  ���Բ������
	USART_ClearFlag(USART1,USART_FLAG_RXNE);
	USART_ClearITPendingBit(USART1,USART_IT_RXNE);	
	
	//��������
	USART_Cmd(USART1,ENABLE);
	
}


void USART_SendSomeByte(USART_TypeDef* USARTx,uint8_t * data,int n)
{
	for(int i = 0;i < n;i++)
	{
		USART_SendData(USARTx,data[i]);
		//�����ȴ����ݷ������
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE) == RESET);
	}
}






//����1���жϺ���
void USART1_IRQHandler(void)
{
	//static uint8_t buf[] = " sb250\r\n";
	//�ж�����ж��ǲ�������
	if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET)
	{
		//�Ӵ��������ȡ����
		RecvData = USART_ReceiveData(USART1);
		//buf[0] = RecvData;
		//USART_SendSomeByte(USART1,buf,sizeof(buf) - 1);
		//printf("%csb250\r\n",RecvData + 1);//������ʹ��
		//���ж�
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);	
	
	}
}



//Ϊ�˿���ʹ��printf  ������Ҫ��дint fputc(int /*c*/, FILE * /*stream*/)�������
int fputc(int c, FILE * stream)
{
	USART_SendData(USART1,c);
		//�����ȴ����ݷ������
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
	return c;
}








