/********************************Copyright (c)**********************************\
**
**                   (c) Copyright 2018, WangYu, China, SZ.
**                           All Rights Reserved
**
**                           By(�����б�������Ϣ�Ƽ����޹�˾)
**
**----------------------------------�ļ���Ϣ------------------------------------
** �ļ�����: beep_driver.c
** ������Ա: WangYu
** ��������: 
** ����ʱ�䣺
** �ĵ�����: 
**
**----------------------------------�汾��Ϣ------------------------------------
** �汾����: V1.0
** �汾˵��: ��ʼ�汾
**
**------------------------------------------------------------------------------
\********************************End of Head************************************/

#include "beep_driver.h"
#include "stm32f10x.h" 
#include "SysTick.h"

#if 0
void Beep_InitConfig(void)
{	
	
//����һ��GPIO_InitTypeDef���͵Ľṹ��
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
// 	u16 CCR1_Val = 32;  // PWM�źŵ�ƽ����ֵ 
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE); 

	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE); //Timer1��ȫ��ӳ��  TIM3_CH4->PC9
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	// Time base configuration 	 
	TIM_TimeBaseStructure.TIM_Period = 255;       //����ʱ����0������255����Ϊ256�Σ�Ϊһ����ʱ����
	TIM_TimeBaseStructure.TIM_Prescaler = 1999;	    //����Ԥ��Ƶ��2000Ԥ��Ƶ����Ϊ36KHz
	TIM_TimeBaseStructure.TIM_ClockDivision = 0 ;	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	// PWM1 Mode configuration: Channe1 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //����ΪPWMģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
	TIM_OCInitStructure.TIM_Pulse = 0;	   //��������ֵ�������������������ֵʱ����ƽ��������
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;//����ʱ������ֵС��CCR1_Valʱ,�����Ч��ƽ
	
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);	 //ʹ��ͨ��1
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
		
	TIM_Cmd(TIM3, ENABLE);                   //ʹ�ܶ�ʱ��3

}

static void PWM_Channel4_Out(u16 pwm)
{
	TIM3->CCR4 =  pwm;
}

void Beep_Contrl(u16 pwm)
{
	PWM_Channel4_Out(pwm);
}

#else

/*******************************************************************************
** ��������: Beep_InitConfig
** ��������: 
** ����˵��: None
** ����˵��: None
** ������Ա: WangYu
** ��������: 2018-08-22
**------------------------------------------------------------------------------
** �޸���Ա:
** �޸�����:
** �޸�����:
**------------------------------------------------------------------------------
********************************************************************************/
void Beep_InitConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE); 
	
	//��������������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_SetBits(GPIOC, GPIO_Pin_9);
}

void BEEP_Contrl(u8 status)
{
	if(status)
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_9);
	}
	else
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_9);
	}
}
void Beep_On(void)		//��������
{
	u8 i = 0;

	for(i = 0; i < 100; i++)
	{					
		BEEP_Contrl(0);
		delay_us(400);
		BEEP_Contrl(1);
		delay_us(400);
	}
}
void Beep_Off(void)		//��������
{
	BEEP_Contrl(0);
}

void Beep_hold(void)	//��������10ms����Ŀ��͹�
{
	Beep_On();
	delay_us(10);
	Beep_Off();
	delay_ms(10);
	
}

#endif

