#include "exti_driver.h"
#include "stm32f10x.h"
#include "led_driver.h"



#if 1
static void Exti_Config(void)
{
	EXTI_InitTypeDef EXTI_InitStruct;
	
	//初始化外部中断EXTI0  EXTI1  EXTI2  EXTI3
	EXTI_InitStruct.EXTI_Line = EXTI_Line0|EXTI_Line1|EXTI_Line2|EXTI_Line3;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;//中断模式
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;//上升沿触发中断
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
}

#else

static void Exti_Config(void)
{	
	//初始化外部中断EXTI0  EXTI1  EXTI2  EXTI3
	EXTI->IMR |= 0x0f;
	EXTI->RTSR |= 0x0f;	//上升沿触发
//	EXTI->FTSR |= 0x0f;	//下降沿触发
}

#endif



static void Exti_NVIC_InitConfig(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	//设置优先级分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
#ifdef EXTI0
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;//选择中断通道
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;//抢占式优先级设置
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;//响应式优先级设置
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//使能
	NVIC_Init(&NVIC_InitStruct);
#endif 
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI1_IRQn;//选择中断通道
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;//抢占式优先级设置
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;//响应式优先级设置
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//使能
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI2_IRQn;//选择中断通道
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;//抢占式优先级设置
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;//响应式优先级设置
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//使能
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI3_IRQn;//选择中断通道
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;//抢占式优先级设置
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;//响应式优先级设置
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//使能
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

