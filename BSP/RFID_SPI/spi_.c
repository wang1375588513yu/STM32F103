
#include "spi_driver.h"
#include "usart_driver.h"

/**********************************************************
**************************引脚定义*************************
**SPI_NSS:			PA4
**SPI_CLK:			PA5
**SPI_MISO:			PA6
**SPI_MOSI:			PA7
***********************************************************/

/******************************************************
**函数功能：SPI初始化
*******************************************************/
#define RF_CLK_Clr()		GPIO_ResetBits(GPIOA, GPIO_Pin_11)
#define RF_CLK_Set()		GPIO_SetBits(GPIOA, GPIO_Pin_11)

#define RF_MOSI_Clr()		GPIO_ResetBits(GPIOB, GPIO_Pin_8)	
#define RF_MOSI_Set()		GPIO_SetBits(GPIOB, GPIO_Pin_8)

#define RF_NSS_Clr()		GPIO_ResetBits(GPIOA, GPIO_Pin_12)	
#define RF_NSS_Set()		GPIO_SetBits(GPIOA, GPIO_Pin_12)

#define RF_MISO_READ()		GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2)

void RFID_SPI_InitConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
//	RF_CLK_Clr();
	RF_CLK_Set();
	RF_NSS_Set();
}


uint8_t RFID_SPI_WriterByte(uint8_t dat)
{	
	uint8_t i;
	uint8_t rx_data = 0;
//	RF_CLK_Set();
	for(i = 0;i < 8;i++)
	{
		RF_CLK_Clr();
		if(dat&0x80)
		{
			RF_MOSI_Set();
		}
		else	RF_MOSI_Clr();
		dat <<= 1;
		
		RF_CLK_Set();
		rx_data <<= 1;
		if(RF_MISO_READ())
		{
			rx_data |= 1;
		}
	}
	
	return rx_data;
}

