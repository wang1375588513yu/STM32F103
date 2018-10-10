#include "SeqList.h"
#include "SeqQueue.h"

/*
*����һ��˳����� 
*/
SeqQueue* SeqQueue_Create(void)
{
	return SeqList_Create();
}

/*
*����˳����� 
*/
void SeqQueue_Destroy(SeqQueue* queue)
{
	SeqList_Destroy(queue);
}

/*
*���˳����� 
*/
void SeqQueue_Clear(SeqQueue* queue)
{
	SeqList_Clear(queue);
}

/*
*��˳����в�����Ԫ�� item
*/
int SeqQueue_Append(SeqQueue* queue, unsigned char item)
{
	return SeqList_Insert(queue,item,SeqList_Length(queue));
}

/*
*ɾ������Ԫ�� 
*/
unsigned char SeqQueue_Retrieve(SeqQueue* queue)
{
	return SeqList_Delete(queue,0);
}

/*
*��ȡ����Ԫ�� 
*/ 
unsigned char SeqQueue_Header(SeqQueue* queue)
{
	return SeqList_Get(queue,0);
}

/*
*��ȡ���г��� 
*/
int SeqQueue_Length(SeqQueue* queue)
{
	return SeqList_Length(queue);
}

/*
*��ȡ�������� 
*/
int SeqQueue_Capacity(SeqQueue* queue)
{
	return SeqList_Capacity(queue);
}




