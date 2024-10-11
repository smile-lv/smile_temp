#include "duoji.h"
#include "usart.h"
#include "delay.h"
#include "stdio.h"


#define Change_angle  50
int Duiji_Arrayflag = 0;
DataNode Duiji_Array[200];
extern uint8_t RecvData;
int angle1 = 10;
int angle2 = 10;
int angle3 = 10;
int angle4 = 10;




//������ҪŪһ��ƫ�ƵĶ�����ָ��������Ū������һ���ֽ�
uint8_t Duoji_flag = 0;

//�������ݰ����Ȳ��̶�  ������ǿ�������һ���ϳ����ڴ����洢����
uint8_t Duoji_RecvBuf[32] = {0};

//��ȡ������
uint8_t GetCheckNum(uint8_t * cmd,int n)
{
	uint8_t checknum = 0;
	for(int i = 0;i < n;i++)
	{
		checknum += cmd[i];
	}
	return checknum;
}

//pingһ�¶�� ��Ҫ������ping��һ�������num 1~4 ���������Χ�������е����߶��
//ping�ɹ����� Duoji_Success     ʧ�ܷ���Duoji_Failed
DuojiStatus PingDuoji(uint8_t id)
{
	if(id < 1 || id > 4)
		id = 0xff;
	uint8_t PingCmd[6] = {0x12,0x4c,0x01,0x01};
	PingCmd[4] = id;//��һ�����
	PingCmd[5] = GetCheckNum(PingCmd,5);
	
	Duoji_flag = 0;//��������յ����ݾʹӵ�һ���ֽڿ�ʼ����
	
	//��������������
	USART_SendSomeByte(USART2,PingCmd,6);
	
	//������û��ping�ɹ�
	//���г�ʱ���
	int counter = 0;
	while(Duoji_flag < 6 && ++counter < 1000)
	{
		delay_ms(1);
	}
	if(counter >= 1000)
	{
		//pingʧ����
		printf("duoji %d ping failed:time out\r\n",id);
		return Duoji_Failed;
	}
	uint8_t check = GetCheckNum(Duoji_RecvBuf,5);//����У��ֵ �ͷ�������У��Ա�
	if(Duoji_RecvBuf[0] != 0x05 || Duoji_RecvBuf[1] != 0x1c || 
			Duoji_RecvBuf[2] != 0x01 || Duoji_RecvBuf[3] != 0x01 || 
				Duoji_RecvBuf[4] != id || Duoji_RecvBuf[5] != check)
	{
		printf("duoji %d ping failed:check error\r\n",id);
		return Duoji_Failed;
	}

	//��Ҫ�����������ȷ���ǲ��ǳɹ�
	
	return Duoji_Success;
	
	
}
int16_t  ReadAngle(uint8_t id)
{
	if(id < 1 || id > 4)
	id = 0xff;
	uint8_t ReadCmd[6] = {0x12,0x4c,0x0A,0x01};
	ReadCmd[4] = id;//��һ�����
	ReadCmd[5] = GetCheckNum(ReadCmd,5);
	Duoji_flag = 0;//��������յ����ݾʹӵ�һ���ֽڿ�ʼ����

	//��������������
	USART_SendSomeByte(USART2,ReadCmd,sizeof(ReadCmd));
	
	int counter = 0;
	while(Duoji_flag < 8 && ++counter < 1000)
		delay_us(1);
	if(counter >= 1000)
		printf("read angle err : time out\r\n");
	int16_t angle = *(int16_t *)(Duoji_RecvBuf+5);
	
	return angle;
	
}
void SetDamp(uint8_t id,uint16_t power)
{
	uint8_t Damp_cmd[8] = {0x12,0x4c,0x09,0x03};
	Damp_cmd[4]=id;
	uint16_t *ptr =(uint16_t*)(Damp_cmd+5);
	*ptr=power; 
	ptr++;
	Damp_cmd[7] = GetCheckNum(Damp_cmd,7);
	USART_SendSomeByte( USART2,Damp_cmd,sizeof(Damp_cmd));
	//int counter = 0;
	//while(Duoji_flag < 7 && ++counter < 1000)
		//delay_us(1);
	//if(counter >= 1000)
		//printf("read angle err : time out\r\n");
		
}
void SetAngle(uint8_t id,int16_t angle,uint16_t interval,uint16_t power)
{
	if(id < 1 || id > 4)
		id = 0xff;
	uint8_t Cmd[12] = {0x12,0x4c,0x08,0x07};
	Cmd[4] = id;
	int16_t * ptr = (int16_t *)(Cmd + 5);
	*ptr = angle;
	uint16_t * ptr1 = (uint16_t *)(Cmd + 7);
	*ptr1 = interval;
	ptr1++;
	*ptr1 = power;
	Cmd[11] = GetCheckNum(Cmd,11);
	
	//�������Ϣ�������
	USART_SendSomeByte(USART2,Cmd,12);
		
}







