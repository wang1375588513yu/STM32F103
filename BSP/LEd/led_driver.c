/********************************Copyright (c)**********************************\
**
**                   (c) Copyright 2018, WangYu, China, SZ.
**                           All Rights Reserved
**
**                           By(�����б�������Ϣ�Ƽ����޹�˾)
**
**----------------------------------�ļ���Ϣ------------------------------------
** �ļ�����: led_driver.c
** ������Ա: WangYu
** ��������: 2018-08-22 
** �ĵ�����: 
**
**----------------------------------�汾��Ϣ------------------------------------
** �汾����: V1.0
** �汾˵��: ��ʼ�汾
**
**------------------------------------------------------------------------------
\********************************End of Head************************************/

#include "led_driver.h"

//��ʼ��
void Led_InitConfig(void)
{

	GPIO_InitTypeDef GPIO_InitStruct;
	
	//��pc��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;//ѡ��6.7.8��
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//��ת����Ϊ
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;//��©ģʽ
	
	
	
	//��ʼ��pc678��Ϊ�ߵ�ƽ
	GPIO_SetBits(GPIOC,GPIO_Pin_6);
	GPIO_SetBits(GPIOC,GPIO_Pin_7);
	GPIO_SetBits(GPIOC,GPIO_Pin_8);
	
	GPIO_Init(GPIOC,&GPIO_InitStruct);//���ó�ʼ������
	//GPIOC->ODR |= 0x07 << 6;//6 7 8 �����1
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


