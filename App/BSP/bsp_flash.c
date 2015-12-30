#include "includes.h"

//写FLASH之前需要先执行这个函数
void MCUFlashUnlock(void)
{
	FLASH_Unlock(); 
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
}
//#define FLASH_Sector_0     ((uint16_t)0x0000) /*!< Sector Number 0 */
//#define FLASH_Sector_1     ((uint16_t)0x0008) /*!< Sector Number 1 */
//#define FLASH_Sector_2     ((uint16_t)0x0010) /*!< Sector Number 2 */
//#define FLASH_Sector_3     ((uint16_t)0x0018) /*!< Sector Number 3 */
//#define FLASH_Sector_4     ((uint16_t)0x0020) /*!< Sector Number 4 */
//#define FLASH_Sector_5     ((uint16_t)0x0028) /*!< Sector Number 5 */
//#define FLASH_Sector_6     ((uint16_t)0x0030) /*!< Sector Number 6 */
//#define FLASH_Sector_7     ((uint16_t)0x0038) /*!< Sector Number 7 */
//#define FLASH_Sector_8     ((uint16_t)0x0040) /*!< Sector Number 8 */
//#define FLASH_Sector_9     ((uint16_t)0x0048) /*!< Sector Number 9 */
//#define FLASH_Sector_10    ((uint16_t)0x0050) /*!< Sector Number 10 */
//#define FLASH_Sector_11    ((uint16_t)0x0058) /*!< Sector Number 11 */
/*
*FLASH_Sector_0     0x0800 0000 ~ 0x0800 3fff   16KB
*FLASH_Sector_1		0x0800 4000 ~ 0x0800 7fff   16KB
*FLASH_Sector_2     0x0800 8000 ~ 0x0800 bfff   16KB
*FLASH_Sector_3		0x0800 c000 ~ 0x0800 ffff   16KB
*FLASH_Sector_4		0x0801 0000 ~ 0x0801 ffff   64KB
*FLASH_Sector_5		0x0802 0000 ~ 0x0803 ffff   128KB
*FLASH_Sector_6		0x0804 0000 ~ 0x0805 ffff   128KB
*FLASH_Sector_7		0x0806 0000 ~ 0x0807 ffff   128KB
*FLASH_Sector_8		0x0808 0000 ~ 0x0809 ffff   128KB
*FLASH_Sector_9		0x080a 0000 ~ 0x080b ffff   128KB
*FLASH_Sector_10	0x080c 0000 ~ 0x080d ffff   128KB
*FLASH_Sector_11	0x080e 0000 ~ 0x080f ffff   128KB
*f407---1MB
*/
//擦除sector,写FLASH之前是需要先擦除的
unsigned char MCUFlashErase(unsigned char sector)
{
	unsigned char statu;
	if(FLASH_EraseSector(sector,VoltageRange_3) == FLASH_COMPLETE)
	{
		statu = 1;
	}
	else
	{statu = 0;}
	return statu;
}


//固定在sector11 0x080e0000,需要从0x080e0000开始存数据
unsigned char MCUFlashWrite(int FlashAddress, int Data)
{
	unsigned char statu;
	if(FLASH_ProgramWord(FlashAddress, Data) == FLASH_COMPLETE)
	{
		statu = 1;
	}
	else
	{statu = 0;}
	return statu;
}

//从某地址连续写入一串字符串
void flash_w_buffer(int FlashAddress, char *Data)
{
	while(*Data != '\0')
	{
		while(FLASH_ProgramByte(FlashAddress, *Data) != FLASH_COMPLETE);
		FlashAddress++;
		Data++;
	}
}


char* flash_r_buffer(int FlashAddress, int length)
{
	char Data[4096]={'\0'};
	int i=0;
	for(i=0; i<length; i++ )
		{
			Data[i] = (  *( (char *)(FlashAddress+i) )  == 0xff ) ? 0x00 : *( (char *)(FlashAddress+i) );//如果没有写入数据，默认状态是0xff，此时转换为0x00(null)
			//Data[i] =   *( (char *)(FlashAddress+i) );//直接读取
		}
	return Data;
}


//事例
//	if( *( (char *)0x080e0000 ) == 0xff )
//	{
//		MCUFlashUnlock();
//		while(MCUFlashErase(FLASH_Sector_11) != 1);
//		printf("FLASH_Sector_11 Erased\r\n");
//		flash_w_buffer(0x080e0000,"1234567890");
//		FLASH_Lock();
//		printf("flash_w_buffer done.\r\n");
//	}
