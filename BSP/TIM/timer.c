#include "timer.h"
#include "stm32f10x.h"


void Tim3_Init(void)
{
	TIM_TimeBaseInitTypeDef    TIM_TimeBaseInitSture;
	NVIC_InitTypeDef           NVIC_InitSture;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//ʹ��TIM3
	
	//��ʼ����ʱ��
	TIM_TimeBaseInitSture.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitSture.TIM_Period=7200;
	TIM_TimeBaseInitSture.TIM_Prescaler=0;
	TIM_TimeBaseInitSture.TIM_ClockDivision=0;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitSture);
	
	//��������жϣ�������ʽ�ж�
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	TIM_ITConfig(TIM3,TIM_IT_Trigger,ENABLE);
	
	//�ж����ȼ�����
	NVIC_InitSture.NVIC_IRQChannel=TIM3_IRQn;
	NVIC_InitSture.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitSture.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitSture.NVIC_IRQChannelSubPriority=3;
	NVIC_Init(&NVIC_InitSture);
	
	TIM_Cmd(TIM3,ENABLE);
}

void TIM2_Configuration(void) 
{ 
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct; 
	TIM_ICInitTypeDef TIM_ICInitStructure;

	TIM_ICInitStructure.TIM_Channel=TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;//�����ز���
	TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;//ӳ�䵽TI2�� 
	TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;//�ڲ���������ÿ̽�⵽һ������ִ��һ�β���
	TIM_ICInitStructure.TIM_ICFilter=0;//�˲����ã������������������϶������ȶ�(��������ߵ�ƽ��ֻ�������ɼ���N����ƽ���Ǹߵ�ƽ����Ч��)
	TIM_PWMIConfig(TIM2,&TIM_ICInitStructure);//���������벶������
	
	TIM_SelectInputTrigger(TIM2,TIM_TS_TI2FP2);//ѡ���˲����TI2FP2������Ϊ����Դ����������ĳ���ĸ�λ
	TIM_SelectSlaveMode(TIM2,TIM_SlaveMode_Reset);//
	TIM_SelectMasterSlaveMode(TIM2,TIM_MasterSlaveMode_Enable);//������ʱ���ı�������
	TIM_ITConfig(TIM2,TIM_IT_CC2,ENABLE);//�����жϴ�
	TIM_ClearFlag(TIM2,TIM_IT_CC2);//�����λ��־
	TIM_Cmd(TIM2,ENABLE);//ʹ�ܶ�ʱ��2
}


