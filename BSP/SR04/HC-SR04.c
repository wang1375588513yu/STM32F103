

#include "HC-SR04.h"
#include "Led_driver.h"
#include "stm32f10x_it.h"


void SR04_Cconfig_init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStruct1;
	GPIO_InitTypeDef GPIO_InitStruct2;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC| RCC_APB2Periph_AFIO,ENABLE);
	GPIO_InitStruct1.GPIO_Pin = SR04_TRIG;
	GPIO_InitStruct1.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct1.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_X, &GPIO_InitStruct1);
	GPIO_ResetBits(GPIO_X,SR04_TRIG);
	GPIO_InitStruct2.GPIO_Pin = SP04_ECH0;
	GPIO_InitStruct2.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIO_Y, &GPIO_InitStruct2);
	GPIO_ResetBits(GPIO_Y,SP04_ECH0);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource4);
}
#if 1
void EXIT_config(void)
{
	EXTI_InitTypeDef  EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line4;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);		
}

void NVIC_config_init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
    
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
#endif
void TIM3_Init(void)
{

	TIM_TimeBaseInitTypeDef    TIM_TimeBaseInitSture;
//	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	TIM_TimeBaseInitSture.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitSture.TIM_Period=5000;
	TIM_TimeBaseInitSture.TIM_Prescaler=7199;
	TIM_TimeBaseInitSture.TIM_ClockDivision=0;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitSture);
	

	TIM_Cmd(TIM3,ENABLE);
	
}


void Run_SR04(void)
{	
	delay_ms(1000);
	GPIO_SetBits(GPIO_X,SR04_TRIG);
	delay_us(20);
	GPIO_ResetBits(GPIO_X,SR04_TRIG);
}



void HC_init(void)
{
	SR04_Cconfig_init();
	EXIT_config();
	NVIC_config_init();
	TIM3_Init();

}
