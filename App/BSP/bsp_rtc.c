#include <includes.h>





//获取时间
char* get_time(void)
{  
	char time[30]={'\0'};
	RTC_TimeTypeDef   	RTC_GetTimeStructure;
	RTC_DateTypeDef 	RTC_GetDateStructure;
	if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x32F1)//假如修改了，则再次进行配置
		{
			RTC_Config(inner);
			set_time(0x15,0x12,0x28,0x14,0x50,0x01,0x01,RTC_H12_PM);
			//return "fail"
		}
	else
		{//没有修改，则直接继续
			RTC_GetTime(RTC_Format_BIN, &RTC_GetTimeStructure);
			RTC_GetDate(RTC_Format_BIN, &RTC_GetDateStructure);
			//2015-11-10-10:18:33
			sprintf(time,"20%d-%d-%d-%d:%d:%d|",\
			(RTC_GetDateStructure.RTC_Year), (RTC_GetDateStructure.RTC_Month),\
			(RTC_GetDateStructure.RTC_Date), (RTC_GetTimeStructure.RTC_Hours),\
			(RTC_GetTimeStructure.RTC_Minutes), (RTC_GetTimeStructure.RTC_Seconds) );
		}
	return time;
}
char* get_time2(void)
{  
	char time[80]={'\0'};
	RTC_TimeTypeDef   	RTC_GetTimeStructure;
	RTC_DateTypeDef 	RTC_GetDateStructure;
	if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x32F1)//假如修改了，则再次进行配置
		{
			RTC_Config(inner);
			set_time(0x15,0x12,0x28,0x14,0x50,0x01,0x01,RTC_H12_PM);
			//return "fail"
		}
	else
		{//没有修改，则直接继续
			RTC_GetTime(RTC_Format_BIN, &RTC_GetTimeStructure);
			RTC_GetDate(RTC_Format_BIN, &RTC_GetDateStructure);
			//2015-11-10-10:18:33
			sprintf(time,"20%d-%d-%d-%d:%d:%d",\
			RTC_GetDateStructure.RTC_Year,RTC_GetDateStructure.RTC_Month,\
			RTC_GetDateStructure.RTC_Date,RTC_GetTimeStructure.RTC_Hours,\
			RTC_GetTimeStructure.RTC_Minutes,RTC_GetTimeStructure.RTC_Seconds);
		}
	return time;
}







//RTC设置成内部或者外部时钟
void RTC_Config(rtc_clk rtcclk)
{
  RTC_InitTypeDef RTC_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  PWR_BackupAccessCmd(ENABLE);

	if(rtcclk == extrnal)
	{
		/* Enable the LSE OSC */
		RCC_LSEConfig(RCC_LSE_ON);
		/* Wait till LSE is ready */  
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
		/* Select the RTC Clock Source */
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
		/* Enable the RTC Clock */
		RCC_RTCCLKCmd(ENABLE);
		/* Wait for RTC APB registers synchronisation */
		RTC_WaitForSynchro();
	}
	if(rtcclk == inner)
	{
		RCC_LSICmd(ENABLE); //开启内部低速
		while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);

		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
		RCC_RTCCLKCmd(ENABLE);
		RTC_WaitForSynchro();
	}
	
	RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
	RTC_InitStructure.RTC_SynchPrediv = 0xFF;
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
	RTC_Init(&RTC_InitStructure);
	set_time(0x15,0x12,0x28,0x14,0x50,0x01,0x01,RTC_H12_PM);
}



//设置时间
// RTC_H12_AM                     ((uint8_t)0x00)
// RTC_H12_PM                     ((uint8_t)0x40)
void set_time(	unsigned char year,
				unsigned char mounth,
				unsigned char day,
				unsigned char hour,
				unsigned char min,
				unsigned char sec,
				unsigned char week,
				unsigned char ampm)
{
	RTC_TimeTypeDef RTC_TimeStructure;
	RTC_DateTypeDef RTC_DateStructure;
	
	RTC_DateStructure.RTC_Year = year;
	RTC_DateStructure.RTC_Month = mounth;
	RTC_DateStructure.RTC_Date = day;
	RTC_DateStructure.RTC_WeekDay = week;
	//设置时间
	RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure);
	
	
	RTC_TimeStructure.RTC_H12     = ampm;
	RTC_TimeStructure.RTC_Hours   = hour;
	RTC_TimeStructure.RTC_Minutes = min;
	RTC_TimeStructure.RTC_Seconds = sec; 
	RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure);   

	RTC_WriteBackupRegister(RTC_BKP_DR0, 0x32F1);
}








