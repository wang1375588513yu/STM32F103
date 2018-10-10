#ifndef __I2C_DRIVER_H
#define __I2C_DRIVER_H

#include "stm32f10x.h"                  // Device header
#include "SysTick.h"


#define I2C_WR	0		/* Ð´¿ØÖÆbit */
#define I2C_RD	1		/* ¶Á¿ØÖÆbit */

void I2C_GPIO_Init(void);
void I2C_START(void);
void I2C_STOP(void);

void I2C_SendACK(u8 ack);
u8 I2C_ReceiveACK(void);

void I2C_SendByte(u8 dat);
u8 I2C_RecvByte(void);

uint8_t I2C_CheckDevice(uint8_t _Address);




#endif 


