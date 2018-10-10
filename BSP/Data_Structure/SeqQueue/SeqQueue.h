#ifndef _SEQQUEUE_H_
#define _SEQQUEUE_H_


typedef unsigned char SeqQueue;
/*
*����һ��˳����� 
*/
SeqQueue* SeqQueue_Create(void);

/*
*����˳����� 
*/
void SeqQueue_Destroy(SeqQueue* queue);

/*
*���˳����� 
*/
void SeqQueue_Clear(SeqQueue* queue);

/*
*��˳����в�����Ԫ�� 
*/
int SeqQueue_Append(SeqQueue* queue, unsigned char item);

/*
*ɾ������Ԫ�� 
*/
unsigned char SeqQueue_Retrieve(SeqQueue* queue);

/*
*��ȡ����Ԫ�� 
*/ 
unsigned char SeqQueue_Header(SeqQueue* queue);

/*
*��ȡ���г��� 
*/
int SeqQueue_Length(SeqQueue* queue);

/*
*��ȡ�������� 
*/
int SeqQueue_Capacity(SeqQueue* queue);


#endif







