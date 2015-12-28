 /******************************************************************************
 * File：			type32.h
 * Brief：    		32位系统下类型定义
 * Author:			tao.lan
 * Date:			2011-8-9 created
 * Description:		通用数据类型定义，根据lzw Type32bit.h编写
 ******************************************************************************/
 
#ifndef __TYPE32_H
#define	__TYPE32_H

//////////////////////////////////////////////////////////////////////////////////
// 定义32位环境下的通用类型
//////////////////////////////////////////////////////////////////////////////////

typedef	unsigned char			U8;
typedef U8*						P_U8;
typedef signed char				S8;
typedef S8*						P_S8;
typedef unsigned short			U16;
typedef U16*					P_U16;
typedef signed short			S16;
typedef S16*					P_S16;
typedef unsigned int			U32;
typedef U32*					P_U32;
typedef signed int				S32;
typedef S32*					P_S32;
typedef void*					P_VOID;

typedef	unsigned long			time_t;
typedef	unsigned long			clock_t;
typedef int                     except_t;

typedef unsigned char			BYTE;
typedef unsigned short			WORD;
typedef unsigned long			DWORD;
typedef unsigned long long		QWORD;
typedef BYTE*					PBYTE;
typedef BYTE*					LPBYTE;
typedef const BYTE*				PCBYTE;
typedef const BYTE*				LPCBYTE;
typedef WORD*					PWORD;
typedef WORD*					LPWORD;
typedef const WORD*				PCWORD;
typedef const WORD*				LPCWORD;
typedef DWORD*					PDWORD;
typedef DWORD*					LPDWORD;
typedef const DWORD*			PCDWORD;
typedef const DWORD*			LPCDWORD;
typedef QWORD*					PQWORD;
typedef QWORD*					LPQWORD;
typedef const QWORD*			PCQWORD;
typedef const QWORD*			LPCQWORD;

typedef void*					HANDLE;
typedef	void*					LPVOID;
typedef	void*					PVOID;
typedef	const void*				LPCVOID;
//typedef void					VOID;
#define VOID                    void
#define EXTERN                  extern
#define STATIC                  static 
typedef	unsigned char			UINT8;
typedef	unsigned short int		UINT16;
typedef	unsigned long int		UINT32;
typedef	unsigned long long int	UINT64;
typedef	signed char  			INT8;
typedef	signed short int		INT16;
typedef	signed long int	   		INT32;
typedef	signed long long int 	INT64;

typedef signed int				INT;
typedef signed int *			PINT;
typedef signed int *			LPINT;
typedef unsigned int			UINT;
typedef unsigned int *			PUINT;
typedef unsigned int *			LPUINT;
typedef signed long				LONG;
typedef signed long *			PLONG;
typedef signed long *			LPLONG;
typedef unsigned long			ULONG;
typedef unsigned long *			PULONG;
typedef unsigned long *			LPULONG;

typedef volatile unsigned int   VUINT;	
typedef volatile unsigned int * PVUINT;
typedef UINT					STATUS;	
typedef STATUS					LRESULT;
typedef UINT					PARAM;
typedef UINT					WPARAM;
typedef UINT					LPARAM;
typedef	UINT					BOOL;

typedef char					CHAR;
typedef unsigned char           UCHAR;
typedef CHAR *					LPSTR;
typedef const CHAR *			LPCSTR;
typedef CHAR *                  PCHAR;

typedef void (*TPFunction)(void);

					
#if	defined(TRUE)
#undef	TRUE
#endif
#define	TRUE					(1 == 1)

#if	defined(FALSE)
#undef	FALSE
#endif
#define	FALSE					(1 == 0)

#if	defined(NULL)
#undef	NULL
#endif
#define	NULL					(0)

#if	defined(SIZEOF)
#undef	SIZEOF
#endif
#define	SIZEOF(v)				((UINT)sizeof(v))

#if defined(SIZEOF_STRUCTM)
#undef	SIZEOF_STRUCTM
#endif
#define	SIZEOF_STRUCTM(stc, m)	(sizeof(((stc*)(0x0))->m))

#if	defined(_countof)
#undef	_countof
#endif
#define _countof(array) 		(sizeof(array) / sizeof((array)[0]))

#if	defined(COUNTOF)
#undef	COUNTOF
#endif
#define	COUNTOF(array)			(_countof(array))

