#include "list.h"



//返回链表的头节点
ListHeadNode * HeadList_Init(void)
{
	ListHeadNode * head = malloc(sizeof(ListHeadNode));
	memset(head,0,sizeof(ListHeadNode));
	return head;
}

//创建连接节点
static ListNode * HeadList_CreateListNode(int16_t angle1,int16_t angle2,int16_t angle3,int16_t angle4)
{
	ListNode * ptr = malloc(sizeof(ListNode));
	ptr ->data.angle1 = angle1;
	ptr ->data.angle2 = angle2;
	ptr ->data.angle3 = angle3;
	ptr ->data.angle4 = angle4;
	ptr ->next = NULL;
	return ptr;
}

//将节点加入到链表
static void HeadList_AddListNode(ListHeadNode *head,ListNode * node)
{
	if(head == NULL || head ->num > LISTNODEMAXNUM)//最大上限限制
		return;
	if(head ->num == 0)//链表里面没有东西
	{
		head ->first = head ->last = node;
	}
	else//链表里面有东西了  加入到最后一个
	{
		head ->last ->next = node;
		head ->last = node;
	}
	head ->num++;
}

//加入数据到HeadList
void HeadList_AddData(ListHeadNode *head,int16_t angle1,int16_t angle2,int16_t angle3,int16_t angle4)
{
	HeadList_AddListNode(head,HeadList_CreateListNode(angle1,angle2,angle3,angle4));
}

//清空链表 干掉这个链表 头结点还是会存在
//如果保存的是单次的动作  每次保存动作的时候就清除一下
void HeadList_Clean(ListHeadNode *head)
{
	if(head == NULL)
		return;
	while(head ->first != NULL)
	{
		//摘下它的第一个节点
		ListNode * ptr = head ->first;
		head ->first = head ->first ->next;//换头
		free(ptr);
	}
	head ->last = NULL;
	head ->num = 0;
}


//干掉整个链表  整个工程结束的时候干掉这个链表就可以了
void HeadList_Destroy(ListHeadNode *head)
{
	HeadList_Clean(head);
	free(head);
}



	//遍历这个链表  在使用的时候自己去做这个事情
	void HeadList_Bianli(ListHeadNode *head)
	{
		ListNode * ptr = head ->first;//遍历指针
		while(ptr)
		{
			
			printf("%d\r\t",ptr ->data.angle1);//4个角度就在这里拿
			printf("%d\r\t",ptr ->data.angle2);//4个角度就在这里拿
			printf("%d\r\t",ptr ->data.angle3);//4个角度就在这里拿
			printf("%d\r\t",ptr ->data.angle4);//4个角度就在这里拿
			
			
			ptr = ptr ->next;
		}
		
	}








