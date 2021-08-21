#ifndef SERVO_TASK_H
#define SERVO_TASK_H

#include "main.h"
#include "Remote_Control.h"

//按键最长有效时间
#define RC_KEY_LONG_TIME 2000

#define REEL_SPEED_KP 5.00f
#define REEL_SPEED_KI 0.00f
#define REEL_SPEED_KD 0.0f
#define REEL_SPEED_MAX_OUT 30000.0f
#define REEL_SPEED_MAX_IOUT 4000.0f

#define SHELL_SPEED_KP 20.00f
#define SHELL_SPEED_KI 0.00f
#define SHELL_SPEED_KD 0.0f
#define SHELL_SPEED_MAX_OUT 8000.0f
#define SHELL_SPEED_MAX_IOUT 4000.0f

#define SHELL_ANGLE_KP 5.00f
#define SHELL_ANGLE_KI 0.00f
#define SHELL_ANGLE_KD 0.0f
#define SHELL_ANGLE_MAX_OUT 8000.0f
#define SHELL_ANGLE_MAX_IOUT 4000.0f

//补弹标识
extern int supply_flag;
extern void ServoTask(void *pvParameters);
extern void supply_behaviour(void);

#endif
