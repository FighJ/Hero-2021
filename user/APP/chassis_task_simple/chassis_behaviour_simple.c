#include "chassis_behaviour_simple.h"
#include "judge_analysis.h"
#include "Chassis_Task_Simple.h"
#include "user_lib.h"
#include "User_Task.h"
#include "Remote_Control.h"

//��ʼ���ٶ� speed_x=ǰ������ speed_y=��ת��ת speed_turn= ��ƽ����ƽ��
int16_t speed_x=0,speed_y=0,speed_turn=0;
//�ٶ������ٶ�
int16_t SPEED_KEY=2;
int16_t SPEED_Y_MAX=2500;
int16_t SPEED_X_MAX=1600;
int16_t  TURN_MAX=2500;
//��ǰ�����ֵ��ID����Ϊ1����ʱ������Ϊ2��3��4
//��ʼ��������ֵ
int16_t current[4]={0,0,0,0};
int16_t current_temp[4]={0,0,0,0};
//��ȡң������Ϣ
const RC_ctrl_t *CHASSIS_RC;
//���ڼ���������ֵ��Ӧ����
fp32 CHASSIS_INS_YAW = 0;
//��ȡ���̵������ֵ
const motor_measure_t *moto_chassis[4];
//��ó�����������
const cap_measure_t *cap;
//���̵��PID��������
static const fp32 PID_WHEEL_SPEED[] = {WHEEL_SPEED_KP, WHEEL_SPEED_KI, WHEEL_SPEED_KD};
//���������ٶ�
float speed[4];
//ģʽ��ֵ����
uint8_t MODE_CHASSIS;
//���̵��PID�ṹ��
PidTypeDef pid_wheel_spd[4];

void chassis_init(void)
{
	for(int k=0; k<4; k++)
	{		
		//PID�㷨��ȷ���ĸ����ӵ��ٶ�һ��
		PID_Init(&pid_wheel_spd[k], PID_POSITION, PID_WHEEL_SPEED, WHEEL_SPEED_MAX_OUT, WHEEL_SPEED_MAX_IOUT);	
	}	
}

//��ͬģʽ�µ���̨��Ϊ
void chassis_mode_behaviour(void)
{
	for(int count = 0; count < 4; count++)
	{
		moto_chassis[count] = get_User_Motor_Measure_Point(count);
	}
	cap = get_Supercap_Measure_Point();
	if (MODE_CHASSIS == CHASSIS_ZERO_FORCE)
	{
			for(int i=0;i<4;i++)
		{
			current[i]=0;
		}
	}
	else if(MODE_CHASSIS == CHASSIS_RC_CONTROL)
	{
		speed_x = -CHASSIS_RC -> rc.ch[0] * RC_SPEED;
		speed_y =  -CHASSIS_RC -> rc.ch[1] * RC_SPEED;
		speed_turn = CHASSIS_RC -> rc.ch[2] * RC_SPEED;
		
		speed[0] = -speed_x - speed_y + speed_turn;
		speed[1] = speed_x - speed_y + speed_turn;
		speed[2] = speed_x + speed_y + speed_turn;
		speed[3] = -speed_x + speed_y + speed_turn;
		
		current[0] = PID_Calc(&pid_wheel_spd[0], moto_chassis[0]->speed_rpm, speed[0]);
		current[1] = PID_Calc(&pid_wheel_spd[1], moto_chassis[1]->speed_rpm, speed[1]);
		current[2] = PID_Calc(&pid_wheel_spd[2], moto_chassis[2]->speed_rpm, speed[2]);
		current[3] = PID_Calc(&pid_wheel_spd[3], moto_chassis[3]->speed_rpm, speed[3]);
	}
	else if(MODE_CHASSIS == CHASSIS_RC_AUTO)
	{		
		if( rc_ctrl.key.v == KEY_PRESSED_OFFSET_W)
		  speed_y=-SPEED_Y_MAX;
		if( rc_ctrl.key.v == KEY_PRESSED_OFFSET_W + KEY_PRESSED_OFFSET_SHIFT)
			speed_y=-5000;
		if( rc_ctrl.key.v == KEY_PRESSED_OFFSET_W + KEY_PRESSED_OFFSET_CTRL)
			speed_y=-1800;
		if( rc_ctrl.key.v == KEY_PRESSED_OFFSET_A)
			speed_x=SPEED_X_MAX;
		if( rc_ctrl.key.v == KEY_PRESSED_OFFSET_S)
			speed_y=SPEED_Y_MAX;
		if( rc_ctrl.key.v == KEY_PRESSED_OFFSET_D)
			speed_x=-SPEED_X_MAX;
		
		if( rc_ctrl.key.v == KEY_PRESSED_OFFSET_W + KEY_PRESSED_OFFSET_A)
		{
			speed_y=-SPEED_Y_MAX;speed_x=SPEED_X_MAX;
		}
		if( rc_ctrl.key.v == 9)
		{
			speed_y=-SPEED_Y_MAX;speed_x=-SPEED_X_MAX;
		}
		if( rc_ctrl.key.v == 6)
		{
			speed_y=SPEED_Y_MAX;speed_x=SPEED_X_MAX;
		}
		if( rc_ctrl.key.v == 10)
		{
			speed_y=SPEED_Y_MAX;speed_x=-SPEED_X_MAX;
		}
		if( rc_ctrl.key.v == KEY_PRESSED_OFFSET_Q)
			speed_turn = -TURN_MAX;
		if( rc_ctrl.key.v == KEY_PRESSED_OFFSET_E)
			speed_turn = TURN_MAX;
		if( rc_ctrl.key.v == 0)
		{
			speed_x = 0;
			speed_y = 0;
			speed_turn = 0;
		}
		if(rc_ctrl.mouse.x != 0)
			speed_turn = rc_ctrl.mouse.x * 12;
	  
		
		//һ�׵�ͨ�˲�����б����Ϊ�����ٶ�����
    first_order_filter_cali(&chassis_cmd_slow_set_vx, speed_x);
    first_order_filter_cali(&chassis_cmd_slow_set_vy, speed_y);
		
		speed[0] = -chassis_cmd_slow_set_vx.out - chassis_cmd_slow_set_vy.out + speed_turn;
		speed[1] = chassis_cmd_slow_set_vx.out - chassis_cmd_slow_set_vy.out + speed_turn;
		speed[2] = chassis_cmd_slow_set_vx.out + chassis_cmd_slow_set_vy.out + speed_turn;
		speed[3] = -chassis_cmd_slow_set_vx.out + chassis_cmd_slow_set_vy.out + speed_turn;
		
		current[0] = PID_Calc(&pid_wheel_spd[0], moto_chassis[0]->speed_rpm, speed[0]);
		current[1] = PID_Calc(&pid_wheel_spd[1], moto_chassis[1]->speed_rpm, speed[1]);
		current[2] = PID_Calc(&pid_wheel_spd[2], moto_chassis[2]->speed_rpm, speed[2]);
		current[3] = PID_Calc(&pid_wheel_spd[3], moto_chassis[3]->speed_rpm, speed[3]);
	 }
	CAN_CMD_CHASSIS(current[0], current[1], current[2], current[3]);
}
