#ifndef SHOOT_TASK_H
#define SHOOT_TASK_H

#include "main.h"
#include "Remote_Control.h"
#include "CAN_Receive.h"
#include "pid.h"
#include "stm32f4xx.h"
#include "gimbal_behaviour_simple.h"

//�������Чʱ��
#define RC_KEY_MAX_TIME 2000

//SHOOT �ٶȻ�PID���� ������ �����޷�
#define SHOOT_SPEED_KP 23.0f
#define SHOOT_SPEED_KI 0.0f
#define SHOOT_SPEED_KD 0.0f
#define SHOOT_SPEED_MAX_OUT 10000.0f
#define SHOOT_SPEED_MAX_IOUT 1000.0f

//�����޷�
#define LIMIT 4000

//Ħ���ֵ���ٶȻ�PID����
#define FRIC_KP 20.0f
#define FRIC_KI 0.0f
#define FRIC_KD 0

//Ħ����M3508����͵���ֵ
#define MAX_FRIC_CAN_CURRENT 16000.0f

//���������
static uint8_t NONE = 0;

//Ħ����������ʶ
extern int fric_flag;
//�������CAN���͵���ֵ
extern int16_t current_shoot;
//������ʶ
extern int shoot_flag;

extern void ShootTask(void *pvParameters);

extern void shoot_behaviour(void);

extern void shoot_init(void);


#endif
