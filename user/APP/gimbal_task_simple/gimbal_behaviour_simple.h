#ifndef GIMBAL_BEHAVIOUR_SIMPLE_H
#define GIMBAL_BEHAVIOUR_SIMPLE_H

#include "main.h"
#include "math.h"
#include "stm32f4xx.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

#include "pid.h"
#include "Remote_Control.h"
#include "CAN_Receive.h"
#include "Shoot_Task.h"

//PITCH 速度环PID参数 最大输出 积分限幅
#define PITCH_SPEED_KP 1.00f
#define PITCH_SPEED_KI 0.00f
#define PITCH_SPEED_KD 0.0f
#define PITCH_SPEED_MAX_OUT 20000.0f
#define PITCH_SPEED_MAX_IOUT 5000.0f

//PITCH 角度环PID参数 最大输出 积分限幅
#define PITCH_ANGLE_KP 100.0f
#define PITCH_ANGLE_KI 0.0f
#define PITCH_ANGLE_KD 0.5f
#define PITCH_ANGLE_MAX_OUT 20000.0f
#define PITCH_ANGLE_MAX_IOUT 5000.0f

//YAW 速度环PID参数 最大输出 积分限幅
#define YAW_SPEED_KP 1.00f
#define YAW_SPEED_KI 0.00f
#define YAW_SPEED_KD 0.0f
#define YAW_SPEED_MAX_OUT 20000.0f
#define YAW_SPEED_MAX_IOUT 4000.0f

//YAW 角度环PID参数 最大输出 积分限幅
#define YAW_ANGLE_KP 200.00f
#define YAW_ANGLE_KI 0.0f
#define YAW_ANGLE_KD 0.20f
#define YAW_ANGLE_MAX_OUT 20000.0f
#define YAW_ANGLE_MAX_IOUT 4000.0f

//YAW 速度环PID参数 最大输出 积分限幅
#define YAW_FOLLOW_SPEED_KP 1.0f
#define YAW_FOLLOW_SPEED_KI 0.0f
#define YAW_FOLLOW_SPEED_KD 0.0f
#define YAW_FOLLOW_SPEED_MAX_OUT 20000.0f
#define YAW_FOLLOW_SPEED_MAX_IOUT 4000.0f

//YAW 角度环PID参数 最大输出 积分限幅
#define YAW_FOLLOW_ANGLE_KP 50.0f
#define YAW_FOLLOW_ANGLE_KI 0.0f
#define YAW_FOLLOW_ANGLE_KD 0.20f
#define YAW_FOLLOW_ANGLE_MAX_OUT 20000.0f
#define YAW_FOLLOW_ANGLE_MAX_IOUT 4000.0f

//云台移动速度
#define pitch_rc_speed 1.0f
#define yaw_rc_speed 1.5f
#define yaw_auto_speed 0.01f

//电机码盘值最大以及中值
#define Half_ecd_range 4096
#define ecd_range 8191
//电机角度绝对零点
#define PITCH_Zero 4700
#define YAW_Zero  5500

//云台校准时间
#define Check_Max_Time 1000

typedef enum
{
  GIMBAL_ZERO_FORCE = 0, //云台无力
  GIMBAL_RC_CONTROL,  //遥控器或PC控制
	GIMBAL_RC_AUTO,
} Gimbal_Mode;

typedef enum
{
  UP = 0,
  MID,  
	DOWN,
} Gimbal_check_Mode;//左拨杆云台模式

extern const RC_ctrl_t *GIMBAL_RC;
extern const RC_ctrl_t *LAST_GIMBAL_RC;
//云台电机CAN发送电流值
extern int16_t current_pitch1;
extern int16_t current_yaw;

//云台校准初始化
extern int16_t Check_Time;

extern const motor_measure_t *moto_yaw;
extern const motor_measure_t *moto_pitch1;
extern const motor_measure_t *moto_pitch2;
extern float YAW_AUTO_Zero;
extern float PITCH1_OUT, YAW_AUTO_OUT, YAW_OUT, YAW_AUTO_Zero;

//云台PID初始化
extern void gimbal_init(void);
//不同模式下云台行为
extern void gimbal_mode_behaviour(void);
//检测左拨杆云台状态
//extern void gimbal_mode_check(void);
//extern void mode_change(void);
//云台电机驱动函数
extern void CAN_CMD_GIMBAL_SIMPLE(int16_t PITCH, int16_t YAW);
	
#endif
