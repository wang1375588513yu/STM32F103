/*******************************************************************
 * ********************������ģ�飨HC-SR04��************************
 *(1)����IO ������࣬������10us �ĸߵ�ƽ�ź�;
 *(2)ģ���Զ�����8 ��40khz �ķ������Զ�����Ƿ����źŷ��أ�
 *(3)���źŷ��أ�ͨ��IO ���һ�ߵ�ƽ���ߵ�ƽ������ʱ�����
 *(4)�������ӷ��䵽���ص�ʱ�䣮���Ծ���=(�ߵ�ƽʱ��*����(340M/S))/2;
 *******************************************************************/


#include "hc_sr04.h"
#include "stm32f10x.h" 
#include "led_driver.h"
#include "beep_driver.h"
#include "usart_driver.h"
#include "SysTick.h"

#define GPIO_PORT_SR04	GPIOB	//pb3��pb4�˿�
#define RCC_SR04_PORT   RCC_APB2Periph_GPIOB	//ʱ��
#define SR04_ECHO_PIN 	GPIO_Pin_4
#define SR04_TRIG_PIN 	GPIO_Pin_3

/*����TRIG�˿ڵĸߵ͵�ƽ*/
#define SR04_TRIG_0()	GPIO_ResetBits(GPIO_PORT_SR04, SR04_TRIG_PIN)
#define SR04_TRIG_1()	GPIO_SetBits(GPIO_PORT_SR04, SR04_TRIG_PIN)

/*���echo�˿ڵĸߵ͵�ƽ*/
#define SR04_ECHO_READ() GPIO_ReadInputDataBit(GPIO_PORT_SR04, SR04_ECHO_PIN)

float Distance; //���Եľ�����
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
	//�������ȼ�����
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//TIM3ʱ��
	//��ʼ����ʱ��
	TIM_TimeBaseInitSture.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitSture.TIM_Period=5000;
	TIM_TimeBaseInitSture.TIM_Prescaler=7199;
	TIM_TimeBaseInitSture.TIM_ClockDivision=0;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitSture);
	
	//��������жϣ�������ʽ�ж�
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	TIM_ITConfig(TIM3,TIM_IT_Trigger,ENABLE);

#if 0	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI1_IRQn;//ѡ���ж�ͨ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;//��ռʽ���ȼ�����
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;//��Ӧʽ���ȼ�����
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//ʹ��
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
	//��ʼ���ⲿ�ж�EXTI0  EXTI1  EXTI2  EXTI3
	EXTI_ClearITPendingBit(EXTI_Line4);  
//    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource5);  
	EXTI_InitStruct.EXTI_Line = EXTI_Line4;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;//�ж�ģʽ
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;//�����ش����ж�
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
	/*��ʼ��PC10��PC11����GPIO��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_SR04_PORT,ENABLE); 
	
	GPIO_InitStructure.GPIO_Pin = SR04_TRIG_PIN ;//PB3
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	
	GPIO_InitStructure.GPIO_Pin = SR04_ECHO_PIN ;//PB4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	
	GPIO_Init(GPIO_PORT_SR04, &GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource4);
	SR04_TRIG_0();        //������SR04_TRIG_PIN����Ϊ�͵�ƽ

	Distance = 0.0;  
}
/***************************************
 **SR04_Ctrl
 ****************************************/
void SR04_Ctrl(void)
{	//ģ�鴥������ 
	SR04_TRIG_1();//�����źţ��ߵ�ƽ10us���ϣ�
	delay_us(20);//����10us�������20us
	SR04_TRIG_0();//���ͻص��͵�ƽ
}

#if 1
void Distance_Show(void)
{
	SR04_Ctrl();
	
	while(!SR04_ECHO_READ());
	TIM_SetCounter(TIM3,0);	//����������
	TIM_Cmd(TIM3,ENABLE);	//��ʱ��ʹ�ܣ� ��ʼ��ʱ
	Led_Ctl(LED_BLUE,ON);
	
	while(SR04_ECHO_READ());
	TIM_Cmd(TIM3,DISABLE);	//��ʱ��ʧ�ܣ�ֹͣ��ʱ
//	Led_Ctl(LED_BLUE,OFF);
	Distance=TIM_GetCounter(TIM3)*340/200.0;//
		
	if(Distance>0)
	{
		printf("Distance:%3f cm\r\n",Distance);	//�����������pc
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
		TIM_SetCounter(TIM3,0);	//����������
		TIM_Cmd(TIM3,ENABLE);	//��ʱ��ʹ�ܣ� ��ʼ��ʱ
		
		
		while(SR04_ECHO_READ());  //�ȴ��͵�ƽ
		
		TIM_Cmd(TIM3,DISABLE);	//��ʱ��ʧ�ܣ�ֹͣ��ʱ
//		Led_Ctl(LED_GREEN,OFF);
		
		Distance=TIM_GetCounter(TIM3)*340/200.0;//
		
		if(Distance>0)
		{
			if(Distance <= 300)
			{
				printf("Distance:%3f cm\r\n",Distance);	//�����������pc
			}
			else
			{
				printf("Distance:300.000cm\r\n");	//�����������pc
			}
			
		}
		
		EXTI_ClearITPendingBit(EXTI_Line4);		//���EXTI��·����λ
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
    TimingDelay_Decrement();  //�ж�����ô˺�������û����һ���жϣ��˺��������ã��˺�����ı���TimingDelay �൱�ڼ���������  
}  
*/
///********��ʱ��3*******/
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
		TIM_SetCounter(TIM3,0);	//����������
		TIM_Cmd(TIM3,ENABLE);	//��ʱ��ʹ�ܣ� ��ʼ��ʱ
		Led_Ctl(LED_GREEN,ON);
		
		while(!SR04_ECHO_READ());  //�ȴ��͵�ƽ
		
		TIM_Cmd(TIM3,DISABLE);	//��ʱ��ʧ�ܣ�ֹͣ��ʱ
		Led_Ctl(LED_GREEN,ON);
		
		Distance=TIM_GetCounter(TIM3)*340/200.0;//
		
		if(Distance>0)
		{
			printf("Distance:%f cm\r\n",Distance);	//�����������pc
		}
			
		EXTI_ClearITPendingBit(EXTI_Line3);		//���EXTI��·����λ
	}
}  

*/

void SR04_InitConfig(void)
{
	Sr04_Config();	//������GPIO��ʼ��
	TIM3_Init();	//��ʱ��3��ʼ��
//	EXTI_init();	//�жϳ�ʼ��
}





