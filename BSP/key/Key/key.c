#include "key.h"
#include "led_driver.h"
#include "SysTick.h"
#include "stm32f10x.h"
#include "beep_driver.h"
#include "usart_driver.h"


static int flag = 1;

void Key_InitConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//1.��GPIOA��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//��ʼ��PA0  1 2 3��Ϊ�������������������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}
void EXTI_Config(void)
{
	EXTI_InitTypeDef EXTI_InitStruct;
	
	NVIC_InitTypeDef NVIC_InitStruct;
	//�������ȼ�����
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	//��ʼ���ⲿ�ж�EXTI0  EXTI1  EXTI2  EXTI3
	EXTI_InitStruct.EXTI_Line =	EXTI_Line0;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;//�ж�ģʽ
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;//�����ش����ж�
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);

	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;//ѡ���ж�ͨ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;//��ռʽ���ȼ�����
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;//��Ӧʽ���ȼ�����
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//ʹ��
	NVIC_Init(&NVIC_InitStruct);
}

void Init(void)
{
	Key_InitConfig();
	EXTI_Config();
}

_KEY_TYPE get_KeyType(void)
{
//	uint32_t aa;
//	//Led_Ctl(LED_BLUE,ON);
//	aa = GPIOA->IDR;

	//GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0);
#if 0
	if(!(aa && 0x01))
	{
		delay_ms(20);
		if(!(aa && 0x01))
		{
			while(!(aa && 0x01))
				;
			return KEY_1;
		}
	}
#else
	if(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0))	//KEY1
	{
		//��ʱһ��ʱ��
		delay_ms(20);
		//�ڶ��ζ�ȡI/O�ڵ�ƽ
		if(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0))
		{
			//�ȴ������ɿ�
			while(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0));
			
			//���ؼ�ֵ
			return KEY_1;
		}
	}
	#endif
	return NoKey;
}

void Key_Scan(_KEY_TYPE _key_type)
{
	//int flag = 1;
	switch((uint8_t)_key_type)
	{
		case KEY_1:
				printf("wocao\r\n");
		    //delay_ms(1000);
			if(flag)
			{
				Led_Ctl(LED_BLUE,ON);
				flag = 0;
				delay_ms(1000);
			}else{
				Led_Ctl(LED_BLUE,OFF);
				flag = 1;
				delay_ms(1000);
			}
			break;
	}
}

