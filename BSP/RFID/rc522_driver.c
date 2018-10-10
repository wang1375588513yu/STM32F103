#include "rc522_driver.h"
#include "stm32f10x.h"
#include <string.h> 
#include "SysTick.h"
#include "usart_driver.h"
#include "spi_driver.h"
#include "Led_driver.h"
#include "beep_driver.h"

#define MAXRLEN 18 
/*
**�������ܣ�RIFD-RC522�˿ڳ�ʼ��
*/
void MF522_io_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	/*GPIOC13 ��� RST*/
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		    // �������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	MF522_RST_HIGH();
}
/******************************
//��    �ܣ���λRC522
//��    ��: �ɹ�����MI_OK
*******************************/
signed char PcdReset(void)
{
	MF522_RST_HIGH();	//��λ������λ
	delay_us(1);
	MF522_RST_LOW();	//��λ������0
	delay_us(1);
	MF522_RST_HIGH();	//��λ������λ
	delay_us(1);
	WriteRawRC(CommandReg,PCD_RESETPHASE);				 //����������
	delay_us(1);
	
	WriteRawRC(ModeReg,0x3D);            //��Mifare��ͨѶ��CRC��ʼֵ0x6363
	WriteRawRC(TReloadRegL,30);           
	WriteRawRC(TReloadRegH,0);
	WriteRawRC(TModeReg,0x8D);			
	WriteRawRC(TPrescalerReg,0x3E);
	WriteRawRC(TxAutoReg,0x40);  
	   
	return MI_OK;
}
/**************************************************************
**��    �ܣ�дRC522�Ĵ���
**����˵����Address[IN]:�Ĵ�����ַ
**          value[IN]:д���ֵ
**************************************************************/
void WriteRawRC(unsigned char Address, unsigned char value)
{  
    u8   ucAddr;

	MF522_NSS_LOW();	   //ȡ��ƬѡR522
    ucAddr = ((Address<<1)&0x7E);

	SPI_MRC_RW(ucAddr);
	SPI_MRC_RW(value);
	MF522_NSS_HIGH();		//��Ƭѡ
}
/********************************************************
**�������ܣ��ر�����  
**			ÿ��������ر����շ���֮��Ӧ������1ms�ļ��
*********************************************************/
void PcdAntennaOff(void)
{
    ClearBitMask(TxControlReg, 0x03);
}


/********************************************************
**�������ܣ��������� 
**			ÿ��������ر����շ���֮��Ӧ������1ms�ļ��
*********************************************************/
void PcdAntennaOn(void)
{
    unsigned char i;

    i = ReadRawRC(TxControlReg);

    if (!(i & 0x03))
    {
        SetBitMask(TxControlReg, 0x03);
    }
}

