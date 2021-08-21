#include "uart_blt566.h"
#include "stm32f4xx.h"

#include "imu901.h"

void UART8_Blt566_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE); //ʹ��GPIOEʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART8,ENABLE);//ʹ��UART8ʱ��
 
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource0,GPIO_AF_UART8); //GPIOE0����ΪUART8
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource1,GPIO_AF_UART8); //GPIOE1����ΪUART8
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1 ; //GPIOE0��GPIOGE1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOE,&GPIO_InitStructure); //��ʼ��PE0,PE1
	
	USART_InitStructure.USART_BaudRate = 921600;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(UART8, &USART_InitStructure); //��ʼ������8
	
	USART_Cmd(UART8, ENABLE);  //ʹ�ܴ���68
	
	USART_ClearFlag(UART8, USART_FLAG_TC);
		
	USART_ITConfig(UART8, USART_IT_RXNE, ENABLE);//��������ж�

	NVIC_InitStructure.NVIC_IRQChannel = UART8_IRQn;//����8�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=4;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����	
}

void UART8_IRQHandler(void) //����8�жϷ������
{
	if(USART_GetITStatus(UART8, USART_IT_RXNE) != RESET)  //�����ж�
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
  temp_buf = (short)(USART_ReceiveData(UART8) & 0xFF);//��ȡ���յ�������;
	
		if(imu901_unpack(temp_buf))
		{
			if(rxPacket.startByte2 == UP_BYTE2)
			{
				//���ݰ�����
				atkpParsing(&rxPacket);
				
			}
		}
}
