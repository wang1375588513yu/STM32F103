#ifndef _KEY_DRIVER_H_
#define _KEY_DRIVER_H_

#include "led_driver.h"
#include "SysTick.h"
#include "stm32f10x.h"
#include "beep_driver.h"
#include "usart_driver.h"


typedef enum{
	KEY_1=0,
	KEY_2,
	NoKey=0xff,
}_KEY_TYPE;

void Init(void);
void Key_InitConfig(void);
void Key_Scan(_KEY_TYPE _key_type);
_KEY_TYPE get_KeyType(void);

#endif

