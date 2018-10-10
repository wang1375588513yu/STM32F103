/**********************************************************************************
   STM32F103RCT6  

	 * Ӳ������˵��
	 ʹ�õ�Ƭ����3��GPRSģ��ͨ��  ע��ʹ�ô���3���Ա������غ�ͨ�Ų����ͻ
	 STM32      GPRSģ��
	 PC10 (USART3-TX)->RXD
	 PC11 (USART3-RX)->TXD
************************************************************************************	 
	 GND	      ->GND

	����  PB1   �ӵ�   �����绰
	����  PB2   �ӵ�   ����绰
 
**********************************************************************************/
#include "stm32f10x.h"
#include "gprs_driver.h"
#include "usart.h"
#include "led_driver.h"
#include "SysTick.h"
#include "timer.h"
#include "string.h"
#include "key.h" 
#include "beep_driver.h"
#include "usart_driver.h"

#define Uart3_Max 200
#define Buf1_Max 60 					  //����1���泤��
#define Buf2_Max 200 					  //����2���泤��
/*************	���س�������	**************/

char *call_phone = "ATD17670524699;\r\n";  //��绰�Է��ĺ���


char *GSM_Way="GSM";//�������ĺ� �������ĺŵĽ��ܿ��Կ����ڵ��Աʼǵĵڶ�����
char *phone="17370524699";		//���Ž��շ��ֻ���
char *content="Hello! Mr.ZHU!";//���Ͷ�������

/*************  ���ر�������	**************/
char Uart3_Buf[Uart3_Max];//����2���ջ���
u8 Times=0,First_Int=0,shijian=0;
vu8 Timer0_start;	//��ʱ��0��ʱ����������
vu8 Uart3_Start;	//����2��ʼ��������
vu8 Uart3_End;	  //����2�������ݽ���

void USART3_IRQHandler(void)                	
{
	u8 Res=0;
	if(USART_GetITStatus(USART3,USART_IT_RXNE) == SET)
	{
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
		Res =USART_ReceiveData(USART3);
		//usart1_putc(Res);
		Uart3_Buf[First_Int] = Res;  	  //�����յ����ַ����浽������
		First_Int++;                			//����ָ������ƶ�
		if(First_Int > Uart3_Max)       		//���������,������ָ��ָ�򻺴���׵�ַ
		{
			First_Int = 0;
		}
		//Led_Ctl(LED_RED,ON);
	}
} 	

/*******************************************************************************
* ������  : TIM2_IRQHandler
* ����    : ��ʱ��2�ж϶Ϸ�����
* ����    : ��
* ���    : ��
* ����    : �� 
* ˵��    : ��
*******************************************************************************/
void TIM2_IRQHandler(void)   //TIM3�ж�
{
	static u8 flag =1;

	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
		
		if(Timer0_start)
			Times++;
		if(Times > shijian)
		{
			Timer0_start = 0;
			Times = 0;
		}
	
		if(flag)
		{
			//Led_Ctl(LED_RED,ON);
			flag=0;
		}
		else
		{
			//Led_Ctl(LED_RED,OFF);
			flag=1;
		}
	}	
}

/*******************************************************************************
* ������ : CLR_Buf2
* ����   : �������2��������
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void CLR_Buf3(void)
{
	u16 k;
	for(k=0;k<Uart3_Max;k++)      //��������������
	{
		Uart3_Buf[k] = 0x00;
	}
    First_Int = 0;              //�����ַ�������ʼ�洢λ��
}

/*******************************************************************************
* ������ : Wait_CREG
* ����   : �ȴ�ģ��ע��ɹ�
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void Wait_CREG(void)
{
	u8 flag;
	u8 k;
	flag = 0;
	CLR_Buf3();
	while(flag == 0)        			
	{
		CLR_Buf3();        
		//UART3_SendString("AT+CREG?");   //��ѯģ���Ƿ�ע��ɹ�
		UART3_SendString("AT+CREG?");
		UART3_SendString("\r\n");
		delay_ms(1000);
		delay_ms(1000);
	    for(k=0;k<Uart3_Max;k++)      			
    	{
			if(Uart3_Buf[k] == ':')
			{
				if(((Uart3_Buf[k+4] == '1')&&(Uart3_Buf[k+5] != '3'))||(Uart3_Buf[k+4] == '5'))  //˵��ģ��ע��ɹ�
				{
					flag = 1;
					break;
				}
			}
		}
	}
}
/*===========================����TEXT���š���START====================================*/
/*******************************************************************************
* ������ : Find
* ����   : �жϻ������Ƿ���ָ�����ַ���
* ����   : 
* ���   : 
* ����   : unsigned char:1 �ҵ�ָ���ַ���0 δ�ҵ�ָ���ַ� 
* ע��   : 
*******************************************************************************/

u8 Find(char *a)
{ 
	if(strstr(Uart3_Buf,a)!=NULL)
	    return 1;
	else
		return 0;
}

/*******************************************************************************
* ������ : Send_AT_Command
* ����   : ����ATָ���
* ����   : �������ݵ�ָ�롢ϣ�����յ���Ӧ������ָ�롢���͵ȴ�ʱ��(��λ��S)
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/

void Send_AT_Command(char *b,char *a,u8 wait_time)         
{
	u8 flag;
	char *c;
	c = b;										//�����ַ�����ַ��c
	CLR_Buf3(); 
	flag = 0;
	while(flag == 0)                    
	{
		if(!Find(a)) 
		{
			if(Timer0_start == 0)
			{
				b = c;							//���ַ�����ַ��b
				for (; *b!='\0';b++)
				{
					while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET);
					USART_SendData(USART3,*b);//UART2_SendData(*b);
				}
				UART3_SendString("\r\n");
				Times = 0;
				shijian = wait_time;
				Timer0_start = 1;
		   }
		}
		else
		{
			flag = 1;
			Timer0_start = 0;
		}
	}
	CLR_Buf3(); 
}

/*******************************************************************************
* ������ : Set_Text_Mode
* ����   : ���ö���ΪTEXT�ı�ģʽ
*******************************************************************************/
void Set_Text_Mode(void)
{
	//char temp[50]="AT+CSGS=";
	Send_AT_Command("ATE0","OK",3);										//ȡ������	
	Send_AT_Command("AT+CMGF=1","OK",3);								//TEXTģʽ	

}
/*******************************************************************************
* ������ : Send_Text_Sms
* ����   : ����TEXT�ı�����
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void Send_Text_Sms(void)
{
	char temp[50]="AT+CMGS=";	
	strcat(temp,(char*)phone); //�ַ���ƴ�Ӻ������⺯����
	Send_AT_Command(temp,">",3); //�ȴ����յ���>���ŷ��Ͷ�������
	UART3_SendString((char*)content);     //���Ͷ�������
	USART_SendData(USART3 ,0X1A);  //���ͽ�����
	UART3_SendString("\r\n");
}

/*===========================����TEXT���š���END====================================*/


/*---------��绰----------*/
void Call_Phone(void)
{
	Wait_CREG();                 //�ȴ�ģ��ע��ɹ�
	UART3_SendString("AT+SNFS=0;\r\n");//����Ƶ�л�������ͨ��
	delay_ms(1000);
	delay_ms(1000);
	UART3_SendString(call_phone);     //����ָ���绰
}


/*---------��TEXT����----------*/
void Send_text_message(void)
{
	Wait_CREG();    //��ѯ�ȴ�ģ��ע��ɹ�

	Set_Text_Mode();//���ö���ΪTEXTģʽ
	Send_Text_Sms();//����һ������Ϣ
}





