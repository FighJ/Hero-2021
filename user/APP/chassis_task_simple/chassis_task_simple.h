#ifndef CHASSIS_TASK_SIMPLE_H
#define CHASSIS_TASK_SIMPLE_H

#include "main.h"
#include "stm32f4xx.h"
#include "user_lib.h"
#include "pid.h"
#include "CAN_Receive.h"
#include "chassis_behaviour_simple.h"

//底盘任务控制间隔 0.002s
#define CHASSIS_CONTROL_TIME 0.002

#define CHASSIS_ACCEL_X_NUM 0.1f
#define CHASSIS_ACCEL_Y_NUM 0.1f

extern first_order_filter_type_t chassis_cmd_slow_set_vx;
extern first_order_filter_type_t chassis_cmd_slow_set_vy;

//模式赋值变量
extern uint8_t MODE_CHASSIS;

extern void chassis_task_Simple(void *pvParameters);

#endif
