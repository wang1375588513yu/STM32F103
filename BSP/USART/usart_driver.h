#ifndef _USART_DRIVER_H_
#define _USART_DRIVER_H_


#include <stdio.h>
#include "stm32f10x.h"

/**************************************************************************/
#define USART1_DEBUG
#define USART2_DEBUG
#define USART3_DEBUG
#define USART1_ENABLE
#define USART2_ENABLE
#define USART3_ENABLE


/**************************************************************************/
typedef enum
{
	USART1_BUF,
	USART2_BUF,
	USART3_BUF
}USART_Buf_Select;

#define USART1_BUFF	512
#define USART2_BUFF	512
#define USART3_BUFF	512

/**************************************************************************/
typedef struct arg_USARTRecvData
{
	s32 Recvcount;
	s8 *Recv_start;
	s8 *Recv_end;
}USARTRecvData;


extern s8 Usart1_buff[USART1_BUFF];
extern s8 Usart2_buff[USART2_BUFF];
extern s8 Usart3_buff[USART3_BUFF];
extern USARTRecvData Usart1_RecvStat;
extern USARTRecvData Usart2_RecvStat;
extern USARTRecvData Usart3_RecvStat;

/**************************************************************************/

void UASRT_NVIC_Configuration(void);
/***************************************
**USART1
****************************************/
void USART1_Config(void);
int fputc(int ch, FILE *f);
int fgetc(FILE *f);
char usart1_putc(char ch);
int usart1_putstr(unsigned char* buff,int len);
int usart1_print(unsigned char* buff);
char usart1_getc(void);
int usart1_getstr(unsigned char* buff,int len);

void UART1_SendString(char* s);

/***************************************
**USART2
****************************************/
void USART2_Config(void);

char usart2_putc(char ch);
int usart2_putstr(unsigned char* buff,int len);
int usart2_print(unsigned char* buff);
char usart2_getc(void);
int usart2_getstr(unsigned char* buff,int len);

/***************************************
**USART3(÷ÿ∂®“Â)
****************************************/
void USART3_Config(void);

char usart3_putc(char ch);
int usart3_putstr(unsigned char* buff,int len);
int usart3_print(unsigned char* buff);
char usart3_getc(void);
int usart3_getstr(unsigned char* buff,int len);

void UART3_SendString(char* s);

void USART_InitConfig(void);

void USART_ReadData_string(void);
#endif