//��ʼ��������ǳ�ʼ������2
//��ʼ������2   bound:�������Ĳ�����
void Duoji_Init(int bound)
{

	//��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO ,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	//��ʼ�����ǵ�GPIO
	//USART2_TX -> PA2(��������)			USART2_RX -> PA3(��������)
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);//USART2_TX -> PA2(��������)
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA,&GPIO_InitStruct);//USART2_RX -> PA3(��������)
	
	//��ʼ������
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = bound;//������
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//������
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�շ�ģʽ
	USART_InitStruct.USART_Parity = USART_Parity_No;//��У��
	USART_InitStruct.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;//8bit����λ
	USART_Init(USART2,&USART_InitStruct);
		
	//����NVIC
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;//����2���ж�Դ
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	//�򿪴��ڽ����ж�
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	
	//���һ�±�־λ  ������ϵ��Ū  ���Բ������
	USART_ClearFlag(USART2,USART_FLAG_RXNE);
	USART_ClearITPendingBit(USART2,USART_IT_RXNE);	
	
	//��������
	USART_Cmd(USART2,ENABLE);
}


//���ݽ������ж�

//����2���жϺ���
void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2,USART_IT_RXNE) == SET)
	{
		//�Ӵ��������ȡ����
		Duoji_RecvBuf[Duoji_flag] = USART_ReceiveData(USART2);
		if(Duoji_flag < 31)//�㲻��Խ����
			Duoji_flag++;//����һ������������һ��
	  RecvData = USART_ReceiveData(USART1);
	
		//���ж�
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);	
	
	}
}

void play_record(ListHeadNode *head)
{
	#if 0
	ListNode * ptr = head ->first;//����ָ��
	int i = 0;
	while(ptr)
	{	
		SetAngle(1,ptr->data.angle1,500,20);
		SetAngle(2,ptr->data.angle2,500,20);
		SetAngle(3,ptr->data.angle3,500,20);
		SetAngle(4,ptr->data.angle4,500,20);		
		printf("%d===%d  %d  %d  %d\r\n",i++,ptr->data.angle1,ptr->data.angle2,ptr->data.angle3,ptr->data.angle4);
		ptr = ptr ->next;
		delay_ms(500);
		
	}
	#endif
	for(int i = 0;i<200;i++)
	{
		
		SetAngle(1,Duiji_Array[i].angle1,50,0);
		SetAngle(2,Duiji_Array[i].angle2,50,0);
		SetAngle(3,Duiji_Array[i].angle3,50,0);
		SetAngle(4,Duiji_Array[i].angle4,50,0);		
		printf("%d++++%d  %d  %d  %d\r\n",i,Duiji_Array[i].angle1,Duiji_Array[i].angle2,Duiji_Array[i].angle3,Duiji_Array[i].angle4);//4���ǶȾ���������

		delay_ms(45);
	}
	SetDamp(1,1);
	SetDamp(2,1);
	SetDamp(3,1);
	SetDamp(4,1);
}
void record_list(ListHeadNode *head)
{
	
	SetDamp(1,1);
	SetDamp(2,1);
	SetDamp(3,1);
	SetDamp(4,1);

	//DataNode Duiji_Array[100];
	for(int i = 0;i<200;i++)
	{
		
		Duiji_Array[i].angle1 =  ReadAngle(1);
		Duiji_Array[i].angle2 =  ReadAngle(2);
		Duiji_Array[i].angle3 =  ReadAngle(3);
		Duiji_Array[i].angle4 =  ReadAngle(4);
		printf("%d++++%d  %d  %d  %d\r\n",i,Duiji_Array[i].angle1,Duiji_Array[i].angle2,Duiji_Array[i].angle3,Duiji_Array[i].angle4);//4���ǶȾ���������

		delay_ms(50);
	}


}
void duoji_control()
{
		
			if(RecvData =='E')
			{
				RecvData ='Z';
				angle1+=Change_angle  ;
				SetAngle(1,angle1,50,0);
				delay_ms(100);
				//SetDamp(1,1);
			}
			 if(RecvData =='F')
			{
				RecvData ='Z';
				angle1-=Change_angle ;
				SetAngle(1,angle1,50,0);
				delay_ms(100);
				//SetDamp(1,1);
			}
			
			 if(RecvData =='H')
			{
				
				RecvData ='Z';
				angle2+=Change_angle  ;
				SetAngle(2,angle2,50,0);
				delay_ms(100);
				//SetDamp(2,1);
			}
			 if(RecvData =='I')
			{
				RecvData ='Z';
				angle2-=Change_angle ;
				SetAngle(2,angle2,50,0);
				delay_ms(100);
				//SetDamp(2,1);
			}	
			
			 if(RecvData =='J')
			{
				
				RecvData ='Z';
				angle3+=Change_angle  ;
				SetAngle(3,angle3,50,0);
				delay_ms(100);
				//SetDamp(3,1);
			}
			 if(RecvData =='K')
			{
				RecvData ='Z';
				angle3-=Change_angle ;
				SetAngle(3,angle3,50,0);
				delay_ms(100);
				//SetDamp(3,1);
			}	
			
			 if(RecvData =='M')
			{
				
				RecvData ='Z';
				angle4+=Change_angle  ;
				SetAngle(4,angle4,50,0);
				delay_ms(100);
				//SetDamp(4,1);
			}
			 if(RecvData =='N')
			{
				RecvData ='Z';
				angle4-=Change_angle ;
				SetAngle(4,angle4,50,0);
				delay_ms(100);
				//SetDamp(4,1);
			}	
			if(RecvData == 'D')
			{
			SetDamp(1,1);
			SetDamp(2,1);
			SetDamp(3,1);
			SetDamp(4,1);
			}
}
