#include "Shoot_Task.h"
#include "main.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "laser.h"
#include "Shoot_Task.h"
#include "judge_analysis.h"

#if INCLUDE_uxTaskGetStackHighWaterMark
uint32_t Shoot_task_stack;
#endif

float Fric_Speed[2];

const motor_measure_t *moto_fric1;
const motor_measure_t *moto_fric2;

static const fp32 PID_FRIC[] = {FRIC_KP, FRIC_KI, FRIC_KD};

int16_t Fric_Current[2] = {0, 0};

PidTypeDef pid_fric_spd[2];

//按键按下时长
uint16_t rc_key_fric_time;
uint16_t rc_key_shoot_time;
//获取遥控器信息
const RC_ctrl_t *SHOOT_RC;
//初始化摩擦轮标识
int fric_flag = 0;
int shoot_flag = 0;
int return_flag = 0;
//初始化速度 角度
float speed_shoot = 0;
float angle_shoot = 0;
float angle_shoot_remain = 0;
//PID参数定义
static const fp32 PID_SHOOT_SPEED[] = {SHOOT_SPEED_KP, SHOOT_SPEED_KI, SHOOT_SPEED_KD};
//初始化电流初值
int16_t current_shoot = 0;
//PID结构体定义
PidTypeDef pid_shoot_speed;
//获取云台编码值信息
const motor_measure_t *moto_trigger;

void ShootTask(void *pvParameters)
{	
		PID_Init(&pid_shoot_speed, PID_POSITION, PID_SHOOT_SPEED, SHOOT_SPEED_MAX_OUT, SHOOT_SPEED_MAX_IOUT);
		for(int16_t NUM = 0; NUM < 2; NUM++)
		{		
			//PID速度环，确保两个摩擦轮的速度一致
			PID_Init(&pid_fric_spd[NUM], PID_POSITION, PID_FRIC, MAX_FRIC_CAN_CURRENT, 4000);
		}
    while (1)
    {
			//获取遥控指针变量
			SHOOT_RC = get_remote_control_point();
			moto_trigger = get_Trigger_Motor_Measure_Point();
			moto_fric1 = get_Fric1_Motor_Measure_Point();
			moto_fric2 = get_Fric2_Motor_Measure_Point();	
			shoot_behaviour();
			//判断摩擦轮是否启动
			if(fric_flag == 0)	
			{
				//摩擦轮关闭
				Fric_Current[0] = PID_Calc(&pid_fric_spd[0], moto_fric1->speed_rpm, 0);
			  Fric_Current[1] = PID_Calc(&pid_fric_spd[1], moto_fric2->speed_rpm, 0);	
				CAN_CMD_SHOOT(Fric_Current[0], Fric_Current[1], current_shoot, 0);
				laser_off();
			}
      else if(fric_flag != 0)
			{
				//摩擦轮启动
				if(JudgementData.game_robot_state_t.shooter_id2_42mm_speed_limit == 10)
				{
					Fric_Current[0] = PID_Calc(&pid_fric_spd[0], moto_fric1->speed_rpm, 3800);
					Fric_Current[1] = PID_Calc(&pid_fric_spd[1], moto_fric2->speed_rpm, -3800);	
				}
				if(JudgementData.game_robot_state_t.shooter_id2_42mm_speed_limit == 16)
				{
					Fric_Current[0] = PID_Calc(&pid_fric_spd[0], moto_fric1->speed_rpm, 6800);
					Fric_Current[1] = PID_Calc(&pid_fric_spd[1], moto_fric2->speed_rpm, -6800);
				}
				CAN_CMD_SHOOT(Fric_Current[0], Fric_Current[1], current_shoot, 0);
				laser_on();
			}
			    if(shoot_flag == 1 && fric_flag != 0)
					{
						//拨弹
						current_shoot = PID_Calc(&pid_shoot_speed, moto_trigger->speed_rpm, 1500);
						CAN_CMD_SHOOT(Fric_Current[0], Fric_Current[1], current_shoot, 0);
					}
					if(shoot_flag == 2)
					{
						//回拨
						current_shoot = PID_Calc(&pid_shoot_speed, moto_trigger->speed_rpm, -1500);
						CAN_CMD_SHOOT(Fric_Current[0], Fric_Current[1], current_shoot, 0);
					}			
					if(shoot_flag == 0)
					{
						//停止拨弹
						current_shoot = PID_Calc(&pid_shoot_speed, moto_trigger->speed_rpm, 0);
						CAN_CMD_SHOOT(Fric_Current[0], Fric_Current[1], current_shoot, 0);
					}
			//系统延时
      vTaskDelay(1);
			
#if INCLUDE_uxTaskGetStackHighWaterMark
        Shoot_task_stack = uxTaskGetStackHighWaterMark(NULL);
#endif
    }
}

void shoot_behaviour(void)
{
    //F键摩擦轮启动
	  if( SHOOT_RC -> key.v == KEY_PRESSED_OFFSET_F)
	  {
				if (rc_key_fric_time < 1000)
			{
				rc_key_fric_time++;
			}
			else
			{
				rc_key_fric_time = 0;
			}
				if( rc_key_fric_time == 500 )
			{
					fric_flag = ~fric_flag ;
			}
		}
	  //左拨杆下拨 拨弹
	  if( SHOOT_RC -> mouse.press_l==1 )
	  {
	    shoot_flag = 1 ;
	  }
		//按R回拨弹丸
		else if( SHOOT_RC -> key.v == KEY_PRESSED_OFFSET_R )
	  {
	    shoot_flag = 2 ;
	  }		
		else
		{
			shoot_flag = 0 ;
		}
}
