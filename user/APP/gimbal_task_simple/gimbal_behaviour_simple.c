#include "gimbal_behaviour_simple.h"
#include "User_Task.h"
#include "imu901.h"
#include "vofa.h"
#include "Gimbal_Task_Simple.h"

float angle_pitch1 = 0;
float angle_pitch1_remain = 0;
float angle_pitch2 = 0;
float angle_pitch2_remain = 0;
float angle_yaw = 0;
float angle_yaw_auto = 0;
float angle_yaw_remain = 0;
float angle_yaw_init = 0;
//��Ϊ״̬���
float PITCH1_OUT, PITCH2_OUT, YAW_OUT, YAW_AUTO_OUT = 0;
//��ȡ��̨����ֵ��Ϣ
const motor_measure_t *moto_pitch1;
const motor_measure_t *moto_pitch2;
const motor_measure_t *moto_yaw;
//��ȡң������Ϣ
const RC_ctrl_t *GIMBAL_RC;
const RC_ctrl_t *LAST_GIMBAL_RC;
//��̨���PID��������
static const fp32 PID_PITCH_SPEED[] = {PITCH_SPEED_KP, PITCH_SPEED_KI, PITCH_SPEED_KD};
static const fp32 PID_YAW_SPEED[] = {YAW_SPEED_KP, YAW_SPEED_KI, YAW_SPEED_KD};
static const fp32 PID_PITCH_ANGLE[] = {PITCH_ANGLE_KP, PITCH_ANGLE_KI, PITCH_ANGLE_KD};
static const fp32 PID_YAW_ANGLE[] = {YAW_ANGLE_KP, YAW_ANGLE_KI, YAW_ANGLE_KD};
static const fp32 PID_YAW_FOLLOW_SPEED[] = {YAW_FOLLOW_SPEED_KP, YAW_FOLLOW_SPEED_KI, YAW_FOLLOW_SPEED_KD};
static const fp32 PID_YAW_FOLLOW_ANGLE[] = {YAW_FOLLOW_ANGLE_KP, YAW_FOLLOW_ANGLE_KI, YAW_FOLLOW_ANGLE_KD};
//��ʼ��������ֵ
int16_t current_pitch1 = 0;
int16_t current_pitch2 = 0;
int16_t current_yaw = 0;
//��̨У׼��ʼ��
int16_t Check_Time = 0;
//���ݳ�ʼֵ
float YAW_AUTO_Zero = 0;
//ģʽ��ֵ����
uint8_t MODE;
uint8_t LAST_MODE;
uint8_t LAST_MODE_TEMP;
uint8_t CHECK_MODE;
uint8_t LAST_CHECK_MODE;
uint8_t LAST_CHECK_MODE_TEMP;
//��̨���PID�ṹ��
PidTypeDef pid_pitch_speed;
PidTypeDef pid_yaw_speed;
PidTypeDef pid_pitch_angle;
PidTypeDef pid_yaw_angle;
PidTypeDef pid_yaw_follow_speed;
PidTypeDef pid_yaw_follow_angle;
void gimbal_init(void)
{
	//PID �ٶȻ� �ǶȻ�
  PID_Init(&pid_pitch_speed, PID_POSITION, PID_PITCH_SPEED, PITCH_SPEED_MAX_OUT, PITCH_SPEED_MAX_IOUT);
  PID_Init(&pid_yaw_speed, PID_POSITION, PID_YAW_SPEED, YAW_SPEED_MAX_OUT, YAW_SPEED_MAX_IOUT);
	PID_Init(&pid_pitch_angle, PID_POSITION, PID_PITCH_ANGLE, PITCH_ANGLE_MAX_OUT, PITCH_ANGLE_MAX_IOUT);
  PID_Init(&pid_yaw_angle, PID_POSITION, PID_YAW_ANGLE, YAW_ANGLE_MAX_OUT, YAW_ANGLE_MAX_IOUT);
	PID_Init(&pid_yaw_follow_speed, PID_POSITION, PID_YAW_FOLLOW_SPEED, YAW_FOLLOW_SPEED_MAX_OUT, YAW_FOLLOW_SPEED_MAX_IOUT);
	PID_Init(&pid_yaw_follow_angle, PID_POSITION, PID_YAW_FOLLOW_ANGLE, YAW_FOLLOW_ANGLE_MAX_OUT, YAW_FOLLOW_ANGLE_MAX_IOUT);
}

//�������ֵ���� 0��8191
#define ECD_Format(ecd)         \
    {                           \
        if ((ecd) > ecd_range)  \
            (ecd) -= ecd_range; \
        else if ((ecd) < 0)     \
            (ecd) += ecd_range; \
    }
		
