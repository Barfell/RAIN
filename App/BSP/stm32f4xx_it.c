/*
*********************************************************************************************************
*
*			我们只需要添加需要的中断函数即可。一般中断函数名是固定的，除非您修改了启动文件中的函数名
*				\Libraries\CMSIS\Device\ST\STM32F4xx\Source\Templates\arm\startup_stm32f4xx.s
*
*			启动文件是汇编语言文件，定了每个中断的服务函数，这些函数使用了WEAK 关键字，表示弱定义，因此如
*			果我们在c文件中重定义了该服务函数（必须和它同名），那么启动文件的中断函数将自动无效。这也就
*			函数重定义的概念。
*
*			为了加强模块化，我们建议将中断服务程序分散到对应的驱动模块文件中。比如systick中断服务程序
*			放在 bsp_timer.c 文件中。
*
*********************************************************************************************************
*/

#include "stm32f4xx_it.h"
#include <stdio.h>
#include  <includes.h>
#define ERR_INFO "\r\nEnter HardFault_Handler, System Halt.\r\n"

unsigned int 	CountTimes    = 0;//计时
double 			OnFreq               = 0 ;//频率
unsigned char 	measureflag   = 0;//测量完毕的标志
unsigned char 	PulseNumFlag  = 0;//用于记录已经发射几个脉冲--控制个数
extern unsigned char PulseNUM ;//发射脉冲的个数

unsigned char 	PreRecvieData = 0;
unsigned char 	Reciv[20];
unsigned char 	RecivNum      = 0;
unsigned char 	IsCMDflag     = 0;
unsigned char 	UploadFlag    = 0;

unsigned char yuliang_buf[25];
extern unsigned char takeflag;
/*
*********************************************************************************************************
*	Cortex-M3 内核异常中断服务程序
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*	函 数 名: NMI_Handler
*	功能说明: 不可屏蔽中断服务程序。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void NMI_Handler(void)
{
}

/*
*********************************************************************************************************
*	函 数 名: HardFault_Handler
*	功能说明: 硬件故障中断服务程序。其他异常处理被关闭，而又发生了异常，则触发。
*			  执行异常处理时，发生了异常，则触发。复位时默认使能。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void HardFault_Handler(void)
{
#if 1
  const char *pError = ERR_INFO;
  uint8_t i;

  for (i = 0; i < sizeof(ERR_INFO); i++)
  {
     USART1->DR = pError[i];
     /* 等待发送结束 */
     while ((USART1->SR & USART_FLAG_TC) == (uint16_t)RESET);
  }
#endif

  /* 当硬件失效异常发生时进入死循环 */
  while (1)
  {
  }
}

/*
*********************************************************************************************************
*	函 数 名: MemManage_Handler
*	功能说明: 内存管理异常中断服务程序。违反MPU设定的存储器访问规则时触发。 复位时默认未使能
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void MemManage_Handler(void)
{
  /* 当内存管理异常发生时进入死循环 */
  while (1)
  {
  }
}

/*
*********************************************************************************************************
*	函 数 名: BusFault_Handler
*	功能说明: 总线访问异常中断服务程序。取指令、数据读写、堆栈操作出现异常。 复位时默认未使能
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void BusFault_Handler(void)
{
  /* 当总线异常时进入死循环 */
  while (1)
  {
  }
}

/*
*********************************************************************************************************
*	函 数 名: UsageFault_Handler
*	功能说明: 用法错误中断服务程序。执行未定义指令、非对齐操作、除零时触发。 复位时默认未使能
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void UsageFault_Handler(void)
{
  /* 当用法异常时进入死循环 */
  while (1)
  {
  }
}

/*
*********************************************************************************************************
*	函 数 名: SVC_Handler
*	功能说明: 通过SWI指令的系统服务调用中断服务程序。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void SVC_Handler(void)
{
}

/*
*********************************************************************************************************
*	函 数 名: DebugMon_Handler
*	功能说明: 调试监视器中断服务程序。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void DebugMon_Handler(void)
{
}

/*
*********************************************************************************************************
*	函 数 名: PendSV_Handler
*	功能说明: 可挂起的系统服务调用中断服务程序。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void PendSV_Handler(void)
{
}
























/*------------------------------------------测量频率-----------------------------------------------*/
void TIM4_IRQHandler(void)
{
	//ucoiii里面进入硬件中断服务函数需要以下函数
	OSIntEnter();//进入中断
	// 外部脉冲达到100个
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		CountTimes = TIM2->CNT;//得到时间,结束计时
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		OnFreq = (double)30000000/(double)(CountTimes);//算出频率
		TIM_Cmd(TIM4,DISABLE);
		TIM4->CNT = 0;
		//printf("FINISHED MEASURE tim2 :%d\r\n",CountTimes);
		TIM_Cmd(TIM2,DISABLE);
		TIM2->CNT = 0;//关闭计时,等下一次测量
		measureflag = 1;
	}
	//第15个脉冲开始计时
	if(TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET)
	{	
		TIM2->CNT = 0;
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
		TIM_Cmd(TIM2,ENABLE);//开始计时了
	}
	OSIntExit();//退出中断--ucosiii
}


/*------------------------------------------激励脉冲-----------------------------------------------*/
void TIM3_IRQHandler(void)
{
	//ucoiii里面进入硬件中断服务函数需要以下函数
	OSIntEnter();//进入中断
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		PulseNumFlag++;
		if(PulseNumFlag == PulseNUM)
		{
			PulseNumFlag = 0;
			PulseNUM = 0;
			TIM_Cmd(TIM3,DISABLE);//达到脉冲个数之后就关闭定时器，停止发射
		}
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
	OSIntExit();//退出中断--ucosiii
}


void EXTI9_5_IRQHandler(void)
{
	OSIntEnter();
		if(EXTI_GetITStatus(EXTI_Line9) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line9);
		}
	OSIntExit();
}






void EXTI0_IRQHandler(void)
{
	int i;
	OSIntEnter();
	if(EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line0);
		if(takeflag == 0)
		{
			memset(yuliang_buf,0,25);
			strcat(yuliang_buf,"20");
			strcat(yuliang_buf,get_time());
			//printf("%s",yuliang_buf);
			takeflag = 1;//读取了一此
		}
		
	}
	OSIntExit();
}