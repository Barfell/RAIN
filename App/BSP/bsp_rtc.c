#include <includes.h>

char* get_time(void)
{  
	char time[50]={'\0'};
	RTC_TimeTypeDef   	RTC_GetTimeStructure;
	RTC_DateTypeDef 	RTC_GetDateStructure;
	if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x32F2)//?????,???????
		{RTC_Config();}
	else
		{//????,?????

			RTC_GetTime(RTC_Format_BIN, &RTC_GetTimeStructure);
			RTC_GetDate(RTC_Format_BIN, &RTC_GetDateStructure);
			
			sprintf(time,"%d-%d-%d-%d:%d:%d|",\
			RTC_GetDateStructure.RTC_Year,RTC_GetDateStructure.RTC_Month,\
			RTC_GetDateStructure.RTC_Date,RTC_GetTimeStructure.RTC_Hours,\
			RTC_GetTimeStructure.RTC_Minutes,RTC_GetTimeStructure.RTC_Seconds);
		}
		return time;
}

char* get_time2(void)
{  
	char time[50]={'\0'};
	RTC_TimeTypeDef   	RTC_GetTimeStructure;
	RTC_DateTypeDef 	RTC_GetDateStructure;
	if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x32F2)//?????,???????
		{RTC_Config();}
	else
		{//????,?????

			RTC_GetTime(RTC_Format_BIN, &RTC_GetTimeStructure);
			RTC_GetDate(RTC_Format_BIN, &RTC_GetDateStructure);
			
			sprintf(time,"%d-%d-%d-%d:%d:%d",\
			RTC_GetDateStructure.RTC_Year,RTC_GetDateStructure.RTC_Month,\
			RTC_GetDateStructure.RTC_Date,RTC_GetTimeStructure.RTC_Hours,\
			RTC_GetTimeStructure.RTC_Minutes,RTC_GetTimeStructure.RTC_Seconds);
		}
		return time;
}




void RTC_Config(void)
{
	
	RTC_InitTypeDef RTC_InitStructure;
	RTC_TimeTypeDef RTC_TimeStructure;
	RTC_DateTypeDef RTC_DateStructure;
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  /* Enable the PWR clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  /* Allow access to RTC */
  PWR_BackupAccessCmd(ENABLE);
  /* Enable the LSE OSC */
  RCC_LSEConfig(RCC_LSE_ON);
  /* Wait till LSE is ready */  
  while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {
  }
  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
  /* Enable the RTC Clock */
  RCC_RTCCLKCmd(ENABLE);
  /* Wait for RTC APB registers synchronisation */
  RTC_WaitForSynchro();
  
  /* Configure the RTC data register and RTC prescaler */
  RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
  RTC_InitStructure.RTC_SynchPrediv = 0xFF;
  RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
  RTC_Init(&RTC_InitStructure);
  
 
  RTC_DateStructure.RTC_Year = 0x15;
  RTC_DateStructure.RTC_Month = RTC_Month_December;
  RTC_DateStructure.RTC_Date = 0x27;
  RTC_DateStructure.RTC_WeekDay = RTC_Weekday_Sunday;
  RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);
  RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
  RTC_TimeStructure.RTC_Hours   = 0x03;
  RTC_TimeStructure.RTC_Minutes = 0x02;
  RTC_TimeStructure.RTC_Seconds = 0x05; 
  RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);   
  /* Indicator for the RTC configuration */
  RTC_WriteBackupRegister(RTC_BKP_DR0, 0x32F2);
}


void set_time(unsigned char year,
			unsigned char mounth,
			unsigned char date,
			unsigned char week,
			unsigned char AmPm,
			unsigned char hour,
			unsigned char minutes,
			unsigned char second)
{
	RTC_InitTypeDef RTC_InitStructure;
	RTC_TimeTypeDef RTC_TimeStructure;
	RTC_DateTypeDef RTC_DateStructure;
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  /* Enable the PWR clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  /* Allow access to RTC */
  PWR_BackupAccessCmd(ENABLE);
  /* Enable the LSE OSC */
  RCC_LSEConfig(RCC_LSE_ON);
  /* Wait till LSE is ready */  
  while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {
  }
  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
  /* Enable the RTC Clock */
  RCC_RTCCLKCmd(ENABLE);
  /* Wait for RTC APB registers synchronisation */
  RTC_WaitForSynchro();
  
  /* Configure the RTC data register and RTC prescaler */
  RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
  RTC_InitStructure.RTC_SynchPrediv = 0xFF;
  RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
  RTC_Init(&RTC_InitStructure);
  
 //…Ë÷√ ±º‰ 
  RTC_DateStructure.RTC_Year = year;
  RTC_DateStructure.RTC_Month = mounth;
  RTC_DateStructure.RTC_Date = date;
  RTC_DateStructure.RTC_WeekDay = week;
  RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);
  
  RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
  RTC_TimeStructure.RTC_Hours   = hour;
  RTC_TimeStructure.RTC_Minutes = minutes;
  RTC_TimeStructure.RTC_Seconds = second; 
  RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);   
  
  /* Indicator for the RTC configuration */
  RTC_WriteBackupRegister(RTC_BKP_DR0, 0x32F2);
}

