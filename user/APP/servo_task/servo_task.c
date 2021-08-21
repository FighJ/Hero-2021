#include "Servo_Task.h"
#include "main.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "chassis_behaviour.h"
#include "Servo_Task.h"

#if INCLUDE_uxTaskGetStackHighWaterMark
uint32_t Servo_task_stack;
#endif

PidTypeDef pid_reel_speed;
PidTypeDef pid_shell_speed;
PidTypeDef pid_shell_angle;
//static const fp32 PID_REEL_SPEED[] = {REEL_SPEED_KP, REEL_SPEED_KI, REEL_SPEED_KD};
static const fp32 PID_SHELL_SPEED[] = {SHELL_SPEED_KP, SHELL_SPEED_KI, SHELL_SPEED_KD};
static const fp32 PID_SHELL_ANGLE[] = {SHELL_ANGLE_KP, SHELL_ANGLE_KI, SHELL_ANGLE_KD};
//按键按下时长
uint16_t rc_key_time;
int16_t current_reel = 0;
int16_t current_shell_remain = 0;
int16_t current_shell = 0;
int16_t t = 0;
int16_t n = 0;
int8_t flag = 1;
int32_t angle_shell = 0;
int32_t angle_shell_out = 0;
int32_t angle_shell_init = 0;
//获取遥控器信息
const RC_ctrl_t *SUPPLY_RC;
//初始化补弹标识
int supply_flag = 0;
//const motor_measure_t *moto_reel;
const motor_measure_t *moto_shell;
uint16_t time_back = 0;
void ServoTask(void *pvParameters)
{

//  	PID_Init(&pid_reel_speed, PID_POSITION, PID_REEL_SPEED, REEL_SPEED_MAX_OUT, REEL_SPEED_MAX_IOUT);
	  PID_Init(&pid_shell_speed, PID_POSITION, PID_SHELL_SPEED, SHELL_SPEED_MAX_OUT, SHELL_SPEED_MAX_IOUT);
		PID_Init(&pid_shell_angle, PID_POSITION, PID_SHELL_ANGLE, SHELL_ANGLE_MAX_OUT, SHELL_ANGLE_MAX_IOUT);
	  moto_shell = get_Shell_Motor_Measure_Point();
	  angle_shell_init = moto_shell -> total_ecd_out_3508;
    while (1)
    {
			//获取遥控指针变量
		  	SUPPLY_RC = get_remote_control_point();
		  	moto_shell = get_Shell_Motor_Measure_Point();
				t++;
				if(moto_shell->speed_rpm >= 0 && moto_shell->speed_rpm < 5)
				{
					n++;
				}
				if(moto_shell->speed_rpm < 0 && moto_shell->speed_rpm > -5)
				{
					n++;
				}
				
				if(n >= 1600)
				{
					flag = !flag;
					n = 0;
				}			
			if(flag == 1)
			{
				if(t == 2)
				{
					angle_shell = angle_shell + 1;
					t = 0;
				}
			}
			else
			{
				if(t = 2)
				{
					angle_shell = angle_shell - 1;
					t = 0;
				}
				time_back ++;
				if(time_back == 860)
				{
					flag = 1;
					n = 0;
					time_back = 0;
				}
			}
			angle_shell_out = angle_shell + angle_shell_init;
			current_shell_remain = PID_Calc(&pid_shell_angle, moto_shell -> total_ecd_out_3508, angle_shell_out);
			current_shell = PID_Calc(&pid_shell_speed, moto_shell->speed_rpm, current_shell_remain);
			if( rc_ctrl.rc.s[0] == RC_SW_MID )
			{
				CAN_CMD_2006(0, 0, 0, 0);
			}
			else
				CAN_CMD_2006(0, current_shell, 0, 0);
			
			//系统延时
      vTaskDelay(1);
			
#if INCLUDE_uxTaskGetStackHighWaterMark
        Servo_task_stack = uxTaskGetStackHighWaterMark(NULL);
#endif
    }
}

void supply_behaviour(void)
{
	//补给开启
  if ( (SUPPLY_RC -> key.v & KEY_PRESSED_OFFSET_B) || ( (SUPPLY_RC -> rc.s[1] == RC_SW_UP)&&(SUPPLY_RC -> rc.s[0] == RC_SW_UP) ))
  {
		if (rc_key_time < RC_KEY_LONG_TIME)
    {
      rc_key_time++;
    }
  }
    else
    {
      rc_key_time = 0;
    }
		
	//键盘B键 进入补弹模式
  if( rc_key_time == 400 )
	{
	  supply_flag=~supply_flag;
	}
}
