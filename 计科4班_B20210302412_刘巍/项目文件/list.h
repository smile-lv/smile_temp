#ifndef __LIST_H__
#define __LIST_H__
#include "delay.h"
#include "usart.h"



#include<stdio.h>

#include <stdlib.h>
#include <string.h>

//链表节点最大上限限制






#define LISTNODEMAXNUM 10000


//数据域 的数据类型  保存同一个时间的4个舵机的角度
typedef struct 
{
	int16_t angle1;
	int16_t angle2;
	int16_t angle3;
	int16_t angle4;//
}DataNode;


//链表节点的类型
typedef struct ListNode
{
	DataNode data;
	struct ListNode * next;
}ListNode;


//头结点类型
typedef struct ListHeadNode
{
	ListNode * first;//第一个节点
	ListNode * last;//最后一个节点
	int num;//链表里面有多少个元素
}ListHeadNode;




//返回链表的头节点首先需要调用此函数
ListHeadNode* HeadList_Init(void);

//加入数据到HeadList
void HeadList_AddData(ListHeadNode *head,int16_t angle1,int16_t angle2,int16_t angle3,int16_t angle4);

//清空链表 干掉这个链表 头结点还是会存在
//如果保存的是单次的动作  每次保存动作的时候就清除一下
void HeadList_Clean(ListHeadNode *head);

//干掉整个链表  整个工程结束的时候干掉这个链表就可以了
void HeadList_Destroy(ListHeadNode *head);

void HeadList_Bianli(ListHeadNode *head);

#endif


