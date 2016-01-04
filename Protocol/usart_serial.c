#include "kernel.h"

STATIC U32 g_u32Uart3RevCount = 0;
STATIC U8 g_u8LastValue;
extern PQueueInfo pUart3QueueInfo;//串口环形队列

unsigned char UartPreData=0;
unsigned char UartFinishFlag = 0;//主机下发命令结束标志


VOID USART1_IRQHandler(VOID)
{ 
	U8 u8Value = 0;
	OSIntEnter();//进入中断
	
	/* 串口1接收中断处理 */
  	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  	{ 
		USART_ClearFlag(USART1, USART_FLAG_RXNE);
		u8Value = USART_ReceiveData(USART1);
		//while(AddQueue(pUart3QueueInfo, u8Value) != TRUE);
		AddQueue(pUart3QueueInfo, u8Value);
		if(UartPreData == '\r' && u8Value == '\n')
		{UartFinishFlag = 1;}
  	}
	UartPreData = u8Value;//记录上一次的数据
	OSIntExit();//退出中断--ucosiii
}

U32 GetUartReceiveTime(USART_TypeDef* USARTx)
{
	U32 u32Value;
	u32Value = g_u32Uart3RevCount;
	return u32Value;
}
VOID UsartSend(USART_TypeDef* USARTx, U8 *pBuf, U16 u16Length)
{
	U16 u16Count;
	
	for(u16Count = 0; u16Count < u16Length; u16Count++)
	{
		/* 将要发送的数据写入串口1数据发送寄存器 */
		USART_SendData(USARTx, *(pBuf + u16Count)); 
		
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);//检查是否发送完
			
	}
}

VOID UartDeinit(USART_TypeDef* USARTx)
{  	/* 开启串口TX中断 */

	free(pUart3QueueInfo);
  	USART_Cmd(USARTx, DISABLE);
	
	USART_ITConfig(USARTx, USART_IT_RXNE, DISABLE);
	
	USART_ITConfig(USARTx, USART_IT_TXE, DISABLE);
	
}

VOID ReadUsartData(USART_TypeDef* USARTx, U8 *pBuf, U16 *pLength)
{
	U16 u16Count = 0;
	QueueDataType Value;
	PQueueInfo pQueueInfo;
	pQueueInfo = pUart3QueueInfo;
	while(DeleteQueue(pQueueInfo, &Value) == TRUE)
	{
		*(pBuf + u16Count) = Value;
		u16Count++;	
		if(u16Count > USER_UARTSIZE)
		{
			break;
		}
	}
	*pLength = u16Count;			
}


VOID ReadUsartDataEx(USART_TypeDef* USARTx, U8 *pBuf, U16 *pLength)
{
	U16 u16Count = 0;
	QueueDataType Value;
	PQueueInfo pQueueInfo;
	pQueueInfo = pUart3QueueInfo;
	while(DeleteQueue(pQueueInfo, &Value) == TRUE)
	{
		*(pBuf + u16Count) = Value;
		
		u16Count++;	
		if((Value == 0x0A) && (g_u8LastValue == 0x0D))
		{
			break;
		}
		g_u8LastValue = Value;
	}
	*pLength = u16Count;
}



VOID ReadUsartDataSolod(USART_TypeDef* USARTx, U8 *pBuf, U16 u16Length)
{
	U16 u16Count = 0;
	QueueDataType Value;
	PQueueInfo pQueueInfo;
	pQueueInfo = pUart3QueueInfo;
	while(u16Length != u16Count)
	{
		while(DeleteQueue(pQueueInfo, &Value) == TRUE)
		{
			*(pBuf + u16Count) = Value;
			
			u16Count++;	
		}
		
	}
}
