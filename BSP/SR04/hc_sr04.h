#ifndef _HC_SR04_H_
#define _HC_SR04_H_


#include "SysTick.h"
#include "stm32f10x.h" 
#include "led_driver.h"
#include "beep_driver.h"
#include "board_config.h"
#include "tim_driver.h"

//uint32_t  Distance = 0;  
//float Distance; //²âÊÔµÄ¾àÀë½á¹û
//uint8_t   Done;  
//uint32_t __IO time_1ms = 0;  
//uint32_t __IO time_120ms = 0;

void SR04_InitConfig(void);
void SR04_Ctrl(void);
void Distance_Show(void);


#endif

