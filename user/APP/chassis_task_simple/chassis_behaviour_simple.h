#ifndef CHASSIS_BEHAVIOUR_SIMPLE_H
#define CHASSIS_BEHAVIOUR_SIMPLE_H

#include "main.h"
#include "math.h"
#include "stm32f4xx.h"
#include "Chassis_Task_Simple.h"
#include "user_lib.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

#include "pid.h"
#include "INS_Task.h"
#include "gimbal_behaviour_simple.h"
#include "Remote_Control.h"
#include "CAN_Receive.h"

//WHEEL 速度环PID参数 最大输出 积分限幅
#define WHEEL_SPEED_KP 25.0f
#define WHEEL_SPEED_KI 0.0f
#define WHEEL_SPEED_KD 0.0f
#define WHEEL_SPEED_MAX_OUT 16000.0f
#define WHEEL_SPEED_MAX_IOUT 1000.0f


//遥控器速度解算幅度
#define RC_SPEED 4
//底盘跟随云台速度
#define AUTO_SPPED 1

typedef enum
{
  CHASSIS_ZERO_FORCE = 0, //底盘无力
  CHASSIS_RC_CONTROL,  //遥控器或PC控制
	CHASSIS_RC_AUTO,
} Chassis_Mode;

extern const RC_ctrl_t *CHASSIS_RC;
extern const cap_measure_t *cap;
extern void chassis_init(void);
extern void chassis_mode_behaviour(void);
extern int16_t speed_x, speed_y, speed_turn;

#endif
