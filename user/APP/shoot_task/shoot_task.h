#ifndef SHOOT_TASK_H
#define SHOOT_TASK_H

#include "main.h"
#include "Remote_Control.h"
#include "CAN_Receive.h"
#include "pid.h"
#include "stm32f4xx.h"
#include "gimbal_behaviour_simple.h"

//按键最长有效时间
#define RC_KEY_MAX_TIME 2000

//SHOOT 速度环PID参数 最大输出 积分限幅
#define SHOOT_SPEED_KP 23.0f
#define SHOOT_SPEED_KI 0.0f
#define SHOOT_SPEED_KD 0.0f
#define SHOOT_SPEED_MAX_OUT 10000.0f
#define SHOOT_SPEED_MAX_IOUT 1000.0f

//积分限幅
#define LIMIT 4000

//摩擦轮电机速度环PID参数
#define FRIC_KP 20.0f
#define FRIC_KI 0.0f
#define FRIC_KD 0

//摩擦轮M3508最大发送电流值
#define MAX_FRIC_CAN_CURRENT 16000.0f

//无其他电机
static uint8_t NONE = 0;

//摩擦轮启动标识
extern int fric_flag;
//拨弹电机CAN发送电流值
extern int16_t current_shoot;
//拨弹标识
extern int shoot_flag;

extern void ShootTask(void *pvParameters);

extern void shoot_behaviour(void);

extern void shoot_init(void);


#endif
