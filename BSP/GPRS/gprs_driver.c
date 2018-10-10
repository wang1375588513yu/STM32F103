/**********************************************************************************
   STM32F103RCT6  

	 * 硬件连接说明
	 使用单片串口3与GPRS模块通信  注：使用串口3可以避免下载和通信不会冲突
	 STM32      GPRS模块
	 PC10 (USART3-TX)->RXD
	 PC11 (USART3-RX)->TXD
************************************************************************************	 
	 GND	      ->GND

	按键  PB1   接地   接听电话
	按键  PB2   接地   挂起电话
 
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
#define Buf1_Max 60 					  //串口1缓存长度
#define Buf2_Max 200 					  //串口2缓存长度
/*************	本地常量声明	**************/

char *call_phone = "ATD17670524699;\r\n";  //打电话对方的号码


char *GSM_Way="GSM";//短信中心号 关于中心号的介绍可以看串口调试笔记的第二部分
char *phone="17370524699";		//短信接收方手机号
char *content="Hello! Mr.ZHU!";//发送短信内容

/*************  本地变量声明	**************/
char Uart3_Buf[Uart3_Max];//串口2接收缓存
u8 Times=0,First_Int=0,shijian=0;
vu8 Timer0_start;	//定时器0延时启动计数器
vu8 Uart3_Start;	//串口2开始接收数据
vu8 Uart3_End;	  //串口2接收数据结束

void USART3_IRQHandler(void)                	
{
	u8 Res=0;
	if(USART_GetITStatus(USART3,USART_IT_RXNE) == SET)
	{
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
		Res =USART_ReceiveData(USART3);
		//usart1_putc(Res);
		Uart3_Buf[First_Int] = Res;  	  //将接收到的字符串存到缓存中
		First_Int++;                			//缓存指针向后移动
		if(First_Int > Uart3_Max)       		//如果缓存满,将缓存指针指向缓存的首地址
		{
			First_Int = 0;
		}
		//Led_Ctl(LED_RED,ON);
	}
} 	

/*******************************************************************************
* 函数名  : TIM2_IRQHandler
* 描述    : 定时器2中断断服务函数
* 输入    : 无
* 输出    : 无
* 返回    : 无 
* 说明    : 无
*******************************************************************************/
void TIM2_IRQHandler(void)   //TIM3中断
{
	static u8 flag =1;

	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIMx更新中断标志 
		
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
* 函数名 : CLR_Buf2
* 描述   : 清除串口2缓存数据
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void CLR_Buf3(void)
{
	u16 k;
	for(k=0;k<Uart3_Max;k++)      //将缓存内容清零
	{
		Uart3_Buf[k] = 0x00;
	}
    First_Int = 0;              //接收字符串的起始存储位置
}

/*******************************************************************************
* 函数名 : Wait_CREG
* 描述   : 等待模块注册成功
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
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
		//UART3_SendString("AT+CREG?");   //查询模块是否注册成功
		UART3_SendString("AT+CREG?");
		UART3_SendString("\r\n");
		delay_ms(1000);
		delay_ms(1000);
	    for(k=0;k<Uart3_Max;k++)      			
    	{
			if(Uart3_Buf[k] == ':')
			{
				if(((Uart3_Buf[k+4] == '1')&&(Uart3_Buf[k+5] != '3'))||(Uart3_Buf[k+4] == '5'))  //说明模块注册成功
				{
					flag = 1;
					break;
				}
			}
		}
	}
}
/*===========================发送TEXT短信——START====================================*/
/*******************************************************************************
* 函数名 : Find
* 描述   : 判断缓存中是否含有指定的字符串
* 输入   : 
* 输出   : 
* 返回   : unsigned char:1 找到指定字符，0 未找到指定字符 
* 注意   : 
*******************************************************************************/

u8 Find(char *a)
{ 
	if(strstr(Uart3_Buf,a)!=NULL)
	    return 1;
	else
		return 0;
}

/*******************************************************************************
* 函数名 : Send_AT_Command
* 描述   : 发送AT指令函数
* 输入   : 发送数据的指针、希望接收到的应答数据指针、发送等待时间(单位：S)
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/

void Send_AT_Command(char *b,char *a,u8 wait_time)         
{
	u8 flag;
	char *c;
	c = b;										//保存字符串地址到c
	CLR_Buf3(); 
	flag = 0;
	while(flag == 0)                    
	{
		if(!Find(a)) 
		{
			if(Timer0_start == 0)
			{
				b = c;							//将字符串地址给b
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
* 函数名 : Set_Text_Mode
* 描述   : 设置短信为TEXT文本模式
*******************************************************************************/
void Set_Text_Mode(void)
{
	//char temp[50]="AT+CSGS=";
	Send_AT_Command("ATE0","OK",3);										//取消回显	
	Send_AT_Command("AT+CMGF=1","OK",3);								//TEXT模式	

}
/*******************************************************************************
* 函数名 : Send_Text_Sms
* 描述   : 发送TEXT文本短信
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void Send_Text_Sms(void)
{
	char temp[50]="AT+CMGS=";	
	strcat(temp,(char*)phone); //字符串拼接函数（库函数）
	Send_AT_Command(temp,">",3); //等待接收到“>”才发送短信内容
	UART3_SendString((char*)content);     //发送短信内容
	USART_SendData(USART3 ,0X1A);  //发送结束符
	UART3_SendString("\r\n");
}

/*===========================发送TEXT短信——END====================================*/


/*---------打电话----------*/
void Call_Phone(void)
{
	Wait_CREG();                 //等待模块注册成功
	UART3_SendString("AT+SNFS=0;\r\n");//将音频切换到耳机通道
	delay_ms(1000);
	delay_ms(1000);
	UART3_SendString(call_phone);     //拨打指定电话
}


/*---------发TEXT短信----------*/
void Send_text_message(void)
{
	Wait_CREG();    //查询等待模块注册成功

	Set_Text_Mode();//设置短信为TEXT模式
	Send_Text_Sms();//发送一条短消息
}





