#ifndef _TOOLS_H
#define _TOOLS_H

// 简单运算定义
#define MAKEBYTE(l, h)			((unsigned char)(((unsigned char)(l) & 0x0F) | ((((unsigned char)(h)) << 4) & 0xF0)))
#define MAKEWORD(l, h)			((unsigned short)((((unsigned char)(l))&0x00FF) | ((((unsigned short)((unsigned char)(h))) << 8)&0xFF00)))
#define LOWORD(l)				((unsigned short)((l) & 0xFFFF))
#define HIWORD(l)				((unsigned short)(((unsigned long)(l) >> 16) & 0xFFFF))
#define LOBYTE(w)				((unsigned char)((w) & 0xFF))
#define HIBYTE(w)				((unsigned char)(((unsigned short)(w) >> 8) & 0xFF))
#define HIHALFBYTE(b)			((unsigned char)(((b) >> 4) & 0x0F))
#define LOHALFBYTE(b)			((unsigned char)((b) & 0x0F))
#define max(a, b)				(((a) > (b)) ? (a) : (b))
#define min(a, b)				(((a) < (b)) ? (a) : (b))



enum
{
	NENCRYPTION_NCOMPRESSION = 0X00,
	ENCRYPTION_NCOMPRESSION,
	NENCRYPTION_COMPRESSION,
	ENCRYPTION_COMPRESSION,
	
};

void hextostr(unsigned char *str ,unsigned char hex);

unsigned int SoftGenCrc32(unsigned int* puData, unsigned int uSize);
unsigned int HardGenCrc32(unsigned int* puData, unsigned int uSize);


#endif

