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

//PITCH �ٶȻ�PID���� ������ �����޷�
#define PITCH_SPEED_KP 1.00f
#define PITCH_SPEED_KI 0.00f
#define PITCH_SPEED_KD 0.0f
#define PITCH_SPEED_MAX_OUT 20000.0f
#define PITCH_SPEED_MAX_IOUT 5000.0f

//PITCH �ǶȻ�PID���� ������ �����޷�
#define PITCH_ANGLE_KP 100.0f
#define PITCH_ANGLE_KI 0.0f
#define PITCH_ANGLE_KD 0.5f
#define PITCH_ANGLE_MAX_OUT 20000.0f
#define PITCH_ANGLE_MAX_IOUT 5000.0f

//YAW �ٶȻ�PID���� ������ �����޷�
#define YAW_SPEED_KP 1.00f
#define YAW_SPEED_KI 0.00f
#define YAW_SPEED_KD 0.0f
#define YAW_SPEED_MAX_OUT 20000.0f
#define YAW_SPEED_MAX_IOUT 4000.0f

//YAW �ǶȻ�PID���� ������ �����޷�
#define YAW_ANGLE_KP 200.00f
#define YAW_ANGLE_KI 0.0f
#define YAW_ANGLE_KD 0.20f
#define YAW_ANGLE_MAX_OUT 20000.0f
#define YAW_ANGLE_MAX_IOUT 4000.0f

//YAW �ٶȻ�PID���� ������ �����޷�
#define YAW_FOLLOW_SPEED_KP 1.0f
#define YAW_FOLLOW_SPEED_KI 0.0f
#define YAW_FOLLOW_SPEED_KD 0.0f
#define YAW_FOLLOW_SPEED_MAX_OUT 20000.0f
#define YAW_FOLLOW_SPEED_MAX_IOUT 4000.0f

//YAW �ǶȻ�PID���� ������ �����޷�
#define YAW_FOLLOW_ANGLE_KP 50.0f
#define YAW_FOLLOW_ANGLE_KI 0.0f
#define YAW_FOLLOW_ANGLE_KD 0.20f
#define YAW_FOLLOW_ANGLE_MAX_OUT 20000.0f
#define YAW_FOLLOW_ANGLE_MAX_IOUT 4000.0f

//��̨�ƶ��ٶ�
#define pitch_rc_speed 1.0f
#define yaw_rc_speed 1.5f
#define yaw_auto_speed 0.01f

//�������ֵ����Լ���ֵ
#define Half_ecd_range 4096
#define ecd_range 8191
//����ǶȾ������
#define PITCH_Zero 4700
#define YAW_Zero  5500

//��̨У׼ʱ��
#define Check_Max_Time 1000

typedef enum
{
  GIMBAL_ZERO_FORCE = 0, //��̨����
  GIMBAL_RC_CONTROL,  //ң������PC����
	GIMBAL_RC_AUTO,
} Gimbal_Mode;

typedef enum
{
  UP = 0,
  MID,  
	DOWN,
} Gimbal_check_Mode;//�󲦸���̨ģʽ

extern const RC_ctrl_t *GIMBAL_RC;
extern const RC_ctrl_t *LAST_GIMBAL_RC;
//��̨���CAN���͵���ֵ
extern int16_t current_pitch1;
extern int16_t current_yaw;

//��̨У׼��ʼ��
extern int16_t Check_Time;

extern const motor_measure_t *moto_yaw;
extern const motor_measure_t *moto_pitch1;
extern const motor_measure_t *moto_pitch2;
extern float YAW_AUTO_Zero;
extern float PITCH1_OUT, YAW_AUTO_OUT, YAW_OUT, YAW_AUTO_Zero;

//��̨PID��ʼ��
extern void gimbal_init(void);
//��ͬģʽ����̨��Ϊ
extern void gimbal_mode_behaviour(void);
//����󲦸���̨״̬
//extern void gimbal_mode_check(void);
//extern void mode_change(void);
//��̨�����������
extern void CAN_CMD_GIMBAL_SIMPLE(int16_t PITCH, int16_t YAW);
	
#endif
