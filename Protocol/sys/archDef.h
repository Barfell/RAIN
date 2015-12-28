#ifndef __ARCHDEF_H
#define __ARCHDEF_H

// 调用约定
#define	STDCALL
#define	CDECL
#define	FASTCALL

// 类型属性转义
#define	STATIC					static
#define	REGISTER				register
#define	EXTERN					extern
#define	CONST					const
#define	VOLATILE				volatile
#define	VAR						

// 私有定义
#if	defined(_DEBUG_PRIVATE_)
	#define	PRIVATE						
#else
	#define	PRIVATE				STATIC
#endif

// 公共定义
#define	PUBLIC	


//////////////////////////////////////////////////////////////////////////////////
// 定义ARM的段定位方法
//////////////////////////////////////////////////////////////////////////////////

// 所有段
#define	DECLSPEC_ARMSECTION(nCode, nRO, nRW, nZI)	\
	PRAGMA(arm section code=#nCode, rodata=#nRO, rwdata=#nRW, zidata=#nZI)

// 代码段
#define	DECLSPEC_ARMCODESECTION(nCode)				\
	PRAGMA(arm section code=#nCode)
	
// 只读段
#define	DECLSPEC_ARMROSECTION(nRO)					\
	PRAGMA(arm section rodata=#nRO)
	
// 读写段
#define	DECLSPEC_ARMRWSECTION(nRW)					\
	PRAGMA(arm section rwdata=#nRW)
	
// 清零段
#define	DECLSPEC_ARMZISECTION(nZI)					\
	PRAGMA(arm section zidata=#nZI)
	
// 默认段
#define	DECLSPEC_ARMDEFSECTION()					\
	PRAGMA(arm section rodata, code, rwdata, zidata)
	
//////////////////////////////////////////////////////////////////////////////////
// 定义指令编码方式
//////////////////////////////////////////////////////////////////////////////////

// ARM指令编码
#define	DECLSPEC_ARMCODE()		PRAGMA(arm)

// Thumb指令编码
#define	DECLSPEC_THUMBCODE()	PRAGMA(thumb)

//////////////////////////////////////////////////////////////////////////////////
// 定义独立元素的定位方法
//////////////////////////////////////////////////////////////////////////////////

// 段定位
#define	LocateSegment(nSeg)	__attribute__((section(#nSeg)))

// 地址定位
#define	LocateAddr(nAddr)	__attribute__((at(nAddr)))

// 强制使用
#define	ForceUsed			__attribute__((used))

// 强制不使用
#define	ForceUnused			__attribute__((unused))

// 使变量使用特定寄存器
#define	AsReg(nReg)			__asm(#nReg)

//////////////////////////////////////////////////////////////////////////////////
// 定义链接优化方式
//////////////////////////////////////////////////////////////////////////////////

// 强制内联
#define	FORCEINLINE			__forceinline STATIC

// 强制不内联
#define	FORCENOINLINE		__attribute__((noinline))

// 别名定义
#define	DECLSPEC_ALIAS(nSection, szOldName)			\
nSection __attribute__((alias(szOldName)))			

#endif