#if	defined(OFFSETOF)
#undef	OFFSETOF
#endif
#define	OFFSETOF(nType, nField)	((UINT)(&(((nType *)0)->nField)))

// 定义原子数据大小
#define	ATOMSIZE				(4)

// BIT位定义
#define	BIT0					(0x00000001)
#define	BIT1					(0x00000002)
#define	BIT2					(0x00000004)
#define	BIT3					(0x00000008)
#define	BIT4					(0x00000010)
#define	BIT5					(0x00000020)
#define	BIT6					(0x00000040)
#define	BIT7					(0x00000080)
#define	BIT8					(0x00000100)
#define	BIT9					(0x00000200)
#define	BIT10					(0x00000400)
#define	BIT11					(0x00000800)
#define	BIT12					(0x00001000)
#define	BIT13					(0x00002000)
#define	BIT14					(0x00004000)
#define	BIT15					(0x00008000)
#define	BIT16					(0x00010000)
#define	BIT17					(0x00020000)
#define	BIT18					(0x00040000)
#define	BIT19					(0x00080000)
#define	BIT20					(0x00100000)
#define	BIT21					(0x00200000)
#define	BIT22					(0x00400000)
#define	BIT23					(0x00800000)
#define	BIT24					(0x01000000)
#define	BIT25					(0x02000000)
#define	BIT26					(0x04000000)
#define	BIT27					(0x08000000)
#define	BIT28					(0x10000000)
#define	BIT29					(0x20000000)
#define	BIT30					(0x40000000)
#define	BIT31					(0x80000000)

// 简单运算定义
#define MAKEBYTE(l, h)			((unsigned char)(((unsigned char)(l) & 0x0F) | ((((unsigned char)(h)) << 4) & 0xF0)))
#define MAKEWORD(l, h)			((unsigned short)((((unsigned char)(l))&0x00FF) | ((((unsigned short)((unsigned char)(h))) << 8)&0xFF00)))
#define MAKELONG(l, h)			((U32)(((U16)(l)) | ((U32)((U16)(h))) << 16))
#define LOWORD(l)				((unsigned short)((l) & 0xFFFF))
#define HIWORD(l)				((unsigned short)(((unsigned long)(l) >> 16) & 0xFFFF))
#define LOBYTE(w)				((unsigned char)((w) & 0xFF))
#define HIBYTE(w)				((unsigned char)(((unsigned short)(w) >> 8) & 0xFF))
#define HIHALFBYTE(b)			((unsigned char)(((b) >> 4) & 0x0F))
#define LOHALFBYTE(b)			((unsigned char)((b) & 0x0F))
#define max(a, b)				(((a) > (b)) ? (a) : (b))
#define min(a, b)				(((a) < (b)) ? (a) : (b))

//#define	MAKEPARAM(l, h)			(MAKELONG((l), (h)))
#define MAKEPARAM			    MAKEWORD
#define MAKELPARAM			    MAKELONG

#define	LOPARAM(l)				LOWORD(l)
#define	HIPARAM(l)				HIWORD(l)

// 对齐转换
#define	ALIGN32BIT(n)			(((n) + 3) & (~3))
#define	ALIGN16BIT(n)			(((n) + 1) & (~1))
#define	ALIGNATOM(n)			ALIGN32BIT(n)
#define	ALIGNUINT(n)			ALIGN32BIT(n)

// 定义最高位
#define	MARKHIGHBIT(nType)		((nType)(~(((nType)-1) >> 1)))

// 定义类型的最大值
#define	MAXUINT					((UINT)-1)
#define	MAXINT					((INT)0x7FFFFFFF)
#define	MAXULONG				((ULONG)-1)
#define	MAXLONG					((LONG)0x7FFFFFFF)
#define	MAXWORD					((WORD)-1)
#define	MAXSHORT				((short)0x00007FFF)
#define	MAXBYTE					((BYTE)-1)
#define	MAXCHAR					((char)0x0000007F)

// 定义位数等级关系
#define	BITSOFATOM				(32)
#define	BITSOFHALF				(16)
#define	BITSOFMIN				(8)


#define __INLINE				__inline
/****************************为与C7S兼容定义以下内容**************************/
#define OFFSET(val, res)        (val = (U16)(((UINT)&res) & 0xFFFF))
#define SEG(val, res)           (val = (U16)(((UINT)&res)>>16UL))

#endif //__TYPE32_H
