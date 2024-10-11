#include "list.h"



//���������ͷ�ڵ�
ListHeadNode * HeadList_Init(void)
{
	ListHeadNode * head = malloc(sizeof(ListHeadNode));
	memset(head,0,sizeof(ListHeadNode));
	return head;
}

//�������ӽڵ�
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

//���ڵ���뵽����
static void HeadList_AddListNode(ListHeadNode *head,ListNode * node)
{
	if(head == NULL || head ->num > LISTNODEMAXNUM)//�����������
		return;
	if(head ->num == 0)//��������û�ж���
	{
		head ->first = head ->last = node;
	}
	else//���������ж�����  ���뵽���һ��
	{
		head ->last ->next = node;
		head ->last = node;
	}
	head ->num++;
}

//�������ݵ�HeadList
void HeadList_AddData(ListHeadNode *head,int16_t angle1,int16_t angle2,int16_t angle3,int16_t angle4)
{
	HeadList_AddListNode(head,HeadList_CreateListNode(angle1,angle2,angle3,angle4));
}

//������� �ɵ�������� ͷ��㻹�ǻ����
//���������ǵ��εĶ���  ÿ�α��涯����ʱ������һ��
void HeadList_Clean(ListHeadNode *head)
{
	if(head == NULL)
		return;
	while(head ->first != NULL)
	{
		//ժ�����ĵ�һ���ڵ�
		ListNode * ptr = head ->first;
		head ->first = head ->first ->next;//��ͷ
		free(ptr);
	}
	head ->last = NULL;
	head ->num = 0;
}


//�ɵ���������  �������̽�����ʱ��ɵ��������Ϳ�����
void HeadList_Destroy(ListHeadNode *head)
{
	HeadList_Clean(head);
	free(head);
}



	//�����������  ��ʹ�õ�ʱ���Լ�ȥ���������
	void HeadList_Bianli(ListHeadNode *head)
	{
		ListNode * ptr = head ->first;//����ָ��
		while(ptr)
		{
			
			printf("%d\r\t",ptr ->data.angle1);//4���ǶȾ���������
			printf("%d\r\t",ptr ->data.angle2);//4���ǶȾ���������
			printf("%d\r\t",ptr ->data.angle3);//4���ǶȾ���������
			printf("%d\r\t",ptr ->data.angle4);//4���ǶȾ���������
			
			
			ptr = ptr ->next;
		}
		
	}








