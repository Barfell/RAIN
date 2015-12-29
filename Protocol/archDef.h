#ifndef __ARCHDEF_H
#define __ARCHDEF_H

// ����Լ��
#define	STDCALL
#define	CDECL
#define	FASTCALL

// ��������ת��
#define	STATIC					static
#define	REGISTER				register
#define	EXTERN					extern
#define	CONST					const
#define	VOLATILE				volatile
#define	VAR						

// ˽�ж���
#if	defined(_DEBUG_PRIVATE_)
	#define	PRIVATE						
#else
	#define	PRIVATE				STATIC
#endif

// ��������
#define	PUBLIC	


//////////////////////////////////////////////////////////////////////////////////
// ����ARM�Ķζ�λ����
//////////////////////////////////////////////////////////////////////////////////

// ���ж�
#define	DECLSPEC_ARMSECTION(nCode, nRO, nRW, nZI)	\
	PRAGMA(arm section code=#nCode, rodata=#nRO, rwdata=#nRW, zidata=#nZI)

// �����
#define	DECLSPEC_ARMCODESECTION(nCode)				\
	PRAGMA(arm section code=#nCode)
	
// ֻ����
#define	DECLSPEC_ARMROSECTION(nRO)					\
	PRAGMA(arm section rodata=#nRO)
	
// ��д��
#define	DECLSPEC_ARMRWSECTION(nRW)					\
	PRAGMA(arm section rwdata=#nRW)
	
// �����
#define	DECLSPEC_ARMZISECTION(nZI)					\
	PRAGMA(arm section zidata=#nZI)
	
// Ĭ�϶�
#define	DECLSPEC_ARMDEFSECTION()					\
	PRAGMA(arm section rodata, code, rwdata, zidata)
	
//////////////////////////////////////////////////////////////////////////////////
// ����ָ����뷽ʽ
//////////////////////////////////////////////////////////////////////////////////

// ARMָ�����
#define	DECLSPEC_ARMCODE()		PRAGMA(arm)

// Thumbָ�����
#define	DECLSPEC_THUMBCODE()	PRAGMA(thumb)

//////////////////////////////////////////////////////////////////////////////////
// �������Ԫ�صĶ�λ����
//////////////////////////////////////////////////////////////////////////////////

// �ζ�λ
#define	LocateSegment(nSeg)	__attribute__((section(#nSeg)))

// ��ַ��λ
#define	LocateAddr(nAddr)	__attribute__((at(nAddr)))

// ǿ��ʹ��
#define	ForceUsed			__attribute__((used))

// ǿ�Ʋ�ʹ��
#define	ForceUnused			__attribute__((unused))

// ʹ����ʹ���ض��Ĵ���
#define	AsReg(nReg)			__asm(#nReg)

//////////////////////////////////////////////////////////////////////////////////
// ���������Ż���ʽ
//////////////////////////////////////////////////////////////////////////////////

// ǿ������
#define	FORCEINLINE			__forceinline STATIC

// ǿ�Ʋ�����
#define	FORCENOINLINE		__attribute__((noinline))

// ��������
#define	DECLSPEC_ALIAS(nSection, szOldName)			\
nSection __attribute__((alias(szOldName)))			

#endif
