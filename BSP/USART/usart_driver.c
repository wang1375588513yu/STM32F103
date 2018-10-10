/***********************************************************
**文件功能：USART串口操作文件
**接口定义：复用端口
**			USART1_Tx-----------GPIOA9
**			USART1_Rx-----------GPIOA10

**			USART2_Tx-----------GPIOA2
**			USART2_Rx-----------GPIOA3

**			USART3_Tx-----------GPIOB10
**			USART3_Rx-----------GPIOB11
--------重定义-------
**			USART3_Tx-----------GPIOC10
**			USART3_Rx-----------GPIOC11
************************************************************/

#include "stm32f10x.h"
#include "usart_driver.h"
#include "ST_string.h"

#ifdef USART1_ENABLE
s8 Usart1_buff[USART1_BUFF] = {0};
USARTRecvData Usart1_RecvStat = 
{
	0,
	&Usart1_buff[0],
	&Usart1_buff[0],
};
#endif

#ifdef USART2_ENABLE
s8 Usart2_buff[USART3_BUFF] = {0};
USARTRecvData Usart2_RecvStat = 
{
	0,
	&Usart2_buff[0],
	&Usart2_buff[0],
};
#endif

#ifdef USART3_ENABLE
s8 Usart3_buff[USART3_BUFF] = {0};
USARTRecvData Usart3_RecvStat = 
{
	0,
	&Usart3_buff[0],
	&Usart3_buff[0],
};
#endif 

/*清usart缓冲区*/
void Clear_USART_Buff(USART_Buf_Select usart)
{
/********************USART1_start*******************/
	if(usart == USART1_BUF)
	{
#ifdef USART1_ENABLE
		st_memset((s8*)Usart1_buff,(s8)0,USART1_BUFF);
		Usart1_RecvStat.Recvcount = 0;
		Usart1_RecvStat.Recv_start = &Usart1_buff[0];
		Usart1_RecvStat.Recv_end = &Usart1_buff[0];
#endif
	}
/*******************USART1_end********************/
/*******************USART2_start********************/
	else if(usart == USART2_BUF)
	{
#ifdef USART2_ENABLE
		st_memset((s8*)Usart2_buff,(s8)0,USART2_BUFF);
		Usart2_RecvStat.Recvcount = 0;
		Usart2_RecvStat.Recv_start = &Usart2_buff[0];
		Usart2_RecvStat.Recv_end = &Usart2_buff[0];
#endif
	}
/*******************USART2_end*******************/
/*******************USART3_start*******************/	
	else if(usart == USART3_BUF)
	{

		st_memset((s8*)Usart3_buff,(s8)0,USART3_BUFF);
		Usart3_RecvStat.Recvcount = 0;
		Usart3_RecvStat.Recv_start = &Usart3_buff[0];
		Usart3_RecvStat.Recv_end = &Usart3_buff[0];
		

	}
/********************USART3_end*****************/
}
/******************中断配置****************/
void UASRT_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
#ifdef USART1_ENABLE
	/* Enable the USART1 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif
	
#ifdef USART2_ENABLE
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif
	
#ifdef USART3_ENABLE	
	/*Enable the USART3 Interrupt*/
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;	//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//从优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);							//根据指定的参数初始化VIC寄存器 
#endif
	
	
}

/***************************************
**USART1
****************************************/
void USART1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	/* config USART1 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
	
	/* USART1 GPIO config */
	/* Configure USART1 Tx (PA.09) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    
	/* Configure USART1 Rx (PA.10) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* USART1 mode config */
	USART_InitStructure.USART_BaudRate = 38400;		//波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//字长
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	//停止位
	USART_InitStructure.USART_Parity = USART_Parity_No ;	//校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//流控
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//usart模式（收or发）
	USART_Init(USART1, &USART_InitStructure);		
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

//	USART_ClearFlag(USART1,USART_FLAG_TC);
	
	USART1->SR &= ~(1<<6);
	
	USART_Cmd(USART1, ENABLE);
}

#if 1
//stdout
int fputc(int ch, FILE *f)
{
		USART_SendData(USART1, (uint8_t) ch);

		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);		
	
		return (ch);
}

//stdin
int fgetc(FILE *f)
{
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
}
#endif


char usart1_putc(char ch)
{
	USART_SendData(USART1, (uint8_t) ch);
	
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);		

	return (ch);
}

int usart1_putstr(unsigned char* buff,int len)
{
	int i = 0;
	
	for(i = 0;i < len;i++)
	{
		usart1_putc(buff[i]);
	}

	return len;
}

int usart1_print(unsigned char* buff)
{
	int i = 0;
	
	while(*buff != '\0')
	{
		i++;
		usart1_putc(*buff++);
	}

	return i;
}

char usart1_getc(void)
{
	char temp = 0;
	
	while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)!= SET); 
	
	temp = USART_ReceiveData(USART1);

	return temp;
}

int usart1_getstr(unsigned char* buff,int len)
{
	int i = 0;

	for(i = 0;i < len;i++)
	{
		buff[i] = usart1_getc();
	}

	return len;
}

void UART1_SendString(char* s)
{
	while(*s)//检测字符串结束符
	{
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET); 
		USART_SendData(USART1 ,*s++);//发送当前字符
	}
}




