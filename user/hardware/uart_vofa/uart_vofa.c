#include "uart_vofa.h"
u8 rx_flag = 0;                         //����֡���ձ�־
/**************************************
@����             ���ڵĳ�ʼ������
@���ù�ϵ           
@�������           u32 Bound,eg:115200
@����ֵ                ��
@˵��             PE7 => USART7_RX��PE8 => USART7_TX
***************************************/
void Init_Uart7(u32 Bound)
{
    NVIC_InitTypeDef NVIC_InitStructure;      //�����жϽṹ��
    GPIO_InitTypeDef GPIO_InitStructure;      //����IO��ʼ���ṹ�� 
    USART_InitTypeDef USART_InitStructure;    //���崮�ڽṹ��

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE); //ʹ��GPIOEʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART7,ENABLE);//ʹ��USART7ʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);          //DMA1ʱ��ʹ�� 
    //GPIO���Ÿ���ӳ��
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource7,GPIO_AF_UART7);       //���Ÿ���ӳ��  PE7 => USART7_RX
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource8,GPIO_AF_UART7);      //���Ÿ���ӳ�� PE8 => USART7_TX
    //GPIO�ṹ������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8;        //�ܽ�ָ��  PE7  PE8
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                  //���ù���   
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                //���츴����� 
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                  //����
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;             //�ٶ�50MHz   
    GPIO_Init(GPIOE, &GPIO_InitStructure);                        //��ʼ��       
    //Usart�� NVIC ����  
    USART_InitStructure.USART_BaudRate = Bound;                                       //���������ã�һ������Ϊ9600
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                       //�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                            //һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;                               //����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   //��Ӳ������������ 
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                   //�շ�ģʽ   
    USART_Init(UART7,&USART_InitStructure);                                          //��ʼ������
    //ʹ�ܴ���
    USART_Cmd(UART7, ENABLE);    
    //�������ڿ����ж�
    USART_ITConfig(UART7, USART_IT_IDLE, ENABLE);    
    NVIC_InitStructure.NVIC_IRQChannel = UART7_IRQn;         //�����ж�ͨ��  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;   //��ռ���ȼ�3  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;         //�����ȼ�3  
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //IRQͨ��ʹ��  
    NVIC_Init(&NVIC_InitStructure);                           //����ָ���Ĳ�����ʼ��NVIC�Ĵ���
}

/**************************************
@����             DMA�ķ��ͳ�ʼ������
@���ù�ϵ           ��
@�������           
    @Usartx             ָ���Ĵ��ڣ��� USART1
    @Par                DMA�����ַ���� (u32)&USART1->DR
    @DMA_Streamx_tx     ����DMA1�ķ�����������DMA1_Stream0~DMA1_Stream7���μ��ֲ�
    @DMA_Chx            ����DMA��ͨ��ѡ��DMA_Channel_0~DMA_Channel_7
    @Tx_Buf             ����DMA�ķ��ͻ�����
    @Tx_Size            ���������ݸ���
    @DMA_Streamx_Irqn   DMA���������жϣ��� DMA2_Stream7_IRQn
@����ֵ                ��
@˵��             ��
***************************************/
void Init_Dma_Tx_Config(USART_TypeDef *Usartx,u32 Par, DMA_Stream_TypeDef *DMA_Streamx_Tx, 
                        u32 DMA_Chx,u32 Tx_Buf, u16 Tx_Size, u32 DMA_Streamx_Irqn)
{
    NVIC_InitTypeDef NVIC_InitStructure;      //�����жϽṹ��
    DMA_InitTypeDef DMA_InitStructure;        //����DMA�ṹ��
    DMA_DeInit(DMA_Streamx_Tx);                                                //��DMA�ķ�������������Ϊȱʡֵ 
    while (DMA_GetCmdStatus(DMA_Streamx_Tx) != DISABLE) ;                      //�ȴ�DMA�ķ���������������                      

    //DMA�Ľṹ������
    DMA_InitStructure.DMA_Channel = DMA_Chx;                                   //ͨ��ѡ��  
    DMA_InitStructure.DMA_PeripheralBaseAddr = Par;                            //DMA�����ַ  
    DMA_InitStructure.DMA_Memory0BaseAddr = Tx_Buf;                            //DMA �洢��0��ַ  
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;                    //�洢��������ģʽ  
    DMA_InitStructure.DMA_BufferSize = Tx_Size;                                //���ݴ�����   
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;           //���������ģʽ  
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                    //�洢������ģʽ  
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;    //�������ݳ���:8λ  
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;            //�洢�����ݳ���:8λ  
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                              // ʹ����ͨģʽ   
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                      //�е����ȼ�  
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;                     //��ʼ��DMA_FIFOMode 
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;              //FIFO ��ֵѡ��
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;                //�洢��ͻ�����δ���  
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;        //����ͻ�����δ���  
    DMA_Init(DMA_Streamx_Tx, &DMA_InitStructure);                              //��ʼ��DMA Stream  

    //DMA �ж�����    
    NVIC_InitStructure.NVIC_IRQChannel = DMA_Streamx_Irqn;                     //DMA�����������ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;                  //��ռ���ȼ�4
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                         //�����ȼ�0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                            //IRQͨ��ʹ�� 
    NVIC_Init(&NVIC_InitStructure);                                            //����ָ���Ĳ�����ʼ��NVIC�Ĵ���

    USART_DMACmd(Usartx,USART_DMAReq_Tx,ENABLE);                               //ʹ�ܴ���1��DMA����    
    DMA_Cmd(DMA_Streamx_Tx, DISABLE);                                          //�ر�DMA���� 
    DMA_ITConfig(DMA_Streamx_Tx,DMA_IT_TC,ENABLE);                             //ʹ��DMA�ķ����������ж�
}

