#include "uart_blt566.h"
#include "stm32f4xx.h"

#include "imu901.h"

void UART8_Blt566_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE); //使能GPIOE时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART8,ENABLE);//使能UART8时钟
 
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource0,GPIO_AF_UART8); //GPIOE0复用为UART8
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource1,GPIO_AF_UART8); //GPIOE1复用为UART8
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1 ; //GPIOE0与GPIOGE1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOE,&GPIO_InitStructure); //初始化PE0,PE1
	
	USART_InitStructure.USART_BaudRate = 921600;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(UART8, &USART_InitStructure); //初始化串口8
	
	USART_Cmd(UART8, ENABLE);  //使能串口68
	
	USART_ClearFlag(UART8, USART_FLAG_TC);
		
	USART_ITConfig(UART8, USART_IT_RXNE, ENABLE);//开启相关中断

	NVIC_InitStructure.NVIC_IRQChannel = UART8_IRQn;//串口8中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=4;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、	
}

void UART8_IRQHandler(void) //串口8中断服务程序
{
	if(USART_GetITStatus(UART8, USART_IT_RXNE) != RESET)  //接收中断
	{
		Blt566_Data_handle();
	} 
}

#define blt566_receive_buf_size UART8_BUFF_SIZE - 1

uint8_t Blt566_receive_temp_buf[UART8_BUFF_SIZE] = {0};
uint8_t Blt566_receive_buf[UART8_BUFF_SIZE] = {0};

uint16_t blt566_receive_buf = 0;

uint8_t temp_buf = 0;
void Blt566_Data_handle(void)
{
  temp_buf = (short)(USART_ReceiveData(UART8) & 0xFF);//读取接收到的数据;
	
		if(imu901_unpack(temp_buf))
		{
			if(rxPacket.startByte2 == UP_BYTE2)
			{
				//数据包解析
				atkpParsing(&rxPacket);
				
			}
		}
}