//设置闹钟,多长时间后唤醒 1s-23h
void set_alarm(	unsigned char hour,
				unsigned char min,
				unsigned char sec)
{
	RTC_DateTypeDef 	RTC_GetDateStructure;
	RTC_TimeTypeDef   	RTC_GetTimeStructure;
	RTC_AlarmTypeDef    RTC_AlarmStructure;
	RTC_TimeTypeDef     RTC_TimeStructure;
	NVIC_InitTypeDef    NVIC_InitStructure;
	EXTI_InitTypeDef    EXTI_InitStructure;
	
	RTC_AlarmCmd(RTC_Alarm_A,DISABLE);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line17;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
	RTC_ClearITPendingBit(RTC_IT_ALRA);
    EXTI_ClearITPendingBit(EXTI_Line17);
	
	RTC_GetTime(RTC_Format_BIN, &RTC_GetTimeStructure);
	RTC_GetDate(RTC_Format_BIN, &RTC_GetDateStructure);
	//闹钟结构体
	RTC_AlarmStructure.RTC_AlarmDateWeekDay = RTC_GetDateStructure.RTC_WeekDay;//闹钟日期
	RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_WeekDay;//选择日期闹钟还是星期闹钟
	RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_None;//屏蔽某时间标示的比较，这里不屏蔽，全部比较
	//时间结构体
	RTC_TimeStructure.RTC_H12 = RTC_GetTimeStructure.RTC_H12;
	/////////////////////////////h/////////////////////////////////////////
	if(RTC_GetTimeStructure.RTC_Hours+hour >= 24)//下一个时间超过24h
	{	RTC_TimeStructure.RTC_Hours = RTC_GetTimeStructure.RTC_Hours+hour-24;
		RTC_AlarmStructure.RTC_AlarmDateWeekDay = RTC_AlarmStructure.RTC_AlarmDateWeekDay+1;//天数+1
		if(RTC_AlarmStructure.RTC_AlarmDateWeekDay > 7)
			{
				RTC_AlarmStructure.RTC_AlarmDateWeekDay = RTC_AlarmStructure.RTC_AlarmDateWeekDay-7;
			}
	}
	else
	{RTC_TimeStructure.RTC_Hours = RTC_GetTimeStructure.RTC_Hours+hour;}
	/////////////////////////////m/////////////////////////////////////////
	if(RTC_GetTimeStructure.RTC_Minutes+min >= 60 )//下一个时间多出一个小时
	{	
		RTC_TimeStructure.RTC_Minutes = RTC_GetTimeStructure.RTC_Minutes+min-60;
		RTC_TimeStructure.RTC_Hours = RTC_GetTimeStructure.RTC_Hours+hour+1;//h+1
		if(RTC_TimeStructure.RTC_Hours >= 24)//超过24h
			{
				RTC_TimeStructure.RTC_Hours = RTC_GetTimeStructure.RTC_Hours-24;
				RTC_AlarmStructure.RTC_AlarmDateWeekDay = RTC_AlarmStructure.RTC_AlarmDateWeekDay+1;
				if(RTC_AlarmStructure.RTC_AlarmDateWeekDay > 7)
					{
						RTC_AlarmStructure.RTC_AlarmDateWeekDay = RTC_AlarmStructure.RTC_AlarmDateWeekDay-7;
					}
			}
	}
	else
	{RTC_TimeStructure.RTC_Minutes = RTC_GetTimeStructure.RTC_Minutes+min;}
	/////////////////////////////s/////////////////////////////////////////
	if(RTC_GetTimeStructure.RTC_Seconds+sec >= 60)
	{
		RTC_TimeStructure.RTC_Seconds = RTC_GetTimeStructure.RTC_Seconds+sec-60;
		RTC_TimeStructure.RTC_Minutes = RTC_GetTimeStructure.RTC_Minutes+min+1;
		if(RTC_TimeStructure.RTC_Minutes >= 60)
			{
				RTC_TimeStructure.RTC_Minutes = RTC_TimeStructure.RTC_Minutes-60;
				RTC_TimeStructure.RTC_Hours = RTC_GetTimeStructure.RTC_Hours+hour+1;
				if(RTC_TimeStructure.RTC_Hours >= 24)
				{
					RTC_TimeStructure.RTC_Hours = RTC_TimeStructure.RTC_Hours -24;
					RTC_AlarmStructure.RTC_AlarmDateWeekDay = RTC_AlarmStructure.RTC_AlarmDateWeekDay+1;
					if(RTC_AlarmStructure.RTC_AlarmDateWeekDay > 7)
						{
							RTC_AlarmStructure.RTC_AlarmDateWeekDay = RTC_AlarmStructure.RTC_AlarmDateWeekDay-7;
						}
				}
			}
	}
	else
		{RTC_TimeStructure.RTC_Seconds = RTC_GetTimeStructure.RTC_Seconds+sec;}
	RTC_AlarmStructure.RTC_AlarmTime = RTC_TimeStructure;//time
	
	RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);
	RTC_ITConfig(RTC_IT_ALRA, ENABLE);
	RTC_AlarmCmd(RTC_Alarm_A, ENABLE);	
}








//闹钟中断处理函数
void RTC_Alarm_IRQHandler(void)
{
  
  if(RTC_GetITStatus(RTC_IT_ALRA) != RESET)
  {
	set_alarm(0,0,1);
    RTC_ClearITPendingBit(RTC_IT_ALRA);
    EXTI_ClearITPendingBit(EXTI_Line17);
  } 
}


