#ifndef _BSP_RTC_H
#define _BSP_RTC_H

char* get_time(void);
char* get_time2(void);
void RTC_Config(void);
void set_time(unsigned char year,
			unsigned char mounth,
			unsigned char date,
			unsigned char week,
			unsigned char AmPm,
			unsigned char hour,
			unsigned char minutes,
			unsigned char second);

#endif


