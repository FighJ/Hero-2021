#include "chassis_behaviour_simple.h"
#include "judge_analysis.h"
#include "Chassis_Task_Simple.h"
#include "user_lib.h"
#include "User_Task.h"
#include "Remote_Control.h"

//初始化速度 speed_x=前进后退 speed_y=左转右转 speed_turn= 左平移右平移
int16_t speed_x=0,speed_y=0,speed_turn=0;
//速度增加速度
int16_t SPEED_KEY=2;
int16_t SPEED_Y_MAX=2500;
int16_t SPEED_X_MAX=1600;
int16_t  TURN_MAX=2500;
//右前方麦轮电调ID设置为1，逆时针依次为2、3、4
//初始化电流初值
int16_t current[4]={0,0,0,0};
int16_t current_temp[4]={0,0,0,0};
//获取遥控器信息
const RC_ctrl_t *CHASSIS_RC;
//用于计算电机编码值对应弧度
fp32 CHASSIS_INS_YAW = 0;
//获取底盘电机编码值
const motor_measure_t *moto_chassis[4];
//获得超级电容数据
const cap_measure_t *cap;
//底盘电机PID参数定义
static const fp32 PID_WHEEL_SPEED[] = {WHEEL_SPEED_KP, WHEEL_SPEED_KI, WHEEL_SPEED_KD};
//底盘麦轮速度
float speed[4];
//模式赋值变量
uint8_t MODE_CHASSIS;
//底盘电机PID结构体
PidTypeDef pid_wheel_spd[4];

void chassis_init(void)
{
	for(int k=0; k<4; k++)
	{		
		//PID算法，确保四个轮子的速度一致
		PID_Init(&pid_wheel_spd[k], PID_POSITION, PID_WHEEL_SPEED, WHEEL_SPEED_MAX_OUT, WHEEL_SPEED_MAX_IOUT);	
	}	
}

//不同模式下的云台行为
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
	  
		
		//一阶低通滤波代替斜波作为底盘速度输入
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
