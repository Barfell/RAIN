#ifndef __USART_SERIAL_H                           
#define __USART_SERIAL_H

/* 头文件处理 */
#define USER_UARTSIZE	800
/* 全局变量声明 */
typedef struct 
{
    USART_TypeDef* USARTx;
    USART_InitTypeDef* USART_InitStruct;
    BOOL USART_EnableTxInt;
    BOOL USART_EnableRxInt;
    
    U16 USART_PinRx;
    U16 USART_PinTx;
    
    GPIO_TypeDef* USART_PinRxGpio;
    GPIO_TypeDef* USART_PinTxGpio;
    
    U32 USART_Clk;
    U32 USART_PinRxGpioClk;
    U32 USART_PinTxGpioClk;
    U32 USART_APB2PeriphAfioClk;
      
    U8 USART_IRQChannel;
}XSERIAL;



/* 对外函数声明 */

VOID UsartSend(USART_TypeDef* USARTx, U8 *pBuf, U16 u16Length);
VOID ReadUsartData(USART_TypeDef* USARTx, U8 *pBuf, U16 *pLength);
VOID ReadUsartDataEx(USART_TypeDef* USARTx, U8 *pBuf, U16 *pLength);
VOID ReadUsartDataSolod(USART_TypeDef* USARTx, U8 *pBuf, U16 u16Length);
U32 GetUartReceiveTime(USART_TypeDef* USARTx);
VOID UartDeinit(USART_TypeDef* USARTx);
#endif
