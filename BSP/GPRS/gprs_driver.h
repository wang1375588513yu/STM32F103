#ifndef _GPRS_DRIVER_H_
#define _GPRS_DRIVER_H_

#include "stm32f10x.h" 




//串口1发送回车换行
//#define UART1_SendLR() UART1_SendString("\r\n")

//											 USART_SendData(USART1,0X0D);\
//											 
//											 USART_SendData(USART1,0X0A)

//串口2发送回车换行
//#define UART3_SendLR() UART3_SendString("\r\n")
//											 USART_SendData(USART3,0X0D);\
//											 USART_SendData(USART3,0X0A)

//void GPRS_call(void);		
//void GPRS_txt_msg(void);	
void Call_Phone(void);			//打电话
void Send_text_message(void);		//发送txt短信
void Send_Chinese_message(void);//发送中文短信
//void Answer_phone(void);
//void Hang_up_phone(void);

#endif


