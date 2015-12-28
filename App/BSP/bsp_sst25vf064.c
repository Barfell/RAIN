#include <includes.h>
//因为FLASH里面是没有时钟的，所以读取数据的时候需要spi口本身提供给FLASH
void SST25VF064Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
	//MISO ---->SST25VF064--SDO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	//MOSI ---->SST25VF064--SDI
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	//SCK ---->SST25VF064--SCK
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	//CS ---->SST25VF064--CS
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	//HOLD ---->SST25VF064--HOLD
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CRCPolynomial = 0x00;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_FirstBit =  SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_Init(SPI2,&SPI_InitStructure);
	SPI_Cmd(SPI2,ENABLE);
	
}

unsigned char SST25VF064WriteReadByte(unsigned char cmd)
{
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE) == RESET );
	SPI_I2S_SendData(SPI2,cmd);
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE) == RESET );
	SPI_I2S_SendData(SPI2,0xff);
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE) == RESET );
	return SPI_I2S_ReceiveData(SPI2);
	//while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE) == RESET );
	//SPI_I2S_SendData(SPI2,0xff);
}

void SST25VF064Unselect(void)
{
	SST25VF064_CS_HIGH;
}
void SST25VF064Select(void)
{
	SST25VF064_CS_LOW;
}

void SST25VF064Reset(void)
{
	OS_ERR		err;
	SST25VF064_HOLD_HIGH;
	OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_DLY,&err);
	SST25VF064_HOLD_LOW;
	OSTimeDlyHMSM(0,0,0,1,OS_OPT_TIME_DLY,&err);
	SST25VF064_HOLD_HIGH;
	OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_DLY,&err);
}




void SST25VF064SectorErase(unsigned int Addr)
{
	OS_ERR		err;
	unsigned int timeover=0; 
	SST25VF064Select();
	SST25VF064WriteReadByte(WREN);
	SST25VF064WriteReadByte(SECTOR_ERASE);
	SST25VF064WriteReadByte((unsigned char)(Addr>>16));
	SST25VF064WriteReadByte((unsigned char)(Addr>>8));
	SST25VF064WriteReadByte((unsigned char)Addr);
	while(SST25VF064BusySatue() == 0 )
		{
			timeover++;
			if(timeover == 200)
				{timeover = 0;break;}
			OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_DLY,&err);
		}
	SST25VF064WriteReadByte(WRDI);
	SST25VF064Unselect();
}
void SST25VF064BlockErase(unsigned int Addr)
{
	OS_ERR		err;
	unsigned int timeover=0; 
	SST25VF064Select();
	SST25VF064WriteReadByte(WREN);
	SST25VF064WriteReadByte(BLOCK_ERASE);
	SST25VF064WriteReadByte((unsigned char)(Addr>>16));
	SST25VF064WriteReadByte((unsigned char)(Addr>>8));
	SST25VF064WriteReadByte((unsigned char)Addr);
	while(SST25VF064BusySatue() == 0 )
		{
			timeover++;
			if(timeover == 200)
				{timeover = 0;break;}
			OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_DLY,&err);
		}
	SST25VF064WriteReadByte(WRDI);
	SST25VF064Unselect();
}
void SST25VF064BlockOPErase(unsigned int Addr)
{
	OS_ERR		err;
	unsigned int timeover=0; 
	SST25VF064Select();
	SST25VF064WriteReadByte(WREN);
	SST25VF064WriteReadByte(BLOCK_ERASE_OP);
	SST25VF064WriteReadByte((unsigned char)(Addr>>16));
	SST25VF064WriteReadByte((unsigned char)(Addr>>8));
	SST25VF064WriteReadByte((unsigned char)Addr);
	while(SST25VF064BusySatue() == 0 )
		{
			timeover++;
			if(timeover == 200)
				{timeover = 0;break;}
			OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_DLY,&err);
		}
	SST25VF064WriteReadByte(WRDI);
	SST25VF064Unselect();
}
void SST25VF064ChipErase(unsigned int Addr)
{
	OS_ERR		err;
	unsigned int timeover=0; 
	SST25VF064Select();
	SST25VF064WriteReadByte(WREN);
	SST25VF064WriteReadByte(CHIP_ERASE);
	SST25VF064WriteReadByte((unsigned char)(Addr>>16));
	SST25VF064WriteReadByte((unsigned char)(Addr>>8));
	SST25VF064WriteReadByte((unsigned char)Addr);
	while(SST25VF064BusySatue() == 0 )
		{
			timeover++;
			if(timeover == 200)
				{timeover = 0;break;}
			OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_DLY,&err);
		}
	SST25VF064WriteReadByte(WRDI);
	SST25VF064Unselect();
}



//1:unbusy
//0:busy
unsigned char SST25VF064BusySatue(void)
{
	SST25VF064WriteReadByte(RDSR);
	if( SST25VF064WriteReadByte(0xff) & 0x01 )
		{return 0;}
	else
		{return 1;}
}



//向某一个地址写入一个数据
void SST25VF064ProgramByte(unsigned int Addr,unsigned char dat)
{
	unsigned int timeover=0;
	SST25VF064Select();
	SST25VF064WriteReadByte(WREN);
	SST25VF064WriteReadByte(BYTE_PROGRAM);
	SST25VF064WriteReadByte((unsigned char)(Addr>>16));
	SST25VF064WriteReadByte((unsigned char)(Addr>>8));
	SST25VF064WriteReadByte((unsigned char)Addr);
	SST25VF064WriteReadByte(dat);
	while(SST25VF064BusySatue() == 0 )
		{
			timeover++;
			if(timeover == 200)
				{timeover = 0;break;}
			//delay
		}	
	
	SST25VF064WriteReadByte(WRDI);
	SST25VF064Unselect();
}
//从某一个地址开始，写入一定长度的连续数据
void SST25VF064ProgramLength(unsigned int Addr,unsigned char *dat,unsigned int length)
{
	unsigned int i;
	for(i = 0;i<length;i++)
		{
			SST25VF064ProgramByte(Addr+i,*(dat+i));
		}
}




//读取某一地址的一个字节
unsigned char SST25VF064FlashReadByte(unsigned int Addr)
{
		unsigned int dat;
		SST25VF064Select();
		SST25VF064WriteReadByte(READ);
		SST25VF064WriteReadByte((unsigned char)(Addr>>16));
		SST25VF064WriteReadByte((unsigned char)(Addr>>8));
		SST25VF064WriteReadByte((unsigned char)Addr);
		dat = SST25VF064WriteReadByte(0xff);
		SST25VF064Unselect();
		return dat;
}
//读取一定长度到buff
void SST25VF064FlashRead(unsigned int Addr,unsigned char *pbuff,unsigned int length)
{
		unsigned int i;
		SST25VF064Select();
		SST25VF064WriteReadByte(READ);
		SST25VF064WriteReadByte((unsigned char)(Addr>>16));
		SST25VF064WriteReadByte((unsigned char)(Addr>>8));
		SST25VF064WriteReadByte((unsigned char)Addr);
		for(i=0;i<length;i++)
			{
				*(pbuff+i) = SST25VF064WriteReadByte(0xff);
			}
		SST25VF064Unselect();
}

