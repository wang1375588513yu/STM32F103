/*******************************************************************
 * ********************超声波模块（HC-SR04）************************
 *(1)采用IO 触发测距，给至少10us 的高电平信号;
 *(2)模块自动发送8 个40khz 的方波，自动检测是否有信号返回；
 *(3)有信号返回，通过IO 输出一高电平，高电平持续的时间就是
 *(4)超声波从发射到返回的时间．测试距离=(高电平时间*声速(340M/S))/2;
 *******************************************************************/


#include "hc_sr04.h"
#include "stm32f10x.h" 
#include "led_driver.h"
#include "beep_driver.h"
#include "usart_driver.h"
#include "SysTick.h"

#define GPIO_PORT_SR04	GPIOB	//pb3，pb4端口
#define RCC_SR04_PORT   RCC_APB2Periph_GPIOB	//时钟
#define SR04_ECHO_PIN 	GPIO_Pin_4
#define SR04_TRIG_PIN 	GPIO_Pin_3

/*设置TRIG端口的高低电平*/
#define SR04_TRIG_0()	GPIO_ResetBits(GPIO_PORT_SR04, SR04_TRIG_PIN)
#define SR04_TRIG_1()	GPIO_SetBits(GPIO_PORT_SR04, SR04_TRIG_PIN)

/*检测echo端口的高低电平*/
#define SR04_ECHO_READ() GPIO_ReadInputDataBit(GPIO_PORT_SR04, SR04_ECHO_PIN)

float Distance; //测试的距离结果
uint8_t   flag = 1;  
uint32_t __IO time_1ms = 0;  
uint32_t __IO time_120ms = 0;
static uint32_t __IO TimingDelay = 0;   


/***************************************
 **TIM3_Init
 ****************************************/
void TIM3_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitSture;
	//NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitTypeDef NVIC_InitStructure; 
	//设置优先级分组
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//TIM3时钟
	//初始化定时器
	TIM_TimeBaseInitSture.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitSture.TIM_Period=5000;
	TIM_TimeBaseInitSture.TIM_Prescaler=7199;
	TIM_TimeBaseInitSture.TIM_ClockDivision=0;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitSture);
	
	//允许更新中断，触发方式中断
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	TIM_ITConfig(TIM3,TIM_IT_Trigger,ENABLE);

#if 0	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI1_IRQn;//选择中断通道
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;//抢占式优先级设置
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;//响应式优先级设置
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//使能
	NVIC_Init(&NVIC_InitStruct);
#else
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif 

	Led_Ctl(LED_RED,ON);
	delay_ms(1000);
	Led_Ctl(LED_RED,OFF);
	
	TIM_Cmd(TIM3,ENABLE);
	
}

/***************************************
 **EXTI_init
 ****************************************/
void EXTI_init(void)
{
	EXTI_InitTypeDef EXTI_InitStruct;
	//初始化外部中断EXTI0  EXTI1  EXTI2  EXTI3
	EXTI_ClearITPendingBit(EXTI_Line4);  
//    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource5);  
	EXTI_InitStruct.EXTI_Line = EXTI_Line4;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;//中断模式
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;//上升沿触发中断
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	
	Led_Ctl(LED_BLUE,ON);
	delay_ms(1000);
	Led_Ctl(LED_BLUE,OFF);

}

/***************************************
 **SR04_Config
 ****************************************/
void Sr04_Config(void)
{
	/*初始化PC10，PC11两个GPIO口*/
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_SR04_PORT,ENABLE); 
	
	GPIO_InitStructure.GPIO_Pin = SR04_TRIG_PIN ;//PB3
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	
	GPIO_InitStructure.GPIO_Pin = SR04_ECHO_PIN ;//PB4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	
	GPIO_Init(GPIO_PORT_SR04, &GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource4);
	SR04_TRIG_0();        //先设置SR04_TRIG_PIN引脚为低电平

	Distance = 0.0;  
}
/***************************************
 **SR04_Ctrl
 ****************************************/