/****************************************************
**�������ܣ�����RC522�Ĺ�����ʽ 
****************************************************/
char M500PcdConfigISOType(unsigned char type)
{
	if (type == 'A')                     //ISO14443_A
	{ 
		ClearBitMask(Status2Reg,0x08);
		
		/*     WriteRawRC(CommandReg,0x20);    //as default   
		WriteRawRC(ComIEnReg,0x80);     //as default
		WriteRawRC(DivlEnReg,0x0);      //as default
		WriteRawRC(ComIrqReg,0x04);     //as default
		WriteRawRC(DivIrqReg,0x0);      //as default
		WriteRawRC(Status2Reg,0x0);//80    //trun off temperature sensor
		WriteRawRC(WaterLevelReg,0x08); //as default
		WriteRawRC(ControlReg,0x20);    //as default
		WriteRawRC(CollReg,0x80);    //as default
		*/
		WriteRawRC(ModeReg,0x3D);//3F
		/*	   WriteRawRC(TxModeReg,0x0);      //as default???
		WriteRawRC(RxModeReg,0x0);      //as default???
		WriteRawRC(TxControlReg,0x80);  //as default???
		
		WriteRawRC(TxSelReg,0x10);      //as default???
		*/
		WriteRawRC(RxSelReg,0x86);//84
		//      WriteRawRC(RxThresholdReg,0x84);//as default
		//      WriteRawRC(DemodReg,0x4D);      //as default
		
		//      WriteRawRC(ModWidthReg,0x13);//26
		WriteRawRC(RFCfgReg,0x7F);   //4F
		/*   WriteRawRC(GsNReg,0x88);        //as default???
		WriteRawRC(CWGsCfgReg,0x20);    //as default???
		WriteRawRC(ModGsCfgReg,0x20);   //as default???
		*/
		WriteRawRC(TReloadRegL,30);//tmoLength);// TReloadVal = 'h6a =tmoLength(dec) 
		WriteRawRC(TReloadRegH,0);
		WriteRawRC(TModeReg,0x8D);
		WriteRawRC(TPrescalerReg,0x3E);
		
	
		//     PcdSetTmo(106);
		delay_ms(10);
		PcdAntennaOn();
	}
	else
	{ 
		printf("M500PcdConfigISOType MI_NOTAGERR!----->1");
		return (char)MI_NOTAGERR; 
	}
	
	return MI_OK;
}
/******************************************************
**��    �ܣ���RC522�Ĵ���λ
**����˵����reg[IN]:�Ĵ�����ַ
**          mask[IN]:��λֵ
*******************************************************/
void ClearBitMask(unsigned char reg,unsigned char mask)  
{
    char tmp = 0x0;
    tmp = ReadRawRC(reg);
    WriteRawRC(reg, tmp & ~mask);  // clear bit mask
} 	
/***********************************
**�������ܣ�RC522��ʼ��
***********************************/
void RC522_Init(void)
{
	MF522_io_init(); 	//�˿ڳ�ʼ��
	PcdReset(); 	//��λRC522
	PcdAntennaOff(); 	//�ر����� 
	PcdAntennaOn();  	//������
	M500PcdConfigISOType( 'A' );	//����RC522�Ĺ�����ʽ 
	delay_ms(100);	
	delay_ms(100);

}
/**************************************************************
**�������ܣ�������--ɨ�迨������ײ��ѡ������֤A��B������
**��		����g_ucTempbuf---------��Ƭ���ʹ���
**				serial_number-------��Ƭ���кŴ洢
**����	ֵ��ret == 5ʱ�����в�����ȷ�������Ǵ������
**************************************************************/
int Card_handle(unsigned char* g_ucTempbuf,unsigned char* serial_number)
{
	u8 status;
	u8 temp;
	int i = 0;
	int ret = 0;
	unsigned char  DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
	
	status = PcdRequest(PICC_REQALL,g_ucTempbuf);/*ɨ�迨*/
	
	printf("\n status = %d  \n",(int)status);
	
	if(status==0)	//ɨ��ɹ�
	{	
		printf("��������:");
		for(i=0;i<2;i++)
		{	
			temp=g_ucTempbuf[i];
			printf("%X",temp);					
		}
		ret = 1;
	}
	else
	{
		//��ɨ��һ��
		status = PcdRequest(PICC_REQALL,g_ucTempbuf);/*ɨ�迨*/
		printf("\n status = %d  \n", (int)status);
		
		
		if(status==0)	//ɨ��ɹ�
		{	
			printf("��������:");
			for(i=0;i<2;i++)
			{	
				temp=g_ucTempbuf[i];
				printf("%X",temp);					
			}
			ret = 1;
		}
		else
		{
			printf("\r\nɨ��ʧ��\r\n");
			ret = -1;
		}
	}
		
	status = PcdAnticoll(g_ucTempbuf);/*����ײ*/ 
	printf("\n status = %d  \n", (int)status);
	
	if(status == 0)	//�����кųɹ�
	{ 
		printf("\r\n�������");
		for(i=0;i<4;i++)
		{
			temp=g_ucTempbuf[i];
			printf("%X",temp);		
			if(serial_number != NULL)
			{
				serial_number[i] = temp;
			}
		}
		ret += 1;
	}
	else
	{
		ret -= 1;
		printf("\r\n����ײʧ��\r\n");
	}
	
	status = PcdSelect(g_ucTempbuf);//ѡ����Ƭ
	printf("\n status = %d  \n", (int)status);
	
	if(status == 0)
	{
		ret += 1;
		printf("\r\n��ѡ���ɹ�\n");
	} 
	else 
	{
		printf("\r\n��ѡ��ʧ��\r\n");
		ret -= 1;
	}
	
	if(ret == 3)
	{
		//У��A����
		status = PcdAuthState(PICC_AUTHENT1A, 1, DefaultKey, g_ucTempbuf);//��֤��Ƭ���� 
		printf("\n status = %d  \n",(int)status);
		
		if (status == 0)
		{   
			ret += 1;
			printf("У��A����ɹ�\r\n");
		}
		else 
		{
			ret -= 1;
			printf("У��A����ʧ��\r\n");
		}
		
		//У��B����
		status = PcdAuthState(PICC_AUTHENT1B, 1, DefaultKey, g_ucTempbuf);//��֤��Ƭ����   
		printf("\n status = %d  \n",(int)status);
		
		if (status == 0)
		{   
			ret += 1;
			printf("У��B����ɹ�\r\n");
		}
		else 
		{
			ret -= 1;
			printf("У��B����ʧ��\r\n");
		}
	}
	
	return  ret;
}
/*****************************************************************
**��    �ܣ�Ѱ��  /ɨ�迨
**����˵��: req_code[IN]:Ѱ����ʽ
**                0x52 = Ѱ��Ӧ�������з���14443A��׼�Ŀ�
**                0x26 = Ѱδ��������״̬�Ŀ�
**          pTagType[OUT]����Ƭ���ʹ���
**                0x4400 = Mifare_UltraLight
**                0x0400 = Mifare_One(S50)
**                0x0200 = Mifare_One(S70)
**                0x0800 = Mifare_Pro(X)
**                0x4403 = Mifare_DESFire
**��    ��: �ɹ�����MI_OK
******************************************************************/
char PcdRequest(unsigned char req_code,unsigned char *pTagType)
{
	char status;  
	unsigned int  unLen;
	unsigned char ucComMF522Buf[MAXRLEN]; 
	//unsigned char xTest ;
	

	
	ClearBitMask(Status2Reg,0x08);
	WriteRawRC(BitFramingReg,0x07);
/*****************����***********************/	
//	xTest = ReadRawRC(BitFramingReg);
//	if(xTest == 0x07 )
//	{ 
//		printf("PcdRequest WRITE and READ SUCCEED!!\r\n");
////		while(1);
//	}
//	else 
//	{
//		printf("PcdRequest WRITE and READ ERROR!!\r\n");
//		while(1);
//	}
/********************************************/

	SetBitMask(TxControlReg,0x03);
	
	ucComMF522Buf[0] = req_code;
	
	status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,1,ucComMF522Buf,&unLen);

