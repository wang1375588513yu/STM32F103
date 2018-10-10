


#ifndef __HC_SR04_H__
#define __HC_SR04_H__

#include "stm32f10x.h"
#include "SysTick.h"



#define GPIO_X  GPIOC
#define SR04_TRIG GPIO_Pin_11

#define GPIO_Y 	GPIOB
#define SP04_ECH0 GPIO_Pin_4

void Run_SR04(void);
void HC_init(void);


#endif

