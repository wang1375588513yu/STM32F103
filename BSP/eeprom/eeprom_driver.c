#include "eeprom_driver.h"
#include "i2c_driver.h"
#include "usart_driver.h"
#include "SysTick.h"

#define EE_DEV_ADDR			0xA0		/* 24xx04���豸��ַ */
#define EE_PAGE_SIZE		16			/* 24xx04��ҳ���С */
#define EE_SIZE				512			/* 24xx04������ */

/*
*********************************************************************************************************
*	�� �� ��: ee_CheckOk
*	����˵��: �жϴ���EERPOM�Ƿ�����
*	��    �Σ���
*	�� �� ֵ: 1 ��ʾ������ 0 ��ʾ������
*********************************************************************************************************
*/
uint8_t eeprom_CheckOK(void)
{
	if(I2C_CheckDevice(EE_DEV_ADDR) == 0)
		return 1;
	else 
	{
		/*ʧ�ܺ��мǷ���ֹͣ�ź�*/
		I2C_STOP();
		return 0;
	}
}

/******************************************************************
*	�� �� ��: eeprom_ReadBytes
*	����˵��: �Ӵ���EEPROMָ����ַ����ʼ��ȡ��������
*	��    �Σ�_usAddress : ��ʼ��ַ
*			 _usSize : ���ݳ��ȣ���λΪ�ֽ�
*			 _pReadBuf : ��Ŷ��������ݵĻ�����ָ��
*	�� �� ֵ: 0 ��ʾʧ�ܣ�1��ʾ�ɹ�
********************************************************************/
uint8_t eeprom_ReadBytes(uint8_t *_pReadBuf, uint16_t _usAddress, uint16_t _usSize)
{
	uint16_t i;
	
	I2C_START();//1����ʼ�ź�
	
	I2C_SendByte((0XA0|((_usAddress/256)<<1))|I2C_WR);//2�������豸��ַ���Լ���д����λ
	
	if(I2C_ReceiveACK() != 0)
	{
		goto cmd_fail;
	}
	
	
	
	
	
cmd_fail:
	/*����ʧ��֮���м�ֹͣ����ֹӰ�������豸*/
	I2C_STOP();
	printf("Read fild!\r\n");
	return 0;
}

  