/**************************����****************************/
//	if(status  == MI_OK )
//	{ 
//		printf("eturn status OK!!\r\n");
//		while(1);
//	}
//	else 
//	{
//		printf("return status ERROR!!\r\n");
//		while(1);
//	}
/******************************************************/
	if ((status == MI_OK) && (unLen == 0x10))
	{    
		*pTagType     = ucComMF522Buf[0];
		*(pTagType+1) = ucComMF522Buf[1];
	}
	else
	{   
		status = (char)MI_ERR; 
			printf("PcdRequest() MI_NOTAGERR! \n"); 
	}
	
	return status;
}

/*******************************************************************
//��    �ܣ�ͨ��RC522��ISO14443��ͨѶ
//����˵����Command[IN]:RC522������
//          pInData[IN]:ͨ��RC522���͵���Ƭ������
//          InLenByte[IN]:�������ݵ��ֽڳ���
//          pOutData[OUT]:���յ��Ŀ�Ƭ��������
//          *pOutLenBit[OUT]:�������ݵ�λ����
********************************************************************/
char PcdComMF522(unsigned char Command, 
                 unsigned char *pInData, 
                 unsigned char InLenByte,
                 unsigned char *pOutData,  
                 unsigned int  *pOutLenBit)
{
	char status = 0;
	//char status = (char)MI_ERR;
	unsigned char irqEn   = 0x00;
	unsigned char waitFor = 0x00;
	unsigned char lastBits;
	unsigned char n;
	unsigned int i;
	switch (Command)
	{
		case PCD_AUTHENT:
			irqEn   = 0x12;
			waitFor = 0x10;
			break;
		case PCD_TRANSCEIVE:
			irqEn   = 0x77;
			waitFor = 0x30;
			break;
			default:
		break;
	}
	
	WriteRawRC(ComIEnReg,irqEn|0x80);
	ClearBitMask(ComIrqReg,0x80);
	WriteRawRC(CommandReg,PCD_IDLE);
	SetBitMask(FIFOLevelReg,0x80);
		
	for (i=0; i<InLenByte; i++)
	{   
		WriteRawRC(FIFODataReg, pInData[i]);    
	}

	WriteRawRC(CommandReg, Command);
	
	
	if (Command == PCD_TRANSCEIVE)
	{    
		SetBitMask(BitFramingReg,0x80);  }
	
		//    i = 600;//����ʱ��Ƶ�ʵ���������M1�����ȴ�ʱ��25ms
		i = 2000;
	do 
	{
		n = ReadRawRC(ComIrqReg);
		i--;
	}
	while ((i!=0) && !(n&0x01) && !(n&waitFor));
	ClearBitMask(BitFramingReg,0x80);
	
	if (i!=0)
	{    
		if(!(ReadRawRC(ErrorReg)&0x1B))
		{
			status = 0;
			if (n & irqEn & 0x01)
			{   
				status = (char)MI_NOTAGERR; 
        printf("PcdComMF522 MI_NOTAGERR!----->(1)"); 				
			}

			if (Command == PCD_TRANSCEIVE)
			{
				n = ReadRawRC(FIFOLevelReg);
				lastBits = ReadRawRC(ControlReg) & 0x07;
				if (lastBits)
				{   
					*pOutLenBit = (n-1)*8 + lastBits;   
				}
				else
				{   
					*pOutLenBit = n*8;   
				}

				if (n == 0)
				{   
					n = 1;    
				}

				if (n > MAXRLEN)
				{   
					n = MAXRLEN;   
				}

				for (i=0; i<n; i++)
				{   
					pOutData[i] = ReadRawRC(FIFODataReg);    
				}
			}
		}
		else
		{  
			status = (char)MI_ERR;
			printf("PcdComMF522 MI_ERR!------> 2"); 
			//status = 0;
			
		}
	}
	
	SetBitMask(ControlReg,0x80);           // stop timer now
	WriteRawRC(CommandReg,PCD_IDLE); 
	
	return status;
} 

