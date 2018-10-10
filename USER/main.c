/********************************Copyright (c)**********************************\
**
**                   (c) Copyright 2018, WangYu, China, SZ.
**                           All Rights Reserved
**
**                       By(�����б�������Ϣ�Ƽ����޹�˾)
**
**----------------------------------�ļ���Ϣ------------------------------------
** �ļ�����: main.c
** ������Ա: WangYu
** ��������: 2018-08-22
** �ĵ�����:
**
**----------------------------------�汾��Ϣ------------------------------------
** �汾����: V1.0
** �汾˵��: ��ʼ�汾
**
**------------------------------------------------------------------------------
\********************************End of Head************************************/

#include "beep_driver.h"
#include "stm32f10x.h"
#include "led_driver.h"
#include "SysTick.h"
#include "infraed.h"
#include "smoke_driver.h"
#include "hc_sr04.h"
#include "gprs_driver.h"
#include "key.h"
#include "exti_driver.h"
#include "usart_driver.h"
#include "ST_string.h"
#include "rc522_driver.h"
#include "spi_driver.h"
#include "i2c_driver.h"
#include "Run_task.h"

#include "raster_ruler.h"



#if 0
int main(void)
{
    Led_InitConfig();		//LED
    SysTick_init(72);		//�δ�ʱ��
//	Beep_InitConfig();		//BEEP

//	Infraed_InitConfig();	//����
//	ADC1_Init();			//����������
//	SR04_InitConfig();		//������
//	Key_InitConfig();		//����
    Exti_InitConfig();		//�ж�
    USART_InitConfig();
//	RC522_Init();			//RFID
//	RFID_SPI_InitConfig();	//RFID_SPi


    usart1_putstr("Usart1_Begin\r\n",st_strlen("Usart1_Begin\r\n"));
    usart3_putstr("Usart3_Begin\r\n",st_strlen("Usart3_Begin\r\n"));
    Run_Start();
//	Call_Phone();			//��绰
    Send_text_message();	//����TEXT����
//	Send_Chinese_message();

    while(1)
    {
//		RFID_Test();
//		Beep_hold();
//		Led_Ctl(LED_GREEN,ON);
//		Led_Ctl(LED_BLUE,ON);
//		SR04_Ctrl();			//������ģ������
//		Distance_Show();		//������ģ�鹤��
//		Led_Ctl(LED_BLUE,ON);
//		delay_ms(250);
//		Led_Ctl(LED_GREEN,OFF);
//		Led_Ctl(LED_BLUE,OFF);
//		Led_Ctl(LED_BLUE,OFF);
//		delay_ms(250);
//		Beep_hold();
//		Key_Scan(get_KeyType());

//		delay_ms(1000);
//		USART_SendData(USART3, 'a');
//		Beep_On();
//		delay_ms(1500);
//		Beep_Off();
//		delay_ms(1500);
//		delay_ms(1000);
//		Beep_On();
//
//		delay_ms(10);
//		Beep_Off();
        Led_Ctl(LED_BLUE,ON);
//		delay_ms(10);
        Led_Ctl(LED_GREEN,ON);
//		delay_ms(1000);
//		//Beep_Contrl(128);
//		delay_ms(1000);
//

//		Led_Ctl(LED_RED,ON);
//		Led_Ctl(LED_BLUE,ON);
//		Led_Ctl(LED_GREEN,ON);
        //Beep_Contrl(0);
//		delay_ms(1000);
//		Inf_Ctl();		//����
        //Key_Scan(Get_KeyType());
    }
}

#else
int main(void)
{
    Led_InitConfig();		//LED
    SysTick_init(72);		//�δ�ʱ��
    Exti_InitConfig();		//�ж�
    USART_InitConfig();


//	usart1_putstr("Usart1_Begin\r\n",st_strlen("Usart1_Begin\r\n"));
    usart3_putstr("Usart3_Begin\r\n",st_strlen("Usart3_Begin\r\n"));
//	Call_Phone();			//��绰
//	Send_text_message();	//����TEXT����




    while(1)
    {
        USART_SendData(USART3, 'a');
//		Led_Ctl(LED_RED,ON);
//		delay_ms(1000);
        Led_Ctl(LED_BLUE,ON);
        delay_ms(1000);
//		Led_Ctl(LED_GREEN,ON);
//		delay_ms(1000);
//
        Led_Ctl(LED_BLUE,OFF);
        delay_ms(1000);
//		Led_Ctl(LED_BLUE,OFF);

    }
}


#endif