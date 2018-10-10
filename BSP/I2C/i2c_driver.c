/******************************************************************
 **************************** I2C *********************************
 *******************I2C_SCL   PB6 *********************************
 *******************I2C_SDA   PB7 *********************************
 ******************************************************************/


#include "i2c_driver.h"
#include "stm32f10x.h"                  // Device header
#include "SysTick.h"

#define GPIO_PORT_I2C	GPIOB  						/*I2C�˿�*/
#define RCC_I2C_PORT	RCC_APB2Periph_GPIOB		/*I2Cʱ��*/
#define I2C_SCL_PIN		GPIO_Pin_6					/*SCL����*/
#define I2C_SDA_PIN		GPIO_Pin_7					/*SDA����*/

#define I2C_SCL_0()		GPIO_ResetBits(GPIO_PORT_I2C, I2C_SCL_PIN)	/*ʱ����������*/
#define I2C_SCL_1()		GPIO_SetBits(GPIO_PORT_I2C, I2C_SCL_PIN)	/*ʱ����������*/

#define I2C_SDA_0()		GPIO_ResetBits(GPIO_PORT_I2C, I2C_SDA_PIN)	/*������������*/
#define I2C_SDA_1()		GPIO_SetBits(GPIO_PORT_I2C, I2C_SDA_PIN)	/*������������*/

#define I2C_SDA_READ()	GPIO_ReadInputDataBit(GPIO_PORT_I2C, I2C_SDA_PIN)	/*��ȡ�������ߵĵ�ƽ*/

/**************************************
**��������:��ʼ��GPIO
**************************************/
void I2C_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//��ʼ��ʱ��
	
	GPIO_InitStruct.GPIO_Pin = I2C_SCL_PIN | I2C_SDA_PIN;	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;			
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIO_PORT_I2C, &GPIO_InitStruct);
	GPIO_SetBits(GPIO_PORT_I2C, I2C_SCL_PIN | I2C_SDA_PIN);		//���ó�ʼʱ��Ϊ�ߵ�ƽ
}
/**************************************
**��������:��������
**************************************/
static void SDA_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = I2C_SDA_PIN;	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;			
//	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIO_PORT_I2C, &GPIO_InitStruct);
	GPIO_SetBits(GPIO_PORT_I2C,I2C_SDA_PIN);
}
/**************************************
**��������:�������
**************************************/
static void SDA_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = I2C_SDA_PIN;	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;			
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIO_PORT_I2C, &GPIO_InitStruct);
	GPIO_SetBits(GPIO_PORT_I2C,I2C_SDA_PIN);
}

/**************************************
**��������:��ʼ�ź�
**************************************/
void I2C_START(void)
{
	SDA_OUT();		//mode_out
	I2C_SCL_1();	//ʱ��Ϊ�ߵ�ƽ
	I2C_SDA_1();	//������Ϊ�ߵ�ƽ
	delay_us(8);
	I2C_SDA_0();	//����������
	delay_us(8);
	I2C_SCL_0();	//ʱ��������
}

/**************************************
**��������:ֹͣ�ź�
**************************************/
void I2C_STOP(void)
{
	SDA_OUT();		//mode_out
	I2C_SDA_0();	//����Ϊ�͵�ƽ
	I2C_SCL_1();	//ʱ��Ϊ�ߵ�ƽ
	delay_us(8);
	I2C_SDA_1();	//����������
	delay_us(8);
}

/**************************************
**��������:����Ӧ���ź�
**��	��:ack----0---->ACK �϶���Ӧ��
**		      ----1---->NACK �񶨵�Ӧ��
**************************************/
void I2C_SendACK(u8 ack)
{
	SDA_OUT();		//mode_out
	I2C_SCL_0();	//ʱ��Ϊ�͵�ƽ��Ϊ����״̬
	delay_us(8);
	if(ack)
		I2C_SDA_0();	//��Ϊ���ʱ���Ƿ���NACK
	else
		I2C_SDA_1();	//��Ϊ�ٵ�ʱ���Ƿ���ACK
	delay_us(8);
	I2C_SCL_1();	//ʱ�����ߣ��ӻ���ȡ
	delay_us(8);
	I2C_SCL_0();	//ʱ�����ͣ��ص�����״̬
	delay_us(8);
}

/**************************************
**��������:����Ӧ���ź�
**����	ֵ:0-----ACK
**		   1-----NACK
**************************************/
u8 I2C_ReceiveACK(void)
{
	u8 ucErrTime=0;
	
	SDA_IN();		//mode in
	I2C_SDA_1();
	delay_us(4);
	I2C_SCL_1();	//����Ӧ���ź�ʱ��ʱ���ź�Ӧ��Ϊ�ߵ�ƽ
	delay_us(4);
	
	//��ȡSDA�ĵ�ƽֵ���������ĵ�ƽһֱ��1������1�����м���һ����0��������ѭ��
	while(I2C_SDA_READ())
	{	
		ucErrTime++;
		if(ucErrTime>250)
		{	//��250������ÿ�ζ����Ķ��Ǹߵ�ƽ
			I2C_STOP();	//ִ��ֹͣ�ź�
			return 1;	//����1
		}
	}
	I2C_SCL_0();
	return 0;
}

/**************************************
**��������:����1Byte����
**��	��:dat-----Ҫ���͵�����
**************************************/
void I2C_SendByte(u8 dat)
{
	u8 i = 0;
	
	SDA_OUT();	//���ģʽ���������ӻ�����
	for(i = 0; i < 8; i++)
	{		//�ж���1����0�����λ
		I2C_SCL_0();	//��ʱ��Ϊ�͵�ƽ��ʱ����
		if(dat & 0x80)
			I2C_SDA_1();//����1
		else 
			I2C_SDA_0();//����0
		dat <<= 1;	//����һλ
		delay_us(5);
		I2C_SCL_1();	//ʱ�����ߣ��ӻ���ȡ
		delay_us(5); 
	}
}


/**************************************
**��������:��ȡ1Byte����
**����	ֵ:��ȡ��������
**************************************/	
u8 I2C_RecvByte(void)
{
	u8 i = 0;
	u8 byte = 0;
	
	SDA_IN();	//��������ģʽ
	for(i = 0; i < 8; i++)
	{
		delay_us(5);
		I2C_SCL_1();	//ʱ��Ϊ�ߵ�ƽ��ʱ������ź�
		delay_us(5);
		byte <<= 1;
		if(I2C_SDA_READ())
			byte |= 0x01;
		I2C_SCL_0();	//ʱ�����ͣ��ӻ����ź�
		delay_us(5);
	}
	
	return 0;
}

/****************************************************************************
*	�� �� ��: i2c_CheckDevice
*	����˵��: ���I2C�����豸��CPU�����豸��ַ��Ȼ���ȡ�豸Ӧ�����жϸ��豸�Ƿ����
*	��    �Σ�_Address���豸��I2C���ߵ�ַ
*	�� �� ֵ: ����ֵ 0 ��ʾ��ȷ�� ����1��ʾδ̽�⵽
****************************************************************************/
uint8_t I2C_CheckDevice(uint8_t _Address)
{
	u8 ucACK;
	
	I2C_START();	//��ʼ�ź�
	
	I2C_SendByte(_Address | I2C_WR);	//�����豸��ַ�Ӷ�д����λ��0 = w�� 1 = r)
	ucACK = I2C_ReceiveACK();	//����Ӧ���ź�
	
	I2C_STOP();		//ֹͣ�ź�
	
	return ucACK;
}






