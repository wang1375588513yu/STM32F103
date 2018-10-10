#include "infraed.h"
#include "stm32f10x.h" 
#include "led_driver.h"
#include "beep_driver.h"
#include "SysTick.h"

#define infraed_read() GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5)

void Infraed_InitConfig(void)
{
	//��ʼ��PB5����
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init( GPIOB, &GPIO_InitStructure);
	
	//GPIO_ResetBits(GPIOB,GPIO_Pin_5);//������״̬���ڵ͵�ƽ���״̬
	
}

void Inf_Ctl(void)
{
	if(infraed_read())
	{//������Ϊ�ߵ�ƽ�����⵽��������ź�
		Led_Ctl(LED_RED,ON);
//		Beep_On();
	}
	else
	{
		Led_Ctl(LED_RED,OFF);
//		Beep_Off();
	}
}


