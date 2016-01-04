//���ڴ洢�Ͷ�ȡ����������ʵ�ֶ���FLASH
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
*����10�����ݾͼ�¼��flash����ȥ
*��д����
*/
void EXTI0_IRQHandler(void)
{
	int DataNumber = 0;
	OSIntEnter();
	if(EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		RainNumber_RAM++;
		//RAM���������������񳬹���10��
		//������������д�뵽flash���� ,ram��������,flash����������������б��
		if(RainNumber_RAM > 10)
		   {
			DataNumber = *(int *)(RAINNUMBERS_ADDR);//��ȡflash����洢����
			//printf("DataNumber:%d",DataNumber);
			   
			   
			MCUFlashUnlock();
			//get_time���ص�ֵ�����ʱ������bin��ʽ���ֵģ����Ե�ʱ��Ϊ��λ����ʱ�򣬲����ǡ�0x�� ���ǡ�x�������²���21���ֽ�,
			flash_w_buffer( RAINDATA_ADDR + (DataNumber*20), Rain_RAM);//д�����ݵ�flash 21���ֽڴ洢һ������

			/*���flash����洢������ֵ*/
			while(MCUFlashErase(FLASH_Sector_10) != 1);//sector10
			while(FLASH_ProgramWord(RAINNUMBERS_ADDR, (DataNumber + 10)) != FLASH_COMPLETE);
			FLASH_Lock();

			RainNumber_RAM = 0; 
			memset(Rain_RAM,'\0',1024);//����
		   }
		   else
		   {
			//û�г������������
			strcat(Rain_RAM,get_time());
			//printf("rainnumber:%d\r\n",RainNumber_RAM);
		   }
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
	OSIntExit();
}