void SR04_Ctrl(void)
{	//模块触发过程 
	SR04_TRIG_1();//触发信号（高电平10us以上）
	delay_us(20);//至少10us这里给定20us
	SR04_TRIG_0();//拉低回到低电平
}

#if 1
void Distance_Show(void)
{
	SR04_Ctrl();
	
	while(!SR04_ECHO_READ());
	TIM_SetCounter(TIM3,0);	//计数器清零
	TIM_Cmd(TIM3,ENABLE);	//定时器使能， 开始计时
	Led_Ctl(LED_BLUE,ON);
	
	while(SR04_ECHO_READ());
	TIM_Cmd(TIM3,DISABLE);	//定时器失能，停止计时
//	Led_Ctl(LED_BLUE,OFF);
	Distance=TIM_GetCounter(TIM3)*340/200.0;//
		
	if(Distance>0)
	{
		printf("Distance:%3f cm\r\n",Distance);	//输出计算结果到pc
	}
}

#endif

#if 0
/***************************************
 **EXTI1_IRQHandler
 ****************************************/

void EXTI4_IRQHandler(void)  
{  
	if(EXTI_GetITStatus(EXTI_Line4)!=RESET)
	{
		Led_Ctl(LED_GREEN,ON);
		TIM_SetCounter(TIM3,0);	//计数器清零
		TIM_Cmd(TIM3,ENABLE);	//定时器使能， 开始计时
		
		
		while(SR04_ECHO_READ());  //等待低电平
		
		TIM_Cmd(TIM3,DISABLE);	//定时器失能，停止计时
//		Led_Ctl(LED_GREEN,OFF);
		
		Distance=TIM_GetCounter(TIM3)*340/200.0;//
		
		if(Distance>0)
		{
			if(Distance <= 300)
			{
				printf("Distance:%3f cm\r\n",Distance);	//输出计算结果到pc
			}
			else
			{
				printf("Distance:300.000cm\r\n");	//输出计算结果到pc
			}
			
		}
		
		EXTI_ClearITPendingBit(EXTI_Line4);		//清除EXTI线路挂起位
	}
}  
#endif 
/*

void TimingDelay_Decrement(void)
{
	if(TimingDelay != 0x00)
	{
		TimingDelay--;
	}
}

void SysTick_Handler(void)  
{  
    time_1ms++;  
    time_120ms++;  
    if(time_120ms>=80)  
    {  
        SR04_Ctrl();//50ms Trig Sonic  
		
        time_120ms=0;  
    }  
    TimingDelay_Decrement();  //中断里调用此函数，即没发生一次中断，此函数被调用，此函数里的变量TimingDelay 相当于减法计数器  
}  
*/
///********定时器3*******/
//void TIM3_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET)
//	{
//		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
//	}
//}

/***************************************
 **EXTI3_IRQHandler
 ****************************************/
/*
void EXTI3_IRQHandler(void)  
{  
	if(EXTI_GetITStatus(EXTI_Line3)!=RESET)
	{
		TIM_SetCounter(TIM3,0);	//计数器清零
		TIM_Cmd(TIM3,ENABLE);	//定时器使能， 开始计时
		Led_Ctl(LED_GREEN,ON);
		
		while(!SR04_ECHO_READ());  //等待低电平
		
		TIM_Cmd(TIM3,DISABLE);	//定时器失能，停止计时
		Led_Ctl(LED_GREEN,ON);
		
		Distance=TIM_GetCounter(TIM3)*340/200.0;//
		
		if(Distance>0)
		{
			printf("Distance:%f cm\r\n",Distance);	//输出计算结果到pc
		}
			
		EXTI_ClearITPendingBit(EXTI_Line3);		//清除EXTI线路挂起位
	}
}  

*/

void SR04_InitConfig(void)
{
	Sr04_Config();	//超声波GPIO初始化
	TIM3_Init();	//定时器3初始化
//	EXTI_init();	//中断初始化
}





