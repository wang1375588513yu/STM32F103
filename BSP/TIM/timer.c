#include "timer.h"
#include "stm32f10x.h"


void Tim3_Init(void)
{
	TIM_TimeBaseInitTypeDef    TIM_TimeBaseInitSture;
	NVIC_InitTypeDef           NVIC_InitSture;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//使能TIM3
	
	//初始化定时器
	TIM_TimeBaseInitSture.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitSture.TIM_Period=7200;
	TIM_TimeBaseInitSture.TIM_Prescaler=0;
	TIM_TimeBaseInitSture.TIM_ClockDivision=0;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitSture);
	
	//允许更新中断，触发方式中断
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	TIM_ITConfig(TIM3,TIM_IT_Trigger,ENABLE);
	
	//中断优先级管理
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
	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;//上升沿捕获
	TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;//映射到TI2上 
	TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;//在捕获输入上每探测到一个边沿执行一次捕获
	TIM_ICInitStructure.TIM_ICFilter=0;//滤波设置，经历几个周期跳变认定波形稳定(比如采样高电平，只有连续采集到N个电平都是高电平才有效，)
	TIM_PWMIConfig(TIM2,&TIM_ICInitStructure);//以上是输入捕获设置
	
	TIM_SelectInputTrigger(TIM2,TIM_TS_TI2FP2);//选择滤波后的TI2FP2输入作为触发源，触发下面的程序的复位
	TIM_SelectSlaveMode(TIM2,TIM_SlaveMode_Reset);//
	TIM_SelectMasterSlaveMode(TIM2,TIM_MasterSlaveMode_Enable);//启动定时器的被动触发
	TIM_ITConfig(TIM2,TIM_IT_CC2,ENABLE);//捕获中断打开
	TIM_ClearFlag(TIM2,TIM_IT_CC2);//清除复位标志
	TIM_Cmd(TIM2,ENABLE);//使能定时器2
}


