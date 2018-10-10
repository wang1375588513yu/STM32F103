#include "smoke_driver.h"

/******************************************************************
**ADC�˿�:
**�ɼ��˿�:ADC1  ͨ��10-PC0
**				 ͨ��11-PC1
*******************************************************************/

#define ADC1_DR_Address    ((u32)0x40012400+0x4c) //(uint32_t)&ADC1->DR

__IO uint16_t ADC_ConvertedValue[50][1];//�������ADCת�������Ҳ��DMA��Ŀ���ַ����ֵ�˲���������50����ƽ��ֵ
__IO uint16_t After_filter[2];    //���������ƽ��ֵ֮��Ľ��


/*******************************************
**�������ܣ�����ADC1ͨ��10-PC0,11-PC1
********************************************/
static void ADC1_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//ADC������APB2��DMA������AHB��
	/* Enable DMA clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	/* Enable ADC1 and GPIOC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC, ENABLE);//RCC_APB2Periph_GPIOC
	
	/* Configure PC.0  as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_4;//GPIO_Pin_4 (AOUT)| GPIO_Pin_12(DOUT)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/**
  * @brief  ����ADC1�Ĺ���ģʽΪMDAģʽ
  * @param  ��
  * @retval ��
  */
static void ADC1_Mode_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	// DMA channel1 configuration 
	DMA_DeInit(DMA1_Channel1);
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;	 //ADC��ַ,
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_ConvertedValue;	//�ڴ��ַ��DMA��Ŀ���ַ��άbuf��dma���˵���
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 50;	//ÿͨ���ɼ�50��,2��ͨ��,50*2��һ�ΰ��˵Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//�����ַ�̶�������ĵ�ַ��Ҫ����
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  			//�ڴ��ַ���� ���ڴ�ļ�DMA��Ŀ��bufҪ����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//���ݿ��Ϊ����
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	//���ݿ��Ϊ16λ��ADM���ݴ�����Ŀ2^16=65536
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;						//ѭ������
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	// Enable DMA channel0 
	DMA_Cmd(DMA1_Channel1, ENABLE);

	// ADC1 configuration 	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;			//����ADCģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE; 	 	//ɨ��ģʽ��ɨ��ģʽ���ڶ�ͨ���ɼ� ENABLE
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//��������ת��ģʽ������ͣ�ؽ���ADCת��
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//��ʹ���ⲿ����ת��
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 	//�ɼ������Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	 			//Ҫת����ͨ����Ŀ2
	ADC_Init(ADC1, &ADC_InitStructure);
	
	/*����ADCʱ�ӣ�ΪPCLK2��8��Ƶ����9MHz*/
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
	
	/*����ADC1��ͨ��10,11Ϊ239.	5���������ڣ�����Ϊ1 */ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_239Cycles5);//ADC_SampleTime_239Cycles5
	
	/* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);//��adc�����ݾ͸�dma
	
	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);
	
	/*��λУ׼�Ĵ��� */   
	ADC_ResetCalibration(ADC1);
	/*�ȴ�У׼�Ĵ�����λ��� */
	while(ADC_GetResetCalibrationStatus(ADC1));
	
	/* ADCУ׼ */
	ADC_StartCalibration(ADC1);
	/* �ȴ�У׼���*/
	while(ADC_GetCalibrationStatus(ADC1));
	
	/* ����û�в����ⲿ����������ʹ���������ADCת�� */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

/*****************************************************
**�������ܣ�ADC1��ʼ��
******************************************************/
void ADC1_Init(void)
{
	ADC1_GPIO_Config();
	ADC1_Mode_Config();
}

//adc������dma�ᵽbuf���������ƽ��
//��ƽ��ֵ����
void filter(u8 num)
{
	int  sum = 0;
	u8  count;    

	for(count=0;count<50;count++)
	{
		sum += ADC_ConvertedValue[count][num];
	}
	After_filter[num]=sum/50;
}

/*
**�������ܣ���ȡXͨ��ADֵ��0~4096��
**����    ��channel---ģ���ͨ����
**����ȡֵ��	10~11
*/
uint16_t Get_Adc(uint8_t channel)
{
	uint8_t ch = 0;
	
	if(channel == 14)
		ch = 0;
	else if(channel == 11)
		ch = 1;
	else return 0;
	
 	filter(ch);
	return 	After_filter[ch];	  	
}



/***************************************
**�������ܣ���ȡ�ɼ���ѹֵ
**��	ʽ��V(sample) = V(REF) * Value/(0x0FFF + 1)
****************************************/
float Get_AdcMath(u16 adc_val)
{
	u16 temp = adc_val;
	float Value;

//	Value = temp * 3.3;
//	Value = Value / (0x0FFF + 1);
	Value = (float)temp * (3.3 / 4096);//VDDA����

	return Value;
}

