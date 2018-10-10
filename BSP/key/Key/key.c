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
	
	//1.打开GPIOA口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//初始化PA0  1 2 3口为浮空输入或者上拉输入
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
	//设置优先级分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	//初始化外部中断EXTI0  EXTI1  EXTI2  EXTI3
	EXTI_InitStruct.EXTI_Line =	EXTI_Line0;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;//中断模式
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;//上升沿触发中断
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);

	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;//选择中断通道
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;//抢占式优先级设置
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;//响应式优先级设置
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//使能
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
		//延时一段时间
		delay_ms(20);
		//第二次读取I/O口电平
		if(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0))
		{
			//等待按键松开
			while(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0));
			
			//返回键值
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

