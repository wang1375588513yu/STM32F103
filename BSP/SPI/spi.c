/**********************  SPI  *********************
 ********************PA4---SPI_CS(Ƭѡ)	***********
 ********************PA5---SPI_SCK(ʱ��)***********
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

//ƬѡCS
#define SPI_NSS_Clr()	GPIO_ResetBits(GPIO_SPI_PORT, SPI_NSS_Pin)
#define SPI_NSS_Set()	GPIO_SetBits(GPIO_SPI_PORT, SPI_NSS_Pin)
//ʱ��sck
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
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);	//��ʼ��ʱ��
	
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
	
//	GPIO_SetBits(GPIO_SPI_PORT, SPI_SCK_Pin | SPI_MOSI_Pin);		//���ó�ʼʱ��Ϊ�ߵ�ƽ
}




/*********************
 ***SPIʱ��
 **********************/
#ifndef SPI0_ENABLE
/*CPOL = 0 CPHA = 0*/
uint8_t SPI_ReadWrite(uint8_t data))
{
	uint8_t i = 0;
	uint8_t ret = 0;
	
	SPI_NSS_Clr();		//�ȴ�Ƭѡ���͵�ƽ����

	for(i = 0; i < 8; i++)
	{
		/*�ȷ�������*/
		if(data & 0x80)
		SPI_MOSI_Set();
		else
			SPI_MOSI_Clr();
		data <<= 1;
		
		/*�ٶ�����*/
		SPI_SCK_Set();	//ʱ�����ߣ���һ���仯
		ret <<= 1;
		if(SPI_MISO_Read())
			ret |= 0x01;

		SPI_SCK_Clr();	//ʱ�����ͣ�����д״̬
	}	
	SPI_NSS_Set();		//�ر�Ƭѡ
}
#endif

#ifndef SPI1_ENABLE
/*CPOL = 0 CPHA = 1*/
/*�ڶ��������ض�����һ��������д*/
uint8_t SPI_ReadWrite(uint8_t data))
{
	uint8_t i = 0;
	uint8_t ret = 0;
	
	SPI_NSS_Clr();		//�ȴ�Ƭѡ���͵�ƽ����

	for(i = 0 ; i < 8; i++)
	{
		SPI_SCK_Set();	//ʱ�����ߣ���һ�����������
		
		//д����
		if(data & 0x80)
			SPI_MOSI_Set();
		else
			SPI_MOSI_Clr();
		data <<= 1;
		
		SPI_SCK_Clr();	//ʱ�����ͣ��ڶ��������ز���
		//������
		ret <<= 1;
		if(SPI_MISO_Read())
			ret |= 0x01;
	}
	SPI_NSS_Set();
}

#endif

#ifndef SPI2_ENABLE
/*CPOL = 1 CPHA = 0*/
/*ʱ�ӿ���ʱ���Ǹߵ�ƽ����һ�������ز����������ݣ�*/
uint8_t SPI_ReadWrite(uint8_t data))
{
	uint8_t i = 0;
	uint8_t ret = 0;
	
	SPI_NSS_Clr();		//�ȴ�Ƭѡ���͵�ƽ����

	for(i = 0 ; i < 8; i++)
	{	
		SPI_SCK_Clr();
		//������������
		ret <<= 1;
		if(SPI_MISO_Read())
			data |= 0x01;
	
		SPI_SCK_Set();
		/*�����д���ݣ�*/
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
/*��һ�������ط��ͣ��ڶ��������ض�ȡ����*/
uint8_t SPI_ReadWrite(uint8_t data))
{
	uint8_t i = 0;
	uint8_t ret = 0;
	
	SPI_NSS_Clr();		//�ȴ�Ƭѡ���͵�ƽ����

	for(i = 0; i < 8; i++)
	{
		SPI_SCK_Clr();
		
		// send data
		if(data & 0x80)
			SPI_MOSI_Set();
		else
			SPI_MOSI_Clr();
		data <<= 1;
		
		//������
		SPI_SCK_Set();
		ret <<= 1;
		if(SPI_MISO_Read())
			ret |= 0x01;
		
	}
	SPI_NSS_Set();	//Ƭѡ���ߣ�ֹͣ����
}

#endif

