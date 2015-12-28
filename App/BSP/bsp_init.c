#include  <includes.h>

//ʱ�ӳ�ʼ��
void SetClock(void)
{
	ErrorStatus HSEStartUpStatus;
	RCC_DeInit();			    
	RCC_HSEConfig(RCC_HSE_ON);	
	//�ȴ�HSE�ȶ�
	while(HSEStartUpStatus!=SUCCESS)
		{HSEStartUpStatus = RCC_WaitForHSEStartUp();}
	//����pll֮ǰ��pll�ر�
	RCC_PLLCmd(DISABLE);
	RCC_PLLConfig(RCC_PLLSource_HSE,8,336,2,7);    //8mhz
	RCC_PLLCmd(ENABLE);	
	//�ȴ�pll�ȶ�
	while((RCC->CR & RCC_CR_PLLRDY) == 0);
	//��pll����ϵͳʱ�ӣ����ҵȴ��ȶ�
	RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);
	//��������ʱ��
	RCC_HCLKConfig(RCC_SYSCLK_Div1);	           
	RCC_PCLK1Config(RCC_HCLK_Div4);			 	   
	RCC_PCLK2Config(RCC_HCLK_Div2);
}



void wwatch_dog_init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG,ENABLE);
	WWDG_DeInit();
	WWDG_SetPrescaler(WWDG_Prescaler_8);
	WWDG_SetWindowValue(0x7F);
	WWDG_Enable(0x41);
	
//		if((WWDG->CR & 0x7F) == 0x45)
//			{
//				WWDG_SetCounter(0x7f);
//			}
}

void iwatch_dog_init(void)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_128);  
	IWDG_SetReload(100);  
	IWDG_Enable();
	
	//IWDG_ReloadCounter(); //ι��
}

void read_dev_id(unsigned char* DevID_string)
{
	unsigned char DevID_tmp[4] = {'\0'};
	unsigned char i,IDtemp[12];
	memset(DevID_string,'\0',50);
	
	for(i=0;i<12;i++)
		{
			IDtemp[i] = *( (unsigned char*)(0x1fff7a10+i) );
		}
		
		
		
	for(i=0;i<12;i++)
		{
			hextostr(DevID_tmp,IDtemp[i]);
			strcat(DevID_string,DevID_tmp);
			memset(DevID_tmp,'\0',4);
		}
}

