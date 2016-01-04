#include "includes.h"
//测量内部的ADC
void voltage_adc_init(void)
{
	GPIO_InitTypeDef  		GPIO_InitStructure;
	ADC_CommonInitTypeDef 	ADC_CommonInitStructure;
	ADC_InitTypeDef       	ADC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	


	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; 
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInit(&ADC_CommonInitStructure);

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	ADC_Cmd(ADC1, ENABLE);
}

unsigned int get_adc_value(void)
{
	int i = 0x1ffff;
	ADC_RegularChannelConfig(ADC1, 10, 1, ADC_SampleTime_480Cycles );			    
  
	ADC_SoftwareStartConv(ADC1);		
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ))
	{
		i--;
		if(i==1)
		{break;}
	}
	
	//printf("adc ok...\r\n");

	return ADC_GetConversionValue(ADC1);
}

double get_dev_voltage(unsigned int value)
{
	double dev_voltage = 0;
	double x = (double)100/(double)570;
	
	dev_voltage = (double)(value*3.3)/(double)4096;
	dev_voltage = dev_voltage/x;
	return dev_voltage;
}

