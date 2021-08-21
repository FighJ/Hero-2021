#ifndef UART_VOFA_H
#define UART_VOFA_H
#include "sys.h"
#include "stm32f4xx.h"

#define Sbuff 120
#define Rbuff 120


extern u16 rx_len;
extern u8 SendBuffer[Sbuff];
extern u8 ReceiveBuffer[Rbuff];
extern u16 BufferLen;
extern u8 rx_flag; 


extern void Init_Uart7(u32 Bound);
extern void Init_Dma_Tx_Config(USART_TypeDef *Usartx,u32 Par, DMA_Stream_TypeDef *DMA_Streamx_Tx, u32 DMA_Chx,u32 Tx_Buf, u16 Tx_Size, u32 DMA_Streamx_Irqn);
extern void Init_Dma_Rx_Config(USART_TypeDef *Usartx, u32 Par, DMA_Stream_TypeDef *DMA_Streamx_Rx, u32 DMA_Chx,u32 Rx_Buf, u16 Rx_Size);
extern void Dma_Send_Enable(DMA_Stream_TypeDef *DMA_Streamx_tx,u16 ndtr);
extern void DMA1_Stream1_IRQHandler(void);

//DMA 接收数据流初始化
//  Init_Dma_Rx_Config(UART7, (u32)(&UART7->DR), DMA1_Stream3, DMA_Channel_5, (u32)ReceiveBuffer, Rbuff);
//DMA 发送数据流初始化
//Init_Dma_Tx_Config(UART7, (u32)(&UART7->DR), DMA1_Stream1, DMA_Channel_5, (u32)SendBuffer, Sbuff, DMA1_Stream1_IRQn);



#endif
