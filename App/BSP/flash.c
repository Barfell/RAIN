#include "stm32f4xx.h"
void MCUFlashUnlock(void)
{
	FLASH_Unlock(); 
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
}
unsigned char MCUFlashErase(unsigned char sector)
{
	if(FLASH_EraseSector(sector,VoltageRange_3) == FLASH_COMPLETE)
	{
		return (1);
	}
}
//固定在sector11 0x080e0000,需要从0x080e0000开始存数据
unsigned char MCUFlashWrite(int FlashAddress, int Data)
{
	if(FLASH_ProgramWord(FlashAddress, Data) == FLASH_COMPLETE)
		{
		  return (1);
		}
}


void flash_w_buffer(int FlashAddress, unsigned char *Data)
{
	while(*Data != '\0')
	{
		while(FLASH_ProgramByte(FlashAddress, *Data) != FLASH_COMPLETE);
		Data++;
		FlashAddress++;
	}
}
