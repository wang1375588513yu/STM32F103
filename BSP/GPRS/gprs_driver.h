#ifndef _GPRS_DRIVER_H_
#define _GPRS_DRIVER_H_

#include "stm32f10x.h" 




//����1���ͻس�����
//#define UART1_SendLR() UART1_SendString("\r\n")

//											 USART_SendData(USART1,0X0D);\
//											 
//											 USART_SendData(USART1,0X0A)

//����2���ͻس�����
//#define UART3_SendLR() UART3_SendString("\r\n")
//											 USART_SendData(USART3,0X0D);\
//											 USART_SendData(USART3,0X0A)

//void GPRS_call(void);		
//void GPRS_txt_msg(void);	
void Call_Phone(void);			//��绰
void Send_text_message(void);		//����txt����
void Send_Chinese_message(void);//�������Ķ���
//void Answer_phone(void);
//void Hang_up_phone(void);

#endif


