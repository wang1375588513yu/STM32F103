/********************************Copyright (c)**********************************\
**
**                   (c) Copyright 2018, WangYu, China, SZ.
**                           All Rights Reserved
**
**                        By(深圳市北斗云信息科技有限公司)
**
**----------------------------------文件信息------------------------------------
** 文件名称: led_driver.h
** 创建人员: WangYu
** 创建日期: 2018-08-22
** 文档描述: 
**
**----------------------------------版本信息------------------------------------
** 版本代号: V1.0
** 版本说明: 初始版本
**
**------------------------------------------------------------------------------
\********************************End2018-08-22 13:23 of Head************************************/

#ifndef _LED_DRIVER_H_
#define _LED_DRIVER_H_

#include "stm32f10x.h"   


#define ON  0
#define OFF 1

typedef enum
{
	LED_RED = 0,
	LED_GREEN,
	LED_BLUE,
}__LED_TYPE;


void Led_InitConfig(void);       
void Led_Ctl(__LED_TYPE led_type, uint8_t status);

#endif