/***************************************
**USART2
****************************************/
void USART2_Config(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	/* config USART2 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	
	/* USART2 GPIO config */
	/* Configure USART2 Tx (PA.2) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* Configure USART2 Rx (PA.3) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* USART2 mode config */
	USART_InitStructure.USART_BaudRate = 38400;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
	
	USART_ClearFlag(USART2,USART_FLAG_TC);
	USART2->SR &= ~(0x01<<6);
	
//	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
//	USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);

	USART_Cmd(USART2, ENABLE);
}



char usart2_putc(char ch)
{
	USART_SendData(USART2, (uint8_t) ch);
	
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) != SET);		

	return (ch);
}

char usart2_getc(void)
{
	char temp = 0;
	
	while(	USART_GetFlagStatus(USART2,USART_FLAG_RXNE)!= SET);
	
	temp = USART_ReceiveData(USART2);

	return temp;
}



int usart2_putstr(unsigned char* buff,int len)
{
	int i = 0;
	
	for(i = 0;i < len;i++)
	{
		usart2_putc(buff[i]);
	}

	return len;
}

int Usart2_Sendstr(unsigned char* buff)
{
	int ret = 0;
	
	while(*buff)
	{
		usart2_putc(*buff);
		buff++;
		ret++;
	}
	
	return ret;
}

int usart2_getstr(unsigned char* buff,int len)
{
	int i = 0;

	for(i = 0;i < len;i++)
	{
		buff[i] = usart2_getc();
	}

	return len;
}

void USART3_Config(void)
{
	#if 0
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	//NVIC_InitTypeDef NVIC_InitStructure; 

	/*使能GPIOC和复用的时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);	
	/*使能USART3外设时钟*/  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	
	
	//GPIO_PinRemapConfig(GPIO_FullRemap_USART3, ENABLE);//usart3复用功能全部映射
	GPIO_PinRemapConfig(GPIO_PartialRemap_USART3, ENABLE);//usart3复用功能部分映射
	
	/*复位串口3*/
 	USART_DeInit(USART3);  
	
	/*USART3_GPIO初始化设置*/ 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;			//USART3_TXD(PC10)     
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//设置引脚输出最大速率为50MHz
    GPIO_Init(GPIOC, &GPIO_InitStructure);				//调用库函数中的GPIO初始化函数，初始化USART1_TXD(PA.9)  
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				//USART3_RXD(PC11)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
    GPIO_Init(GPIOC, &GPIO_InitStructure);					//调用库函数中的GPIO初始化函数，初始化USART1_RXD(PA.10)
	
	/*USART3 初始化设置*/
	USART_InitStructure.USART_BaudRate = 115200;									//设置波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//1个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;								//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//工作模式设置为收发模式
	USART_Init(USART3, &USART_InitStructure);										//初始化串口3

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);			//使能串口3接收中断
	USART_ClearFlag(USART3, USART_FLAG_TC);					//清除发送完成标志
	USART_Cmd(USART3, ENABLE);     	//使能串口

#else

	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	//NVIC_InitTypeDef NVIC_InitStructure; 

	/*使能GPIOC和复用的时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);	
	/*使能USART3外设时钟*/  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	
	
	//GPIO_PinRemapConfig(GPIO_FullRemap_USART3, ENABLE);//usart3复用功能全部映射
//	GPIO_PinRemapConfig(GPIO_PartialRemap_USART3, ENABLE);//usart3复用功能部分映射
	
	/*复位串口3*/
 	USART_DeInit(USART3);  
	
	/*USART3_GPIO初始化设置*/ 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;			//USART3_TXD(PC10)     
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//设置引脚输出最大速率为50MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);				//调用库函数中的GPIO初始化函数，初始化USART1_TXD(PA.9)  
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				//USART3_RXD(PC11)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);					//调用库函数中的GPIO初始化函数，初始化USART1_RXD(PA.10)
	
	/*USART3 初始化设置*/
	USART_InitStructure.USART_BaudRate = 115200;									//设置波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//1个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;								//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//工作模式设置为收发模式
	USART_Init(USART3, &USART_InitStructure);										//初始化串口3

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);			//使能串口3接收中断
	USART_ClearFlag(USART3, USART_FLAG_TC);					//清除发送完成标志
	USART_Cmd(USART3, ENABLE);      


#endif
#if 0
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;	//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//从优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);							//根据指定的参数初始化VIC寄存器 
#endif
}

char usart3_putc(char ch)
{
	USART_SendData(USART3, (uint8_t) ch);
	
	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) != SET);		

	return (ch);
}

int usart3_putstr(unsigned char* buff,int len)
{
	int i = 0;
	
	for(i = 0;i < len;i++)
	{
		usart3_putc(buff[i]);
	}

	return len;
}

int usart3_print(unsigned char* buff)
{
	int i = 0;
	
	while(*buff != '\0')
	{
		i++;
		usart3_putc(*buff++);
	}

	return i;
}

char usart3_getc(void)
{
	char temp = 0;
	
	while(USART_GetFlagStatus(USART3,USART_FLAG_RXNE)!= SET); 
	
	temp = USART_ReceiveData(USART3);

	return temp;
}

int usart3_getstr(unsigned char* buff,int len)
{
	int i = 0;

	for(i = 0;i < len;i++)
	{
		buff[i] = usart3_getc();
	}

	return len;
}

void UART3_SendString(char* s)
{
	while(*s)//检测字符串结束符
	{
		while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET); 
		USART_SendData(USART3 ,*s++);//发送当前字符
	}
}


void USART_InitConfig(void)
{
	USART1_Config();
//	USART2_Config();
	USART3_Config();
	
	UASRT_NVIC_Configuration();
	
}