/********************************************
**��    �ܣ�����ײ
**����˵��: pSnr[OUT]:��Ƭ���кţ�4�ֽ�
**��    ��: �ɹ�����MI_OK
*********************************************/  
char PcdAnticoll(unsigned char *pSnr)
{
    char status;
    unsigned char i,snr_check=0;
    unsigned int  unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 
    

    ClearBitMask(Status2Reg,0x08);
    WriteRawRC(BitFramingReg,0x00);
    ClearBitMask(CollReg,0x80);
 
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x20;

    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,2,ucComMF522Buf,&unLen);

    if (status == MI_OK)
    {
    	 for (i=0; i<4; i++)
         {   
             *(pSnr+i)  = ucComMF522Buf[i];
             snr_check ^= ucComMF522Buf[i];
         }
         if (snr_check != ucComMF522Buf[i])
         {   
					status = (char)MI_ERR; 
					printf("PcdAnticoll MI_ERR!----->1");
					 
				 }
    }
    
    SetBitMask(CollReg,0x80);
    return status;
}
/********************************************************************
**��    �ܣ�ѡ����Ƭ
**����˵��: pSnr[IN]:��Ƭ���кţ�4�ֽ�
**��    ��: �ɹ�����MI_OK
***********************************************************************/
char PcdSelect(unsigned char *pSnr)
{
    char status;
    unsigned char i;
    unsigned int  unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x70;
    ucComMF522Buf[6] = 0;
    for (i=0; i<4; i++)
    {
    	ucComMF522Buf[i+2] = *(pSnr+i);
    	ucComMF522Buf[6]  ^= *(pSnr+i);
    }
    CalulateCRC(ucComMF522Buf,7,&ucComMF522Buf[7]);
  
    ClearBitMask(Status2Reg,0x08);

    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,9,ucComMF522Buf,&unLen);
    
    if ((status == MI_OK) && (unLen == 0x18))
    {   status = MI_OK;  }
    else
    {   
			status = (char)MI_ERR; 
			printf("PcdSelect MI_ERR!------>1");	
		}

    return status;
}
/*******************************************************************************
**�������ܣ���MF522����CRC16����
********************************************************************************/
void CalulateCRC(unsigned char *pIndata,unsigned char len,unsigned char *pOutData)
{
    unsigned char i,n;

    ClearBitMask(DivIrqReg,0x04);
    WriteRawRC(CommandReg,PCD_IDLE);
    SetBitMask(FIFOLevelReg,0x80);

    for (i=0; i<len; i++)
    {   
		WriteRawRC(FIFODataReg, *(pIndata+i));   
	}
    WriteRawRC(CommandReg, PCD_CALCCRC);

    i = 0xFF;

    do 
    {
        n = ReadRawRC(DivIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x04));

    pOutData[0] = ReadRawRC(CRCResultRegL);
    pOutData[1] = ReadRawRC(CRCResultRegM);
}
/************************************************************************
**��    �ܣ���֤��Ƭ����
**����˵��: auth_mode[IN]: ������֤ģʽ
**                 0x60 = ��֤A��Կ
**                 0x61 = ��֤B��Կ 
**          addr[IN]�����ַ
**          pKey[IN]������
**          pSnr[IN]����Ƭ���кţ�4�ֽ�
**��    ��: �ɹ�����MI_OK
*************************************************************************/               
char PcdAuthState(unsigned char auth_mode,unsigned char addr,unsigned char *pKey,unsigned char *pSnr)
{
    char status;
    unsigned int  unLen;
    unsigned char i,ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = auth_mode;
    ucComMF522Buf[1] = addr;
    for (i=0; i<6; i++)
    {    ucComMF522Buf[i+2] = *(pKey+i);   }
		for (i=0; i<6; i++)
		{    ucComMF522Buf[i+8] = *(pSnr+i);   }
 //   memcpy(&ucComMF522Buf[2], pKey, 6); 
 //   memcpy(&ucComMF522Buf[8], pSnr, 4); 
    
    status = PcdComMF522(PCD_AUTHENT,ucComMF522Buf,12,ucComMF522Buf,&unLen);
    if ((status != MI_OK) || (!(ReadRawRC(Status2Reg) & 0x08)))
    {   
			status = (char)MI_ERR; 
			printf("PcdAuthState MI_ERR!------->1");		
		}
    	status=0x00;
    return status;
}
/**********************************
**�������ܣ������û����
**					������0��1�ж�ȡ
**��		����g_ucTempbuf---------��Ƭ���ʹ���
**					���ɹ����û���Ŵ����g_ucTempbuf�У���ʱ��ȡ����
**����	ֵ��ret == 1ʱ����ʾ��ֵ�ɹ�
**********************************/
int Card_Look_User(unsigned char* g_ucTempbuf)
{
	int ret = 0;
	u8 status  = 0;
	
	status = Card_handle(g_ucTempbuf,NULL);//ɨ�迨�ȵ�
	if(status == 5)		//ֻ��ɨ��ɹ�������������һֱ��������֤�ɹ����Ž�����Ϣ��д��
	{
		status = PcdRead(0x01, g_ucTempbuf);//����1
		if(status==0)
		{
			ret = 1;
			Beep_On();
			delay_ms(500);
			Beep_Off();
			
		Led_Ctl(LED_RED,OFF);
		Led_Ctl(LED_GREEN,ON);
			printf("�鿴�û���Ϣ�ɹ�\r\n");
		}
		else
		{
			ret = -1;
			printf("�鿴�û���Ϣʧ��\r\n");
		}
	}
	
	return ret;
}

