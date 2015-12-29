#ifndef __BSP_H
#define __BSP_H	

#include "includes.h"

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











