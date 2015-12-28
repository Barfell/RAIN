#include  <includes.h>

void hextostr(unsigned char *str ,unsigned char hex)
{
	if( (hex / 16) > 9)
		{*str = (hex / 16)+'0'+ 7;}
	else
		{*str = (hex / 16)+'0';}

	if((hex%16) > 9)
		{*(str+1) = (hex%16)+'0' + 7;}
	else
		{*(str+1) = (hex%16)+'0';}
}


unsigned int SoftGenCrc32(unsigned int* puData, unsigned int uSize) 
{ 
	unsigned int crc = 0xFFFFFFFF;
	unsigned int i=0,j=0,temp; 
	for(i=0;i<uSize;i++)
	{ 
		temp = puData[i]; 
		for(j=0;j<32;j++)
		{ 
			if( (crc ^ temp) & 0x80000000 )
			{ 
				crc = 0x04C11DB7 ^ (crc<<1); 
			}
			else
			{ 
				crc <<=1; 
			} 
			temp<<=1; 
		} 
	}
	
	return crc; 
} 
unsigned int HardGenCrc32(unsigned int* puData, unsigned int uSize)
{
	CRC_ResetDR();
	return CRC_CalcBlockCRC((unsigned int *)puData, (unsigned int)uSize);
}



void usart_update_packet(unsigned char *p,unsigned int length)
{
	while(length != 0)
	{
		//while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
		USART_SendData(USART3, *(p++) );
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
		length--;
	}
}

