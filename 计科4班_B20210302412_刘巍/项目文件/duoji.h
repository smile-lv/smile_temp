#ifndef __DUOJI_H__
#define __DUOJI_H__

#include <stdlib.h>
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "misc.h"

#include "list.h"



//�����״̬
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
	


//��ȡ������
uint8_t GetCheckNum(uint8_t * cmd,int n);

//pingһ�¶�� ��Ҫ������ping��һ�������id 1~4 ���������Χ�������е����߶��
//ping�ɹ����� Duoji_Success     ʧ�ܷ���Duoji_Failed
DuojiStatus PingDuoji(uint8_t id);

//����ĽǶȿ���
//id:1~4 ����ֵΪ���е����߶��  angle���Ƕ� ����Ϊ����
//interval:��ָ���Ƕȵ�ʱ�� power������  0���߳�����������Ϊ��������
void SetAngle(uint8_t id,int16_t angle,uint16_t interval,uint16_t power);



//��ʼ��������ǳ�ʼ������2
//��ʼ������2   bound:�������Ĳ�����
void Duoji_Init(int bound);
int16_t  ReadAngle(uint8_t id);
void play_record(ListHeadNode *head);
void record_list(ListHeadNode *head);
void SetDamp(uint8_t id,uint16_t power);
void duoji_control(void);

#endif