/**************************************
@����             DMA�Ľ��ճ�ʼ������
@���ù�ϵ           ��
@�������           
@Usartx             ָ���Ĵ��ڣ��� USART1
@Par                DMA�����ַ���� (u32)&USART1->DR
@DMA_Streamx_rx     ����DMA1�Ľ�����������DMA1_Stream0~DMA1_Stream7���μ��ֲ�
@DMA_Chx            ����DMA��ͨ��ѡ��DMA_Channel_0~DMA_Channel_7
@Rx_Buf             ����DMA�Ľ��ջ�����
@Rx_Size            ����������ݸ���
@����ֵ                ��
@˵��             ��
***************************************/
void Init_Dma_Rx_Config(USART_TypeDef *Usartx, u32 Par, DMA_Stream_TypeDef *DMA_Streamx_Rx, 
                        u32 DMA_Chx,u32 Rx_Buf, u16 Rx_Size)
{
    DMA_InitTypeDef DMA_InitStructure;        //����DMA�ṹ��
    DMA_DeInit(DMA_Streamx_Rx);                                              //��DMA�Ľ�������������Ϊȱʡֵ
    while (DMA_GetCmdStatus(DMA_Streamx_Rx) != DISABLE);                     //�ȴ�DMA�Ľ��������������� 

    //DMA�Ľṹ������ 
    DMA_InitStructure.DMA_Channel = DMA_Chx;                                 //ͨ��ѡ��  
    DMA_InitStructure.DMA_PeripheralBaseAddr = Par;                          //DMA�����ַ  
    DMA_InitStructure.DMA_Memory0BaseAddr = Rx_Buf;                          //DMA �洢��0��ַ  
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory ;                 //���赽�洢��ģʽ  
    DMA_InitStructure.DMA_BufferSize = Rx_Size;                              //���ݴ�����   
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;         //���������ģʽ  
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                  //�洢������ģʽ  
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //�������ݳ���:8λ  
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;          //�洢�����ݳ���:8λ  
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                            // ʹ����ͨģʽ   
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                    //�е����ȼ�  
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;                   //��ʼ��DMA_FIFOMode 
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;            //FIFO ��ֵѡ��
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;              //�洢��ͻ�����δ���  
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;      //����ͻ�����δ���  
    DMA_Init(DMA_Streamx_Rx, &DMA_InitStructure);                            //��ʼ��DMA Stream  

    USART_DMACmd(Usartx,USART_DMAReq_Rx,ENABLE);                             //ʹ�ܴ��ڵ�DMA����   
    DMA_Cmd(DMA_Streamx_Rx, ENABLE);                                         //����DMA����    
}

/**************************************
@����             DMA�ķ���ʹ��
@���ù�ϵ           ��
@�������           
@DMA_Streamx_tx     ����DMA�ķ�����������DMA1_Stream0~DMA1_Stream7 / DMA2_Stream0~DMA2_Stream7
@ndtr               ����DMA�ķ������ݸ���
@����ֵ                ��
@˵��             ��
***************************************/
void Dma_Send_Enable(DMA_Stream_TypeDef *DMA_Streamx_tx,u16 ndtr)
{
    DMA_Cmd(DMA_Streamx_tx, DISABLE);                         //�ر�DMA����     
    while (DMA_GetCmdStatus(DMA_Streamx_tx) != DISABLE);        //ȷ��DMA���Ա�����        
    DMA_SetCurrDataCounter(DMA_Streamx_tx,ndtr);              //���ݴ�����   
    DMA_Cmd(DMA_Streamx_tx, ENABLE);                          //����DMA���� 
}

/**************************************
@����             DMA�ķ�������жϺ���
@���ù�ϵ           ��
@�������           ��
@����ֵ                ��
@˵��             ��
***************************************/

void DMA1_Stream1_IRQHandler(void)  
{
    if(DMA_GetFlagStatus(DMA1_Stream1,DMA_FLAG_TCIF1)!=RESET)     //�ȴ�DMA1_Steam1�������  
    { 
        DMA_ClearFlag(DMA1_Stream1,DMA_FLAG_TCIF1);           //���DMA1_Steam1������ɱ�־ 
        DMA_Cmd(DMA1_Stream1,DISABLE);                        //�ر�DMA           
        DMA_ClearITPendingBit(DMA1_Stream1, DMA_IT_TCIF1);    //����ж�����λ     
    }  
}  