void CAN_GIMBAL_RC(int16_t PITCH, int16_t YAW)
{	
	moto_pitch1 = get_Pitch1_Gimbal_Motor_Measure_Point();
	moto_yaw = get_Yaw_Gimbal_Motor_Measure_Point();

	angle_pitch1 = PITCH + PITCH_Zero;
	angle_yaw = YAW + YAW_Zero;
	
	angle_pitch1_remain = PID_Calc(&pid_pitch_angle, moto_pitch1->ecd, angle_pitch1);
	current_pitch1 = PID_Calc(&pid_pitch_speed, moto_pitch1->speed_rpm, angle_pitch1_remain);
	
	angle_pitch2_remain = PID_Calc(&pid_pitch_angle, moto_pitch1->ecd, angle_pitch1);
	current_pitch2 = -PID_Calc(&pid_pitch_speed, moto_pitch1->speed_rpm, angle_pitch1_remain);
	
	angle_yaw_remain = PID_Calc(&pid_yaw_angle, moto_yaw->ecd, angle_yaw);
	current_yaw = PID_Calc(&pid_yaw_speed, moto_yaw->speed_rpm, angle_yaw_remain);
	
	CAN_CMD_GIMBAL(current_yaw, current_pitch1, current_pitch2, 0);
}

void CAN_GIMBAL_AUTO(int16_t PITCH, int16_t YAW)
{	
	moto_pitch1 = get_Pitch1_Gimbal_Motor_Measure_Point();
	moto_yaw = get_Yaw_Gimbal_Motor_Measure_Point();

	angle_pitch1 = PITCH + PITCH_Zero;
	angle_yaw = YAW + YAW_AUTO_Zero;
	
	angle_pitch1_remain = PID_Calc(&pid_pitch_angle, moto_pitch1->ecd, angle_pitch1);
	current_pitch1 = PID_Calc(&pid_pitch_speed, moto_pitch1->speed_rpm, angle_pitch1_remain);
	
	angle_pitch2_remain = PID_Calc(&pid_pitch_angle, moto_pitch1->ecd, angle_pitch1);
	current_pitch2 = -PID_Calc(&pid_pitch_speed, moto_pitch1->speed_rpm, angle_pitch1_remain);
	
//	Imu_feed_back();
//	angle_yaw_remain = PID_Calc(&pid_yaw_follow_angle, imu_feedback.total_angle_yaw, angle_yaw);
//	current_yaw = PID_Calc(&pid_yaw_follow_speed, moto_yaw->speed_rpm, angle_yaw_remain);
	
	angle_yaw_remain = PID_Calc(&pid_yaw_angle, moto_yaw->ecd, 5500);
	current_yaw = PID_Calc(&pid_yaw_speed, moto_yaw->speed_rpm, angle_yaw_remain);
	
	CAN_CMD_GIMBAL(current_yaw, current_pitch1, current_pitch2, 0);
}
//��ͬģʽ�µ���̨��Ϊ
void gimbal_mode_behaviour(void)
{ 
		if (MODE == GIMBAL_ZERO_FORCE)
	  {
			Imu_feed_back();
			YAW_AUTO_Zero=imu_feedback.total_angle_yaw;
		  CAN_CMD_GIMBAL(0, 0, 0, 0);
	  }
		
	  else if(MODE == GIMBAL_RC_CONTROL)
	  {
		  if( fabs( GIMBAL_RC -> rc.ch[3] ) > 30  )
		  {
			  if( ( GIMBAL_RC -> rc.ch[3] ) < 0 )
			  {
				  PITCH1_OUT = PITCH1_OUT - pitch_rc_speed;
			  }
			  else
			  {
				  PITCH1_OUT = PITCH1_OUT + pitch_rc_speed;
			  }
		  }
			else
			{
				PITCH1_OUT = PITCH1_OUT;
			}
			if(PITCH1_OUT<0)
				PITCH1_OUT=0;
		  if( fabs( GIMBAL_RC -> rc.ch[4] ) > 30  )
		  {
			  if( ( GIMBAL_RC -> rc.ch[4] ) > 0 )
			  {
				  YAW_OUT = YAW_OUT + yaw_rc_speed;
			  }
			  else if(  GIMBAL_RC -> rc.ch[4]  < 0  )
			  {
				  YAW_OUT = YAW_OUT - yaw_rc_speed;
			  }
		  }
			else
			{
				YAW_OUT = YAW_OUT;
			}
			if(YAW_OUT>1000)
				YAW_OUT=1000;
			if(YAW_OUT<-1000)
				YAW_OUT=-1000;
	    CAN_GIMBAL_RC(PITCH1_OUT, YAW_OUT);
			Imu_feed_back();
			YAW_AUTO_Zero=imu_feedback.total_angle_yaw;
	  }
		
		else if(MODE == GIMBAL_RC_AUTO)
		{
				PITCH1_OUT = PITCH1_OUT - GIMBAL_RC -> mouse.y/20.f;
//			if(PITCH1_OUT > 600)
//				PITCH1_OUT = 0;
			if(PITCH1_OUT < 0)
				PITCH1_OUT = 0;
			CAN_GIMBAL_AUTO(PITCH1_OUT, 0);
		}
	}
