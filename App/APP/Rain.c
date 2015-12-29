//用于存储和读取雨量，里面实现队列FLASH
#include "includes.h"

char rain[6144]={'\0'};
int rainnumber = 0;

void rain_gpio(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);  
	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD,&GPIO_InitStructure);

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD,EXTI_PinSource0);

	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;//Pd0
    EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
}



void EXTI0_IRQHandler(void)
{
	OSIntEnter();
	if(EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		//进入低功耗模式进行写入flash,唤醒时候重新加载数据
		rainnumber++;
		if(rainnumber > 350)
		   {rainnumber = 0; memset(rain,'\0',6144);}
		strcat(rain,get_time());
		printf("rainnumber:%d\r\n",rainnumber);
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
	OSIntExit();
}

