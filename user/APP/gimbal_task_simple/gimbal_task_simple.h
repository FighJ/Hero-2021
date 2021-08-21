#ifndef GIMBAL_TASK_SIMPLE_H
#define GIMBAL_TASK_SIMPLE_H

#include "main.h"
#include "stm32f4xx.h"

#include "pid.h"
#include "INS_Task.h"
#include "CAN_Receive.h"
#include "gimbal_behaviour_simple.h"

#define Gimbal_ModeChannel 0
#define Pitch_Mode_Channel 2
#define Yaw_Mode_Channel 3

//模式赋值变量
extern uint8_t MODE;
extern uint8_t LAST_MODE;

extern void GIMBAL_task_Simple(void *pvParameters);

#endif
