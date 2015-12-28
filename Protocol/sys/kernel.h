#ifndef __BSP_H
#define __BSP_H	

#include "tool.h"
#include "stm32f4xx.h"
#include "Protocol.h"
#include "usart_serial.h"

enum
{
    ERR_MALLOC_FAIL,
};

typedef struct _tagSysInfo
{
	BOOL RTC_FLAG;
	
}CSysInfo, *PSysInfo;
																	    
extern CSysInfo cSysInfo;

VOID ReadLockId(U32 *pData);

#endif











