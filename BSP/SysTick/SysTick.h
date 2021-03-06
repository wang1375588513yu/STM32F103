#ifndef _SYSTICK_H_
#define _SYSTICK_H_

#include "stm32f10x.h"
#include "board_config.h"

#ifdef DEVICE_SYSTICK

void SysTick_init(u8 SYSCLK);
void delay_ms(u16 nms);
void delay_us(u32 nus);


#endif

#endif
