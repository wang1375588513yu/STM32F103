/**********************  SPI  *********************
 ********************PA4---SPI_CS(片选)	***********
 ********************PA5---SPI_SCK(时钟)***********
 ********************PA6---SPI_MISO ***************
 ********************PA7---SPI_MOSI ***************
 
 **************************************************/

#include "spi.h"
#include "stm32f10x.h" 

#define GPIO_SPI_PORT	GPIOA
#define SPI_NSS_Pin		GPIO_Pin_4
#define SPI_SCK_Pin		GPIO_Pin_5
#define SPI_MOSI_Pin	GPIO_Pin_7
#define SPI_MISO_Pin	GPIO_Pin_6
#define RCC_SPI_PORT	RCC_APB2Periph_GPIOA

//片选CS
#define SPI_NSS_Clr()	GPIO_ResetBits(GPIO_SPI_PORT, SPI_NSS_Pin)
#define SPI_NSS_Set()	GPIO_SetBits(GPIO_SPI_PORT, SPI_NSS_Pin)
//时钟sck
#define SPI_SCK_Clr()	GPIO_ResetBits(GPIO_SPI_PORT, SPI_SCK_Pin)
#define SPI_SCK_Set()	GPIO_SetBits(GPIO_SPI_PORT, SPI_SCK_Pin)
//MOSI
#define SPI_MOSI_Clr()	GPIO_ResetBits(GPIO_SPI_PORT, SPI_MOSI_Pin)
#define SPI_MOSI_Set()	GPIO_SetBits(GPIO_SPI_PORT, SPI_MOSI_Pin)
//MISO
#define SPI_MISO_Read()	GPIO_ReadInputDataBit(GPIO_SPI_PORT, SPI_MISO_Pin)


void Spi_InitConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);	//初始化时钟
	
	GPIO_InitStructure.GPIO_Pin = SPI_SCK_Pin | SPI_MOSI_Pin | SPI_NSS_Pin;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_SPI_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = SPI_MISO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//CPOL = 1
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//CPHA = 1
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);
	
//	GPIO_SetBits(GPIO_SPI_PORT, SPI_SCK_Pin | SPI_MOSI_Pin);		//设置初始时刻为高电平
}




/*********************
 ***SPI时序
 **********************/
#ifndef SPI0_ENABLE
/*CPOL = 0 CPHA = 0*/
uint8_t SPI_ReadWrite(uint8_t data))
{
	uint8_t i = 0;
	uint8_t ret = 0;
	
	SPI_NSS_Clr();		//先打开片选，低电平工作

	for(i = 0; i < 8; i++)
	{
		/*先发送数据*/
		if(data & 0x80)
		SPI_MOSI_Set();
		else
			SPI_MOSI_Clr();
		data <<= 1;
		
		/*再读数据*/
		SPI_SCK_Set();	//时钟拉高，第一个变化
		ret <<= 1;
		if(SPI_MISO_Read())
			ret |= 0x01;

		SPI_SCK_Clr();	//时钟拉低，处于写状态
	}	
	SPI_NSS_Set();		//关闭片选
}
#endif

#ifndef SPI1_ENABLE
/*CPOL = 0 CPHA = 1*/
/*第二个跳变沿读，第一个跳变沿写*/
uint8_t SPI_ReadWrite(uint8_t data))
{
	uint8_t i = 0;
	uint8_t ret = 0;
	
	SPI_NSS_Clr();		//先打开片选，低电平工作

	for(i = 0 ; i < 8; i++)
	{
		SPI_SCK_Set();	//时钟拉高，第一个跳变沿输出
		
		//写数据
		if(data & 0x80)
			SPI_MOSI_Set();
		else
			SPI_MOSI_Clr();
		data <<= 1;
		
		SPI_SCK_Clr();	//时钟拉低，第二个跳变沿采样
		//读数据
		ret <<= 1;
		if(SPI_MISO_Read())
			ret |= 0x01;
	}
	SPI_NSS_Set();
}

#endif

#ifndef SPI2_ENABLE
/*CPOL = 1 CPHA = 0*/
/*时钟空闲时刻是高电平，第一个跳变沿采样（读数据）*/
uint8_t SPI_ReadWrite(uint8_t data))
{
	uint8_t i = 0;
	uint8_t ret = 0;
	
	SPI_NSS_Clr();		//先打开片选，低电平工作

	for(i = 0 ; i < 8; i++)
	{	
		SPI_SCK_Clr();
		//采样，读数据
		ret <<= 1;
		if(SPI_MISO_Read())
			data |= 0x01;
	
		SPI_SCK_Set();
		/*输出（写数据）*/
		if(data & 0x80)
			SPI_MOSI_Set();
		else
			SPI_MOSI_Clr();
		
	}
	
	SPI_NSS_Set();
}

#endif

#ifndef SPI3_ENABLE
/*CPOL = 1 CPHA = 1*/
/*第一个跳变沿发送，第二个跳变沿读取接收*/
uint8_t SPI_ReadWrite(uint8_t data))
{
	uint8_t i = 0;
	uint8_t ret = 0;
	
	SPI_NSS_Clr();		//先打开片选，低电平工作

	for(i = 0; i < 8; i++)
	{
		SPI_SCK_Clr();
		
		// send data
		if(data & 0x80)
			SPI_MOSI_Set();
		else
			SPI_MOSI_Clr();
		data <<= 1;
		
		//读数据
		SPI_SCK_Set();
		ret <<= 1;
		if(SPI_MISO_Read())
			ret |= 0x01;
		
	}
	SPI_NSS_Set();	//片选拉高，停止工作
}

#endif

