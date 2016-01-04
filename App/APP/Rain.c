//用于存储和读取雨量，里面实现队列FLASH
#include "includes.h"

char Rain_RAM[1024]={'\0'};
int RainNumber_RAM = 0;


void rain_gpio(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD,&GPIO_InitStructure);

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD,EXTI_PinSource0);

	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_Line    = EXTI_Line0;//Pd0
    EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
}


/*
*超过10组数据就记录到flash里面去
*改写个数
*/
void EXTI0_IRQHandler(void)
{
	int DataNumber = 0;
	OSIntEnter();
	if(EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		RainNumber_RAM++;
		//RAM里面的数据数量书否超过了10组
		//超过了则将数据写入到flash里面 ,ram进行清零,flash保存的数据总量进行变更
		if(RainNumber_RAM > 10)
		   {
			DataNumber = *(int *)(RAINNUMBERS_ADDR);//获取flash里面存储数量
			//printf("DataNumber:%d",DataNumber);
			   
			   
			MCUFlashUnlock();
			//get_time返回的值里面的时间是以bin格式体现的，所以当时间为个位数的时候，不会是‘0x’ 而是‘x’，导致不够21个字节,
			flash_w_buffer( RAINDATA_ADDR + (DataNumber*20), Rain_RAM);//写入数据到flash 21个字节存储一组数据

			/*变更flash里面存储的数量值*/
			while(MCUFlashErase(FLASH_Sector_10) != 1);//sector10
			while(FLASH_ProgramWord(RAINNUMBERS_ADDR, (DataNumber + 10)) != FLASH_COMPLETE);
			FLASH_Lock();

			RainNumber_RAM = 0; 
			memset(Rain_RAM,'\0',1024);//清零
		   }
		   else
		   {
			//没有超过则继续保持
			strcat(Rain_RAM,get_time());
			//printf("rainnumber:%d\r\n",RainNumber_RAM);
		   }
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
	OSIntExit();
}

