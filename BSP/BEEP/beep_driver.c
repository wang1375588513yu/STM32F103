/********************************Copyright (c)**********************************\
**
**                   (c) Copyright 2018, WangYu, China, SZ.
**                           All Rights Reserved
**
**                           By(深圳市北斗云信息科技有限公司)
**
**----------------------------------文件信息------------------------------------
** 文件名称: beep_driver.c
** 创建人员: WangYu
** 创建日期: 
** 创建时间：
** 文档描述: 
**
**----------------------------------版本信息------------------------------------
** 版本代号: V1.0
** 版本说明: 初始版本
**
**------------------------------------------------------------------------------
\********************************End of Head************************************/

#include "beep_driver.h"
#include "stm32f10x.h" 
#include "SysTick.h"

#if 0
void Beep_InitConfig(void)
{	
	
//定义一个GPIO_InitTypeDef类型的结构体
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
// 	u16 CCR1_Val = 32;  // PWM信号电平跳变值 
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE); 

	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE); //Timer1完全重映射  TIM3_CH4->PC9
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	// Time base configuration 	 
	TIM_TimeBaseStructure.TIM_Period = 255;       //当定时器从0计数到255，即为256次，为一个定时周期
	TIM_TimeBaseStructure.TIM_Prescaler = 1999;	    //设置预分频：2000预分频，即为36KHz
	TIM_TimeBaseStructure.TIM_ClockDivision = 0 ;	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	// PWM1 Mode configuration: Channe1 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //配置为PWM模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
	TIM_OCInitStructure.TIM_Pulse = 0;	   //设置跳变值，当计数器计数到这个值时，电平发生跳变
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //当定时器计数值小于CCR1_Val时为高电平
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;//当定时器计数值小于CCR1_Val时,输出有效电平
	
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);	 //使能通道1
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
		
	TIM_Cmd(TIM3, ENABLE);                   //使能定时器3

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
** 函数名称: Beep_InitConfig
** 功能描述: 
** 参数说明: None
** 返回说明: None
** 创建人员: WangYu
** 创建日期: 2018-08-22
**------------------------------------------------------------------------------
** 修改人员:
** 修改日期:
** 修改描述:
**------------------------------------------------------------------------------
********************************************************************************/
void Beep_InitConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE); 
	
	//蜂鸣器控制引脚
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
void Beep_On(void)		//蜂鸣器开
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
void Beep_Off(void)		//蜂鸣器关
{
	BEEP_Contrl(0);
}

void Beep_hold(void)	//蜂鸣器以10ms间隔的开和关
{
	Beep_On();
	delay_us(10);
	Beep_Off();
	delay_ms(10);
	
}

#endif

