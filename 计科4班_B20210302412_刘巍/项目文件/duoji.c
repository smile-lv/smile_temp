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




//我们需要弄一个偏移的东西来指明现在你弄的是哪一个字节
uint8_t Duoji_flag = 0;

//由于数据包长度不固定  因此我们可以设置一个较长的内存来存储数据
uint8_t Duoji_RecvBuf[32] = {0};

//获取检验码
uint8_t GetCheckNum(uint8_t * cmd,int n)
{
	uint8_t checknum = 0;
	for(int i = 0;i < n;i++)
	{
		checknum += cmd[i];
	}
	return checknum;
}

//ping一下舵机 你要告诉我ping哪一个舵机：num 1~4 不在这个范围就是所有的在线舵机
//ping成功返回 Duoji_Success     失败返回Duoji_Failed
DuojiStatus PingDuoji(uint8_t id)
{
	if(id < 1 || id > 4)
		id = 0xff;
	uint8_t PingCmd[6] = {0x12,0x4c,0x01,0x01};
	PingCmd[4] = id;//哪一个舵机
	PingCmd[5] = GetCheckNum(PingCmd,5);
	
	Duoji_flag = 0;//你接下来收的数据就从第一个字节开始放起
	
	//将这个包发给舵机
	USART_SendSomeByte(USART2,PingCmd,6);
	
	//看看有没有ping成功
	//进行超时检测
	int counter = 0;
	while(Duoji_flag < 6 && ++counter < 1000)
	{
		delay_ms(1);
	}
	if(counter >= 1000)
	{
		//ping失败了
		printf("duoji %d ping failed:time out\r\n",id);
		return Duoji_Failed;
	}
	uint8_t check = GetCheckNum(Duoji_RecvBuf,5);//计算校验值 和发过来的校验对比
	if(Duoji_RecvBuf[0] != 0x05 || Duoji_RecvBuf[1] != 0x1c || 
			Duoji_RecvBuf[2] != 0x01 || Duoji_RecvBuf[3] != 0x01 || 
				Duoji_RecvBuf[4] != id || Duoji_RecvBuf[5] != check)
	{
		printf("duoji %d ping failed:check error\r\n",id);
		return Duoji_Failed;
	}

	//需要检验才能最终确定是不是成功
	
	return Duoji_Success;
	
	
}
int16_t  ReadAngle(uint8_t id)
{
	if(id < 1 || id > 4)
	id = 0xff;
	uint8_t ReadCmd[6] = {0x12,0x4c,0x0A,0x01};
	ReadCmd[4] = id;//哪一个舵机
	ReadCmd[5] = GetCheckNum(ReadCmd,5);
	Duoji_flag = 0;//你接下来收的数据就从第一个字节开始放起

	//将这个包发给舵机
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
	
	//将这个消息发给舵机
	USART_SendSomeByte(USART2,Cmd,12);
		
}







//初始化舵机就是初始化串口2
//初始化串口2   bound:传进来的波特率
void Duoji_Init(int bound)
{

	//打开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO ,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	//初始化我们的GPIO
	//USART2_TX -> PA2(复用推挽)			USART2_RX -> PA3(浮空输入)
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);//USART2_TX -> PA2(复用推挽)
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA,&GPIO_InitStruct);//USART2_RX -> PA3(浮空输入)
	
	//初始化串口
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = bound;//波特率
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无流控
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//收发模式
	USART_InitStruct.USART_Parity = USART_Parity_No;//无校验
	USART_InitStruct.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;//8bit数据位
	USART_Init(USART2,&USART_InitStruct);
		
	//配置NVIC
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;//串口2的中断源
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	//打开串口接收中断
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	
	//清除一下标志位  如果是上电就弄  可以不用清除
	USART_ClearFlag(USART2,USART_FLAG_RXNE);
	USART_ClearITPendingBit(USART2,USART_IT_RXNE);	
	
	//开启串口
	USART_Cmd(USART2,ENABLE);
}


//数据接收在中断

//串口2的中断函数
void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2,USART_IT_RXNE) == SET)
	{
		//从串口里面获取数据
		Duoji_RecvBuf[Duoji_flag] = USART_ReceiveData(USART2);
		if(Duoji_flag < 31)//你不能越界了
			Duoji_flag++;//放完一个就往后面走一个
	  RecvData = USART_ReceiveData(USART1);
	
		//清中断
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);	
	
	}
}

void play_record(ListHeadNode *head)
{
	#if 0
	ListNode * ptr = head ->first;//遍历指针
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
		printf("%d++++%d  %d  %d  %d\r\n",i,Duiji_Array[i].angle1,Duiji_Array[i].angle2,Duiji_Array[i].angle3,Duiji_Array[i].angle4);//4个角度就在这里拿

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
		printf("%d++++%d  %d  %d  %d\r\n",i,Duiji_Array[i].angle1,Duiji_Array[i].angle2,Duiji_Array[i].angle3,Duiji_Array[i].angle4);//4个角度就在这里拿

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
