//#include "usart.h"
//#include "stm32f4xx.h"
//#include "judge_analysis.h"

//void USART4_Judge_Config(void)
//{
//	GPIO_InitTypeDef	GPIO_InitStructure;
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
//  //USART4 PA0 TX PA1 RX
//	GPIO_InitStructure.GPIO_Mode   =   GPIO_Mode_AF;
//	GPIO_InitStructure.GPIO_OType  =   GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_Pin    =   GPIO_Pin_0 | GPIO_Pin_1;
//	GPIO_InitStructure.GPIO_PuPd   =   GPIO_PuPd_UP;
//	GPIO_InitStructure.GPIO_Speed  =   GPIO_Speed_100MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_UART4);
//	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_UART4);
//	
//	
//	USART_InitTypeDef   USART_InitStructure;
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
//	//Dbus Judge USART4_RX DMA1 Channel4 Stream2 
//	USART_InitStructure.USART_BaudRate              =   115200;
//	USART_InitStructure.USART_HardwareFlowControl   =   USART_HardwareFlowControl_None;
//	USART_InitStructure.USART_Mode                  =   USART_Mode_Rx|USART_Mode_Tx;
//	USART_InitStructure.USART_Parity                =   USART_Parity_No;
//	USART_InitStructure.USART_StopBits              =   USART_StopBits_1;
//	USART_InitStructure.USART_WordLength            =   USART_WordLength_8b;
//	USART_Init(UART4, &USART_InitStructure);
//	USART_DMACmd(UART4, USART_DMAReq_Rx, ENABLE);
//	USART_ITConfig(UART4, USART_IT_IDLE, ENABLE);
//  USART_Cmd(UART4, ENABLE);
//	
//	
//	NVIC_InitTypeDef	NVIC_InitStructure;
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
//	//Dudge数据读取
//	NVIC_InitStructure.NVIC_IRQChannel						=	UART4_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelCmd					=	ENABLE;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	=	7;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority			=	0;
//  NVIC_Init(&NVIC_InitStructure);
//	USART_ITConfig(UART4, USART_IT_IDLE, ENABLE);

//  DMA_InitTypeDef     DMA_InitStructure;
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
//	DMA_DeInit(DMA1_Stream2);
//  while (DMA_GetCmdStatus(DMA1_Stream2) != DISABLE)  {
//  }
//	//Dbus Judge USART3_RX DMA1 Channel4 Stream1 
//	DMA_InitStructure.DMA_Channel           =   DMA_Channel_4 ;
//	DMA_InitStructure.DMA_PeripheralBaseAddr=   (uint32_t)(&UART4->DR);
//	DMA_InitStructure.DMA_Memory0BaseAddr   =   (uint32_t)(JudgeDataBuffer);
//	DMA_InitStructure.DMA_DIR               =   DMA_DIR_PeripheralToMemory;
//	DMA_InitStructure.DMA_BufferSize        =   JudgeBufferLength;
//	DMA_InitStructure.DMA_PeripheralInc     =   DMA_PeripheralInc_Disable;
//	DMA_InitStructure.DMA_MemoryInc         =   DMA_MemoryInc_Enable;
//	DMA_InitStructure.DMA_MemoryDataSize    =   DMA_MemoryDataSize_Byte;
//	DMA_InitStructure.DMA_PeripheralDataSize=   DMA_PeripheralDataSize_Byte;
//	DMA_InitStructure.DMA_Mode              =   DMA_Mode_Normal;
//	DMA_InitStructure.DMA_Priority          =   DMA_Priority_VeryHigh;
//	DMA_InitStructure.DMA_FIFOMode          =   DMA_FIFOMode_Disable;
//	DMA_InitStructure.DMA_FIFOThreshold     =   DMA_FIFOThreshold_Full;
//	DMA_InitStructure.DMA_MemoryBurst       =   DMA_MemoryBurst_Single;
//	DMA_InitStructure.DMA_PeripheralBurst   =   DMA_PeripheralBurst_Single;
//	DMA_Init(DMA1_Stream2, &DMA_InitStructure);
//	DMA_Cmd(DMA1_Stream2, ENABLE);
//  USART_DMACmd(UART4, USART_DMAReq_Rx, ENABLE);

//}

#include "usart.h"
#include "stm32f4xx.h"
#include "judge_analysis.h"

void USART4_Judge_Config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
  //USART4 PA0 TX PA1 RX
	GPIO_InitStructure.GPIO_Mode   =   GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType  =   GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin    =   GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_PuPd   =   GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed  =   GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_UART4);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_UART4);
	
	
	USART_InitTypeDef   USART_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	//Dbus Judge USART4_RX DMA1 Channel4 Stream2 
	USART_InitStructure.USART_BaudRate              =   115200;
	USART_InitStructure.USART_HardwareFlowControl   =   USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                  =   USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStructure.USART_Parity                =   USART_Parity_No;
	USART_InitStructure.USART_StopBits              =   USART_StopBits_1;
	USART_InitStructure.USART_WordLength            =   USART_WordLength_8b;
	USART_Init(UART4, &USART_InitStructure);
	USART_DMACmd(UART4, USART_DMAReq_Rx, ENABLE);
	USART_ITConfig(UART4, USART_IT_IDLE, ENABLE);
  USART_Cmd(UART4, ENABLE);
	
	
	NVIC_InitTypeDef	NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	//Dudge数据读取
	NVIC_InitStructure.NVIC_IRQChannel						=	UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd					=	ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	=	7;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority			=	0;
  NVIC_Init(&NVIC_InitStructure);
	USART_ITConfig(UART4, USART_IT_IDLE, ENABLE);

  DMA_InitTypeDef     DMA_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
	DMA_DeInit(DMA1_Stream2);
  while (DMA_GetCmdStatus(DMA1_Stream2) != DISABLE)  {
  }
	//Dbus Judge USART3_RX DMA1 Channel4 Stream1 
	DMA_InitStructure.DMA_Channel           =   DMA_Channel_4 ;
	DMA_InitStructure.DMA_PeripheralBaseAddr=   (uint32_t)(&UART4->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr   =   (uint32_t)(JudgeDataBuffer);
	DMA_InitStructure.DMA_DIR               =   DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize        =   JudgeBufferLength;
	DMA_InitStructure.DMA_PeripheralInc     =   DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc         =   DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_MemoryDataSize    =   DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralDataSize=   DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_Mode              =   DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority          =   DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_FIFOMode          =   DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold     =   DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst       =   DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst   =   DMA_PeripheralBurst_Single;
	DMA_Init(DMA1_Stream2, &DMA_InitStructure);
	DMA_Cmd(DMA1_Stream2, ENABLE);
  USART_DMACmd(UART4, USART_DMAReq_Rx, ENABLE);

}
