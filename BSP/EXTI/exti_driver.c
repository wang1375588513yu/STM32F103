#include "exti_driver.h"
#include "stm32f10x.h"
#include "led_driver.h"



#if 1
static void Exti_Config(void)
{
	EXTI_InitTypeDef EXTI_InitStruct;
	
	//��ʼ���ⲿ�ж�EXTI0  EXTI1  EXTI2  EXTI3
	EXTI_InitStruct.EXTI_Line = EXTI_Line0|EXTI_Line1|EXTI_Line2|EXTI_Line3;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;//�ж�ģʽ
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;//�����ش����ж�
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
}

#else

static void Exti_Config(void)
{	
	//��ʼ���ⲿ�ж�EXTI0  EXTI1  EXTI2  EXTI3
	EXTI->IMR |= 0x0f;
	EXTI->RTSR |= 0x0f;	//�����ش���
//	EXTI->FTSR |= 0x0f;	//�½��ش���
}

#endif



static void Exti_NVIC_InitConfig(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	//�������ȼ�����
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
#ifdef EXTI0
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;//ѡ���ж�ͨ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;//��ռʽ���ȼ�����
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;//��Ӧʽ���ȼ�����
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//ʹ��
	NVIC_Init(&NVIC_InitStruct);
#endif 
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI1_IRQn;//ѡ���ж�ͨ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;//��ռʽ���ȼ�����
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;//��Ӧʽ���ȼ�����
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//ʹ��
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI2_IRQn;//ѡ���ж�ͨ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;//��ռʽ���ȼ�����
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;//��Ӧʽ���ȼ�����
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//ʹ��
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI3_IRQn;//ѡ���ж�ͨ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;//��ռʽ���ȼ�����
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;//��Ӧʽ���ȼ�����
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//ʹ��
	NVIC_Init(&NVIC_InitStruct);
}



void Exti_InitConfig(void)
{
	Exti_Config();
	Exti_NVIC_InitConfig();
	//Led_Ctl(LED_GREEN,ON);
}
/*
void EXTI3_IRQHandler(void)
{
	
}*/

