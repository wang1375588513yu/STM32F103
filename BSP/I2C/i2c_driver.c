/******************************************************************
 **************************** I2C *********************************
 *******************I2C_SCL   PB6 *********************************
 *******************I2C_SDA   PB7 *********************************
 ******************************************************************/


#include "i2c_driver.h"
#include "stm32f10x.h"                  // Device header
#include "SysTick.h"

#define GPIO_PORT_I2C	GPIOB  						/*I2C端口*/
#define RCC_I2C_PORT	RCC_APB2Periph_GPIOB		/*I2C时钟*/
#define I2C_SCL_PIN		GPIO_Pin_6					/*SCL引脚*/
#define I2C_SDA_PIN		GPIO_Pin_7					/*SDA引脚*/

#define I2C_SCL_0()		GPIO_ResetBits(GPIO_PORT_I2C, I2C_SCL_PIN)	/*时钟引脚拉低*/
#define I2C_SCL_1()		GPIO_SetBits(GPIO_PORT_I2C, I2C_SCL_PIN)	/*时钟引脚拉高*/

#define I2C_SDA_0()		GPIO_ResetBits(GPIO_PORT_I2C, I2C_SDA_PIN)	/*数据引脚拉低*/
#define I2C_SDA_1()		GPIO_SetBits(GPIO_PORT_I2C, I2C_SDA_PIN)	/*数据引脚拉高*/

#define I2C_SDA_READ()	GPIO_ReadInputDataBit(GPIO_PORT_I2C, I2C_SDA_PIN)	/*读取数据总线的电平*/

/**************************************
**函数功能:初始化GPIO
**************************************/
void I2C_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//初始化时钟
	
	GPIO_InitStruct.GPIO_Pin = I2C_SCL_PIN | I2C_SDA_PIN;	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;			
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIO_PORT_I2C, &GPIO_InitStruct);
	GPIO_SetBits(GPIO_PORT_I2C, I2C_SCL_PIN | I2C_SDA_PIN);		//设置初始时刻为高电平
}
/**************************************
**函数功能:数据输入
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
**函数功能:数据输出
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
**函数功能:起始信号
**************************************/
void I2C_START(void)
{
	SDA_OUT();		//mode_out
	I2C_SCL_1();	//时钟为高电平
	I2C_SDA_1();	//数据线为高电平
	delay_us(8);
	I2C_SDA_0();	//数据线拉低
	delay_us(8);
	I2C_SCL_0();	//时钟线拉低
}

/**************************************
**函数功能:停止信号
**************************************/
void I2C_STOP(void)
{
	SDA_OUT();		//mode_out
	I2C_SDA_0();	//数据为低电平
	I2C_SCL_1();	//时钟为高电平
	delay_us(8);
	I2C_SDA_1();	//数据线拉高
	delay_us(8);
}

/**************************************
**函数功能:发送应答信号
**参	数:ack----0---->ACK 肯定的应答
**		      ----1---->NACK 否定的应答
**************************************/
void I2C_SendACK(u8 ack)
{
	SDA_OUT();		//mode_out
	I2C_SCL_0();	//时钟为低电平，为发送状态
	delay_us(8);
	if(ack)
		I2C_SDA_0();	//当为真的时候是发送NACK
	else
		I2C_SDA_1();	//当为假的时候是发送ACK
	delay_us(8);
	I2C_SCL_1();	//时钟拉高，从机读取
	delay_us(8);
	I2C_SCL_0();	//时钟拉低，回到发送状态
	delay_us(8);
}

/**************************************
**函数功能:接收应答信号
**返回	值:0-----ACK
**		   1-----NACK
**************************************/
u8 I2C_ReceiveACK(void)
{
	u8 ucErrTime=0;
	
	SDA_IN();		//mode in
	I2C_SDA_1();
	delay_us(4);
	I2C_SCL_1();	//接收应答信号时，时钟信号应该为高电平
	delay_us(4);
	
	//读取SDA的电平值，当读到的电平一直是1，返回1，当中间有一次是0，则跳出循环
	while(I2C_SDA_READ())
	{	
		ucErrTime++;
		if(ucErrTime>250)
		{	//在250次里面每次读到的都是高电平
			I2C_STOP();	//执行停止信号
			return 1;	//返回1
		}
	}
	I2C_SCL_0();
	return 0;
}

/**************************************
**函数功能:发送1Byte数据
**参	数:dat-----要发送的数据
**************************************/
void I2C_SendByte(u8 dat)
{
	u8 i = 0;
	
	SDA_OUT();	//输出模式，主机给从机发送
	for(i = 0; i < 8; i++)
	{		//判断是1还是0，最高位
		I2C_SCL_0();	//在时钟为低电平的时候发送
		if(dat & 0x80)
			I2C_SDA_1();//发送1
		else 
			I2C_SDA_0();//发送0
		dat <<= 1;	//左移一位
		delay_us(5);
		I2C_SCL_1();	//时钟拉高，从机读取
		delay_us(5); 
	}
}


/**************************************
**函数功能:读取1Byte数据
**返回	值:读取到的数据
**************************************/	
u8 I2C_RecvByte(void)
{
	u8 i = 0;
	u8 byte = 0;
	
	SDA_IN();	//主机输入模式
	for(i = 0; i < 8; i++)
	{
		delay_us(5);
		I2C_SCL_1();	//时钟为高电平的时候接收信号
		delay_us(5);
		byte <<= 1;
		if(I2C_SDA_READ())
			byte |= 0x01;
		I2C_SCL_0();	//时钟拉低，从机发信号
		delay_us(5);
	}
	
	return 0;
}

/****************************************************************************
*	函 数 名: i2c_CheckDevice
*	功能说明: 检测I2C总线设备，CPU向发送设备地址，然后读取设备应答来判断该设备是否存在
*	形    参：_Address：设备的I2C总线地址
*	返 回 值: 返回值 0 表示正确， 返回1表示未探测到
****************************************************************************/
uint8_t I2C_CheckDevice(uint8_t _Address)
{
	u8 ucACK;
	
	I2C_START();	//起始信号
	
	I2C_SendByte(_Address | I2C_WR);	//发送设备地址加读写控制位（0 = w， 1 = r)
	ucACK = I2C_ReceiveACK();	//接收应答信号
	
	I2C_STOP();		//停止信号
	
	return ucACK;
}






