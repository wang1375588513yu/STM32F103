#include "eeprom_driver.h"
#include "i2c_driver.h"
#include "usart_driver.h"
#include "SysTick.h"

#define EE_DEV_ADDR			0xA0		/* 24xx04的设备地址 */
#define EE_PAGE_SIZE		16			/* 24xx04的页面大小 */
#define EE_SIZE				512			/* 24xx04总容量 */

/*
*********************************************************************************************************
*	函 数 名: ee_CheckOk
*	功能说明: 判断串行EERPOM是否正常
*	形    参：无
*	返 回 值: 1 表示正常， 0 表示不正常
*********************************************************************************************************
*/
uint8_t eeprom_CheckOK(void)
{
	if(I2C_CheckDevice(EE_DEV_ADDR) == 0)
		return 1;
	else 
	{
		/*失败后，切记发送停止信号*/
		I2C_STOP();
		return 0;
	}
}

/******************************************************************
*	函 数 名: eeprom_ReadBytes
*	功能说明: 从串行EEPROM指定地址处开始读取若干数据
*	形    参：_usAddress : 起始地址
*			 _usSize : 数据长度，单位为字节
*			 _pReadBuf : 存放读到的数据的缓冲区指针
*	返 回 值: 0 表示失败，1表示成功
********************************************************************/
uint8_t eeprom_ReadBytes(uint8_t *_pReadBuf, uint16_t _usAddress, uint16_t _usSize)
{
	uint16_t i;
	
	I2C_START();//1、开始信号
	
	I2C_SendByte((0XA0|((_usAddress/256)<<1))|I2C_WR);//2、发送设备地址，以及读写控制位
	
	if(I2C_ReceiveACK() != 0)
	{
		goto cmd_fail;
	}
	
	
	
	
	
cmd_fail:
	/*命令失败之后，切记停止，防止影响其他设备*/
	I2C_STOP();
	printf("Read fild!\r\n");
	return 0;
}

  



