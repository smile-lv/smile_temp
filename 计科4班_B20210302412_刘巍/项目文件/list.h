#ifndef __LIST_H__
#define __LIST_H__
#include "delay.h"
#include "usart.h"



#include<stdio.h>

#include <stdlib.h>
#include <string.h>

//����ڵ������������






#define LISTNODEMAXNUM 10000


//������ ����������  ����ͬһ��ʱ���4������ĽǶ�
typedef struct 
{
	int16_t angle1;
	int16_t angle2;
	int16_t angle3;
	int16_t angle4;//
}DataNode;


//����ڵ������
typedef struct ListNode
{
	DataNode data;
	struct ListNode * next;
}ListNode;


//ͷ�������
typedef struct ListHeadNode
{
	ListNode * first;//��һ���ڵ�
	ListNode * last;//���һ���ڵ�
	int num;//���������ж��ٸ�Ԫ��
}ListHeadNode;




//���������ͷ�ڵ�������Ҫ���ô˺���
ListHeadNode* HeadList_Init(void);

//�������ݵ�HeadList
void HeadList_AddData(ListHeadNode *head,int16_t angle1,int16_t angle2,int16_t angle3,int16_t angle4);

//������� �ɵ�������� ͷ��㻹�ǻ����
//���������ǵ��εĶ���  ÿ�α��涯����ʱ������һ��
void HeadList_Clean(ListHeadNode *head);

//�ɵ���������  �������̽�����ʱ��ɵ��������Ϳ�����
void HeadList_Destroy(ListHeadNode *head);

void HeadList_Bianli(ListHeadNode *head);

#endif


