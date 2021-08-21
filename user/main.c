/**
  ****************************(C) COPYRIGHT 2016 DJI****************************
  * @file       main.c/h
  * @brief      stm32初始化以及开始任务freeRTOS。h文件定义相关全局宏定义以及
  *             typedef 一些常用数据类型
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Dec-26-2018     RM              1. 完成
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
    //中断组 4
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    //初始化滴答时钟
    delay_init(configTICK_RATE_HZ);
    //流水灯，红绿灯初始化
    led_configuration();
	  //stm32 板载温度传感器ADC初始化
    temperature_ADC_init();
		//电源功率控制DAC初始化
//		Dac1_Init();
	  //蜂鸣器初始化
    buzzer_init(30000, 90);
	  //遥控器初始化
    remote_control_init();
	  //CAN接口初始化
    CAN1_mode_init(CAN_SJW_1tq, CAN_BS2_2tq, CAN_BS1_6tq, 5, CAN_Mode_Normal);
    CAN2_mode_init(CAN_SJW_1tq, CAN_BS2_2tq, CAN_BS1_6tq, 5, CAN_Mode_Normal);
	  //24输出控制口初始化
    power_ctrl_configuration();
		//USART4初始化 用于读取裁判系统
		USART4_Judge_Config();
		//UART8初始化 用于读取Blt566数据
		UART8_Blt566_Config();
		//USART2初始化 用于Vofa+上位机
		Init_Uart7(115200);
		Init_Dma_Tx_Config(UART7, (u32)(&UART7->DR), DMA1_Stream1, DMA_Channel_5, (u32)Vofa.message, sizeof(Vofa.message), DMA1_Stream1_IRQn);
}
