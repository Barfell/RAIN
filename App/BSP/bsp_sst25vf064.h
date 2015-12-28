#ifndef __BSP_SST25VF064_H
#define __BSP_SST25VF064_H

#define SST25VF064_CS_LOW 			GPIO_ResetBits(GPIOB,GPIO_Pin_12)
#define SST25VF064_CS_HIGH		 	GPIO_SetBits(GPIOB,GPIO_Pin_12)
#define SST25VF064_HOLD_LOW 			GPIO_ResetBits(GPIOB,GPIO_Pin_11)
#define SST25VF064_HOLD_HIGH		 	GPIO_SetBits(GPIOB,GPIO_Pin_11)



//read
#define READ          		0x03		// normal read 
#define READ_DOUBLE_IO		0xBB		//read in double IO
#define READ_DOUBLE_OUT 	0x3B		//read in double OUT
#define READ_HS       		0x0B		// read with 8 extra clocks between address and data. for high speed bus 
//Erase
#define SECTOR_ERASE  		0x20		// Sector Erase 4k
#define BLOCK_ERASE   		0x52		// Block Erase 32k
#define BLOCK_ERASE_OP 		0xD8		// Block Erase 64k
#define CHIP_ERASE    		0x60		// Chip Erase
#define CHIP_ERASE_OP 		0xC7		// Chip Erase optional cmd
//program
#define BYTE_PROGRAM  		0x02		// Byte Program
#define AAI_PROGRAM   		0xAD		// auto increment mode Word program


#define RDSR          		0x05		// Read Status Register
#define EWSR          		0x50		// Enable Write Status Register
#define WRSR          		0x01		// Write Status register
#define WREN          		0x06		// Write Enable
#define WRDI          		0x04		// Write Disable
#define RDID         		 	0x90		// Read ID
#define RDID_OP       		0xAB		// Read ID, optional cmd
#define JEDEC_ID      		0x9F		// JEDEC ID read cmd

#define EHLD							0xAA		//SWITCH  RST#/HOLD# to HOLD func 
#define SID								0x88		//read safe ID
#define SID_PROGRAM				0xA5		//program in user's safe ID area
#define SID_LOCK					0x85		//LOCK the ID area


//初始化ADC的引脚
void SST25VF064Init(void);
unsigned char SST25VF064WriteReadByte(unsigned char cmd);
void SST25VF064Unselect(void);
void SST25VF064Select(void);
void SST25VF064Reset(void);
void SST25VF064SectorErase(unsigned int Addr);
void SST25VF064BlockErase(unsigned int Addr);
void SST25VF064BlockOPErase(unsigned int Addr);
void SST25VF064ChipErase(unsigned int Addr);
//1:unbusy
//0:busy
unsigned char SST25VF064BusySatue(void);
//向某一个地址写入一个数据
void SST25VF064ProgramByte(unsigned int Addr,unsigned char dat);
//从某一个地址开始，写入一定长度的连续数据
void SST25VF064ProgramLength(unsigned int Addr,unsigned char *dat,unsigned int length);
//读取某一地址的一个字节
unsigned char SST25VF064FlashReadByte(unsigned int Addr);
//读取一定长度到buff
void SST25VF064FlashRead(unsigned int Addr,unsigned char *pbuff,unsigned int length);


#endif



