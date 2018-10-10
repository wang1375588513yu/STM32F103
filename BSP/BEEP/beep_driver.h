#ifndef _BEEP_DRIVER_H_
#define _BEEP_DRIVER_H_

//#define BEEP_CTL(a) 		if(a) GPIO_SetBits(GPIOC,GPIO_Pin_9);\
												else 	GPIO_ResetBits(GPIOC,GPIO_Pin_9);


#include "stm32f10x.h" 
#include "board_config.h"

#if 0
void Beep_InitConfig(void);
void Beep_Contrl(u16 pwm);
#else
void Beep_InitConfig(void);
void BEEP_Contrl(u8 status);
#endif 
void Beep_On(void);
void Beep_Off(void);
void Beep_hold(void);
#endif
