#ifndef __DUOJI_H__
#define __DUOJI_H__

#include <stdlib.h>
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "misc.h"

#include "list.h"



//舵机的状态
typedef enum
{
	Duoji_Success,
	Duoji_Failed
}DuojiStatus;
typedef  struct Node
{
	int data;
	uint32_t duoji ;
	struct Node *next;
}link;
	


//获取检验码
uint8_t GetCheckNum(uint8_t * cmd,int n);

//ping一下舵机 你要告诉我ping哪一个舵机：id 1~4 不在这个范围就是所有的在线舵机
//ping成功返回 Duoji_Success     失败返回Duoji_Failed
DuojiStatus PingDuoji(uint8_t id);

//舵机的角度控制
//id:1~4 其他值为所有的在线舵机  angle：角度 符号为方向
//interval:到指定角度的时间 power：功率  0或者超过保护功率为保护功率
void SetAngle(uint8_t id,int16_t angle,uint16_t interval,uint16_t power);



//初始化舵机就是初始化串口2
//初始化串口2   bound:传进来的波特率
void Duoji_Init(int bound);
int16_t  ReadAngle(uint8_t id);
void play_record(ListHeadNode *head);
void record_list(ListHeadNode *head);
void SetDamp(uint8_t id,uint16_t power);
void duoji_control(void);

#endif


