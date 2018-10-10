#include<stdio.h>
#include"SeqList.h"



typedef unsigned char TSeqListNode;

typedef struct _tag_SeqList
{
	int capacity;               //��ʾ���Ա��������
	int length;                 //��ʾ���Ա�ǰԪ�ظ���
	TSeqListNode* node;         //��ʾ���Ա�̬������ռ䣬������ָ�룬���������� 
}TSeqList;

TSeqList Queue_Info;

unsigned char Queue_Info_buf[5] = {0};

/*
    �÷������ڴ������ҷ���һ���յ����Ա�
*/
SeqList* SeqList_Create(void)
{
	TSeqList* ret = NULL;
	

	ret = &Queue_Info;
	
	if( ret != NULL)
	{
		ret->capacity = 5;
		ret->length = 0;
		ret->node = Queue_Info_buf;
	}
	
	return ret;
}

/*
    �÷�����������һ�����Ա�list
*/
void SeqList_Destroy(SeqList* list)
{
//	free(list);
}

/*
    �÷������ڽ�һ�����Ա�list�е�����Ԫ�����
    ʹ�����Ա�ص�����ʱ�ĳ�ʼ״̬
*/
void SeqList_Clear(SeqList* list)
{
	TSeqList* sList = (TSeqList*)list;
	int ret = ( list != NULL );
	int i = 0;
	
	if(ret)
	{
		sList->length = 0;
		
		for(i = 0;i < sList->capacity;i++)
			sList->node[i] = 0xff;
	}
}

/*
    �÷������ڷ���һ�����Ա�list�е�����Ԫ�ظ���
*/
int SeqList_Length(SeqList* list)
{
	int ret = -1;
	TSeqList* sList = (TSeqList*)list;
	
	if( list != NULL)
	{
		ret = sList->length;
	}
	
	return ret;
}

/*
	�÷������ڷ������Ա�list�е�������� 
*/
int SeqList_Capacity(SeqList* list)
{
	int ret = -1;
	TSeqList* sList = (TSeqList*)list;
	
	if( list != NULL)
	{
		ret = sList->capacity;
	}
	
	return ret;
}

/*
    �÷���������һ�����Ա�list��posλ�ô�������Ԫ��node
    ����ֵΪ1��ʾ����ɹ���0��ʾ����ʧ��
*/
int SeqList_Insert(SeqList* list, SeqListNode node, int pos)
{
		//1.�ж����Ա��Ƿ�Ϸ�
	int ret = (list != NULL);
	TSeqList* sList = (TSeqList*)list;
//	int i;
	
	//2.�жϲ���λ���Ƿ�Ϸ� 
	ret = ret && ( sList->length + 1<= sList->capacity);
	ret = ret && (pos >= 0);
	
	if( ret )
	{
		if( pos >= sList->length )
		{
			pos = sList->length;
		}
		
		//3.�����һ��Ԫ�ص�����λ�õ�Ԫ�ض�����ƶ�һλ
//		for(i = sList->length ; i > pos; i--)
//		{
//			sList->node[i] = sList->node[i-1];
//		} 
		
		 //4.��Ԫ�ز���
//		 sList->node[i] = (TSeqListNode)node;
		 
		 sList->node[sList->length] = (TSeqListNode)node;
		 
		//5.���Ա��ȼ�1 
		sList->length++;
		if(sList->capacity < sList->length)
			sList->length = sList->capacity;
	}
	
	return ret;
}

/*
    �÷������ڻ�ȡһ�����Ա�list��posλ�ô���Ԫ��
    ����ֵΪposλ�ô���Ԫ�أ�NULL��ʾ��ȡʧ��
*/
SeqListNode SeqList_Get(SeqList* list, int pos)
{
	SeqListNode ret = 0xff;
	TSeqList* sList = (TSeqList*)list;
	
	if( (list != NULL) && (pos >= 0) && (pos < sList->length) )
	{
		ret = (TSeqListNode)(sList->node[pos]);
	}
	
	return ret;
}

/*
    �÷�������ɾ��һ�����Ա�list��posλ�ô���Ԫ��
    ����ֵΪ��ɾ����Ԫ�أ�NULL��ʾɾ��ʧ��
*/
SeqListNode SeqList_Delete(SeqList* list, int pos) 
{
	SeqListNode ret = SeqList_Get(list,pos);
	TSeqList* sList = (TSeqList*)list;
	int i;
	
	if( ret != 0)
	{
		//��pos+1��Ԫ��֮�������Ԫ�ض���ǰ�ƶ�һλ 
		for(i = pos +1;i < sList->length;i++)
		{
			sList->node[i-1] = sList->node[i];
		}
		
		if(sList->length > 0)
			sList->node[sList->length-1] = 0xff;
		else
			sList->node[sList->length] = 0xff;
			
		if(sList->length > 0)
			sList->length--;
	}
	
	return ret;
}





