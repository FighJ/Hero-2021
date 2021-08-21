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

//��������ʱ��
uint16_t rc_key_fric_time;
uint16_t rc_key_shoot_time;
//��ȡң������Ϣ
const RC_ctrl_t *SHOOT_RC;
//��ʼ��Ħ���ֱ�ʶ
int fric_flag = 0;
int shoot_flag = 0;
int return_flag = 0;
//��ʼ���ٶ� �Ƕ�
float speed_shoot = 0;
float angle_shoot = 0;
float angle_shoot_remain = 0;
//PID��������
static const fp32 PID_SHOOT_SPEED[] = {SHOOT_SPEED_KP, SHOOT_SPEED_KI, SHOOT_SPEED_KD};
//��ʼ��������ֵ
int16_t current_shoot = 0;
//PID�ṹ�嶨��
PidTypeDef pid_shoot_speed;
//��ȡ��̨����ֵ��Ϣ
const motor_measure_t *moto_trigger;

void ShootTask(void *pvParameters)
{	
		PID_Init(&pid_shoot_speed, PID_POSITION, PID_SHOOT_SPEED, SHOOT_SPEED_MAX_OUT, SHOOT_SPEED_MAX_IOUT);
		for(int16_t NUM = 0; NUM < 2; NUM++)
		{		
			//PID�ٶȻ���ȷ������Ħ���ֵ��ٶ�һ��
			PID_Init(&pid_fric_spd[NUM], PID_POSITION, PID_FRIC, MAX_FRIC_CAN_CURRENT, 4000);
		}
    while (1)
    {
			//��ȡң��ָ�����
			SHOOT_RC = get_remote_control_point();
			moto_trigger = get_Trigger_Motor_Measure_Point();
			moto_fric1 = get_Fric1_Motor_Measure_Point();
			moto_fric2 = get_Fric2_Motor_Measure_Point();	
			shoot_behaviour();
			//�ж�Ħ�����Ƿ�����
			if(fric_flag == 0)	
			{
				//Ħ���ֹر�
				Fric_Current[0] = PID_Calc(&pid_fric_spd[0], moto_fric1->speed_rpm, 0);
			  Fric_Current[1] = PID_Calc(&pid_fric_spd[1], moto_fric2->speed_rpm, 0);	
				CAN_CMD_SHOOT(Fric_Current[0], Fric_Current[1], current_shoot, 0);
				laser_off();
			}
      else if(fric_flag != 0)
			{
				//Ħ��������
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
						//����
						current_shoot = PID_Calc(&pid_shoot_speed, moto_trigger->speed_rpm, 1500);
						CAN_CMD_SHOOT(Fric_Current[0], Fric_Current[1], current_shoot, 0);
					}
					if(shoot_flag == 2)
					{
						//�ز�
						current_shoot = PID_Calc(&pid_shoot_speed, moto_trigger->speed_rpm, -1500);
						CAN_CMD_SHOOT(Fric_Current[0], Fric_Current[1], current_shoot, 0);
					}			
					if(shoot_flag == 0)
					{
						//ֹͣ����
						current_shoot = PID_Calc(&pid_shoot_speed, moto_trigger->speed_rpm, 0);
						CAN_CMD_SHOOT(Fric_Current[0], Fric_Current[1], current_shoot, 0);
					}
			//ϵͳ��ʱ
      vTaskDelay(1);
			
#if INCLUDE_uxTaskGetStackHighWaterMark
        Shoot_task_stack = uxTaskGetStackHighWaterMark(NULL);
#endif
    }
}

void shoot_behaviour(void)
{
    //F��Ħ��������
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
	  //�󲦸��²� ����
	  if( SHOOT_RC -> mouse.press_l==1 )
	  {
	    shoot_flag = 1 ;
	  }
		//��R�ز�����
		else if( SHOOT_RC -> key.v == KEY_PRESSED_OFFSET_R )
	  {
	    shoot_flag = 2 ;
	  }		
		else
		{
			shoot_flag = 0 ;
		}
}
