#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"

#define USART1_REC_MAXLEN 200	//���������ݳ���

void USART1_Init_Config(void);
void UART1_SendString(char* s);
void USART3_Init_Config(void);
void UART3_SendString(char* s);
void UART1_SendLR(void);
void UART3_SendLR(void);

//����1���ͻس�����
#define UART1_SendLR() UART1_SendString("\r\n")

//											 USART_SendData(USART1,0X0D);\
//											 
//											 USART_SendData(USART1,0X0A)
//����2���ͻس�����
#define UART3_SendLR() UART3_SendString("\r\n")
//											 USART_SendData(USART2,0X0D);\
//											 USART_SendData(USART2,0X0A)
#endif


