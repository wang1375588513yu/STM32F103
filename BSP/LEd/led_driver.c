/********************************Copyright (c)**********************************\
**
**                   (c) Copyright 2018, WangYu, China, SZ.
**                           All Rights Reserved
**
**                           By(深圳市北斗云信息科技有限公司)
**
**----------------------------------文件信息------------------------------------
** 文件名称: led_driver.c
** 创建人员: WangYu
** 创建日期: 2018-08-22 
** 文档描述: 
**
**----------------------------------版本信息------------------------------------
** 版本代号: V1.0
** 版本说明: 初始版本
**
**------------------------------------------------------------------------------
\********************************End of Head************************************/

#include "led_driver.h"

//初始化
void Led_InitConfig(void)
{

	GPIO_InitTypeDef GPIO_InitStruct;
	
	//打开pc口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;//选择6.7.8口
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//翻转速率为
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;//开漏模式
	
	
	
	//初始化pc678口为高电平
	GPIO_SetBits(GPIOC,GPIO_Pin_6);
	GPIO_SetBits(GPIOC,GPIO_Pin_7);
	GPIO_SetBits(GPIOC,GPIO_Pin_8);
	
	GPIO_Init(GPIOC,&GPIO_InitStruct);//调用初始化函数
	//GPIOC->ODR |= 0x07 << 6;//6 7 8 口输出1
}


void Led_Ctl(__LED_TYPE led_type,uint8_t status)
{
	switch((uint8_t)led_type)
	{
		case LED_RED:
			if(status)	//OFF
			{
				GPIO_SetBits(GPIOC, GPIO_Pin_8);
			}
			else	//ON
			{
				GPIO_ResetBits(GPIOC, GPIO_Pin_8);
			}
			break;
		case LED_GREEN:
			if(status)	//OFF
			{
				GPIO_SetBits(GPIOC, GPIO_Pin_7);
			}
			else	//ON
			{
				GPIO_ResetBits(GPIOC, GPIO_Pin_7);
			}
			break;
		case LED_BLUE:
			if(status)	//OFF
			{
				GPIO_SetBits(GPIOC, GPIO_Pin_6);
			}
			else	//ON
			{
				GPIO_ResetBits(GPIOC, GPIO_Pin_6);
			}
			break;
	}
}

/*
void Led_Ctl(__LED_TYPE led_type,uint8_t status)
{
	switch((uint8_t)led_type)
	{
		case LED_RED:
			if(status)	//OFF
			{
				GPIOC->ODR |= 0x01<<8;
			}
			else	//ON
			{
				GPIOC->ODR &= ~(0x01<<8);
			}
			break;
		case LED_GREEN:
			if(status)	//OFF
			{
				GPIOC->BSRR |= 0x01<<7;
			}
			else	//ON
			{
				GPIOC->BSRR |= 0x01<<23;
			}
			break;
		case LED_BLUE:
			if(status)	//OFF
			{
				GPIO_SetBits(GPIOC, GPIO_Pin_6);
			}
			else	//ON
			{
				GPIOC->BRR |= 0x01<<6;
			}
			break;
	}
}
*/


