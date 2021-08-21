/**
  ****************************(C) COPYRIGHT 2016 DJI****************************
  * @file       main.c/h
  * @brief      stm32��ʼ���Լ���ʼ����freeRTOS��h�ļ��������ȫ�ֺ궨���Լ�
  *             typedef һЩ������������
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Dec-26-2018     RM              1. ���
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2016 DJI****************************
  */
#include "main.h"
#include "stm32f4xx.h"
#include "usart.h"
#include "adc.h"
#include "buzzer.h"
#include "can.h"
#include "delay.h"
#include "flash.h"
#include "laser.h"
#include "led.h"
#include "power_ctrl.h"
#include "rc.h"
#include "rng.h"
#include "sys.h"
#include "timer.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "calibrate_task.h"
#include "remote_control.h"
#include "start_task.h"
#include "CAN_Receive.h"
#include "dac.h"
#include "uart_blt566.h"
#include "Vofa_Task.h"
#include "vofa.h"
#include "uart_vofa.h"

void BSP_init(void);
const RC_ctrl_t *rcl;
const motor_measure_t *yaw, *pitch;

int main(void)
{
    BSP_init();
    delay_ms(100);
    startTast();
    vTaskStartScheduler();
    while (1)
    {
			
    }
}

void BSP_init(void)
{
    //�ж��� 4
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    //��ʼ���δ�ʱ��
    delay_init(configTICK_RATE_HZ);
    //��ˮ�ƣ����̵Ƴ�ʼ��
    led_configuration();
	  //stm32 �����¶ȴ�����ADC��ʼ��
    temperature_ADC_init();
		//��Դ���ʿ���DAC��ʼ��
//		Dac1_Init();
	  //��������ʼ��
    buzzer_init(30000, 90);
	  //ң������ʼ��
    remote_control_init();
	  //CAN�ӿڳ�ʼ��
    CAN1_mode_init(CAN_SJW_1tq, CAN_BS2_2tq, CAN_BS1_6tq, 5, CAN_Mode_Normal);
    CAN2_mode_init(CAN_SJW_1tq, CAN_BS2_2tq, CAN_BS1_6tq, 5, CAN_Mode_Normal);
	  //24������ƿڳ�ʼ��
    power_ctrl_configuration();
		//USART4��ʼ�� ���ڶ�ȡ����ϵͳ
		USART4_Judge_Config();
		//UART8��ʼ�� ���ڶ�ȡBlt566����
		UART8_Blt566_Config();
		//USART2��ʼ�� ����Vofa+��λ��
		Init_Uart7(115200);
		Init_Dma_Tx_Config(UART7, (u32)(&UART7->DR), DMA1_Stream1, DMA_Channel_5, (u32)Vofa.message, sizeof(Vofa.message), DMA1_Stream1_IRQn);
}