/**********************************************************
**��    �ܣ���ȡM1��һ������
**����˵��: addr[IN]�����ַ
**          pData[OUT]�����������ݣ�16�ֽ�
**��    ��: �ɹ�����MI_OK
***********************************************************/ 
char PcdRead(unsigned char addr,unsigned char *pData)
{
    char status;
    unsigned int  unLen;
    unsigned char i,ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_READ;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
   
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
    if ((status == MI_OK) && (unLen == 0x90))
 //   {   memcpy(pData, ucComMF522Buf, 16);   }
    {
        for (i=0; i<16; i++)
        {    *(pData+i) = ucComMF522Buf[i];   }
    }
    else
    {   
			status = (unsigned char)MI_ERR; 
			printf("PcdRead MI_ERR!----->1");
		}
    
    return status;
}

/****************************************************
**��    �ܣ���RC522�Ĵ���λ
**����˵����reg[IN]:�Ĵ�����ַ
**          mask[IN]:��λֵ
*****************************************************/
void SetBitMask(unsigned char reg,unsigned char mask)  
{
    char tmp = 0x0;
    tmp = ReadRawRC(reg);
    WriteRawRC(reg,tmp | mask);  // set bit mask
}

/******************************************
//��    �ܣ���RC522�Ĵ���
//����˵����Address[IN]:�Ĵ�����ַ
//��    �أ�������ֵ
*******************************************/
u8 ReadRawRC(unsigned char Address)
{
	u8   ucAddr;
	u8   ucResult=0;

	MF522_NSS_LOW();	   //ȡ��ƬѡR522
	ucAddr = ((Address<<1)&0x7E)|0x80;
	
	SPI_MRC_RW(ucAddr);
	ucResult = SPI_MRC_RW(0); 	//��SPI2FIFO��ȡ����
	MF522_NSS_HIGH();		//��Ƭѡ

//	printf("ucResult = %d\r\n",ucResult);
	return ucResult;
}
/****************************************************
**�������ܣ����ڶ�/дһ�ֽ�����
**��	����dat
**����	ֵ����ȡ�õ�����/״̬ 
*****************************************************/
u8 SPI_MRC_RW(u8 dat)
{  	
	return RFID_SPI_WriterByte(dat);
}


void RFID_Test(void)
{
	unsigned char g_ucTempbuf[20];
	Led_Ctl(LED_GREEN,OFF);
	Led_Ctl(LED_RED,ON);
//	//ѭ��ɨ��IC��Ƶ��
	if(Card_handle(g_ucTempbuf,NULL) == 5)//ɨ�赽IC��Ƶ��
	{
		uint8_t i = 0;
		//��ȡ�û���Ϣ���
		Card_Look_User(g_ucTempbuf);
		printf("CARD ID:");
		for(i = 0;i < 20;i++)
		{
			printf("%d \r\n",g_ucTempbuf[i]);
		}
		delay_ms(1000);
		delay_ms(1000);
	}
}
