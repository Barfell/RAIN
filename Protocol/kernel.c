#include "kernel.h" 

CSysInfo cSysInfo = {0};


VOID EnterException(U32 u32ErrCode)
{
	U8 u8Count = 0;
    while(u8Count < 100)
    {
        printf("error code = %x \r\n", u32ErrCode);
		u8Count++;
    }  
}

VOID ReadLockId(U32 *pData)
{
	//»ñÈ¡CPUÎ¨Ò»ID
	pData[0]=*(vu32*)(0x1FFFF7E8);
	pData[1]=*(vu32*)(0x1FFFF7EC);
	pData[2]=*(vu32*)(0x1FFFF7F0);

}
