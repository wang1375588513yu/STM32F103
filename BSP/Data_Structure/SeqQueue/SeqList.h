#ifndef _SEQLIST_H_
#define _SEQLIST_H_

typedef void SeqList;
typedef unsigned char SeqListNode;

/*
    �÷������ڴ������ҷ���һ���յ����Ա�
*/
SeqList* SeqList_Create(void);

/*
    �÷�����������һ�����Ա�list
*/
void SeqList_Destroy(SeqList* list);

/*
    �÷������ڽ�һ�����Ա�list�е�����Ԫ�����
    ʹ�����Ա�ص�����ʱ�ĳ�ʼ״̬
*/
void SeqList_Clear(SeqList* list);

/*
    �÷������ڷ���һ�����Ա�list�е�����Ԫ�ظ���
*/
int SeqList_Length(SeqList* list);

/*
	�÷������ڷ������Ա�list�еĵ�ǰԪ��λ�� 
*/
int SeqList_Capacity(SeqList* list); 

/*
    �÷���������һ�����Ա�list��posλ�ô�������Ԫ��node
    ����ֵΪ1��ʾ����ɹ���0��ʾ����ʧ��
*/
int SeqList_Insert(SeqList* list, SeqListNode node, int pos);

/*
    �÷������ڻ�ȡһ�����Ա�list��posλ�ô���Ԫ��
    ����ֵΪposλ�ô���Ԫ�أ�NULL��ʾ��ȡʧ��
*/
SeqListNode SeqList_Get(SeqList* list, int pos);

/*
    �÷�������ɾ��һ�����Ա�list��posλ�ô���Ԫ��
    ����ֵΪ��ɾ����Ԫ�أ�NULL��ʾɾ��ʧ��
*/
SeqListNode SeqList_Delete(SeqList* list, int pos);

#endif



