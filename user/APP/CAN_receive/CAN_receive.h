/**
  ****************************(C) COPYRIGHT 2016 DJI****************************
  * @file       can_receive.c/h
  * @brief      ���can�豸�����շ����������ļ���ͨ��can�ж���ɽ���
  * @note       ���ļ�����freeRTOS����
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Dec-26-2018     RM              1. ���
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2016 DJI****************************
  */

#ifndef CANTASK_H
#define CANTASK_H
#include "main.h"

#define CHASSIS_CAN CAN1
#define GIMBAL_CAN CAN2
#define USER_CAN CAN2

/* CAN send and receive ID */
typedef enum
{
    CAN_CHASSIS_ALL_ID = 0x200,
    CAN_3508_M1_ID = 0x201,
    CAN_3508_M2_ID = 0x202,
    CAN_3508_M3_ID = 0x203,
    CAN_3508_M4_ID = 0x204,
	
    CAN_USER_ALL_ID = 0x1FF,
		CAN_YAW_REEL_ID = 0x205,
	  CAN_YAW_SHELL_ID = 0x206,

    CAN_SHOOT_ALL_ID = 0x200,
	  CAN_FRIC1_ID = 0x201,
	  CAN_FRIC2_ID = 0x202,
    CAN_TRIGGER_MOTOR_ID = 0x203,

    CAN_GIMBAL_ALL_ID = 0x1FF,
    CAN_YAW_MOTOR_ID = 0x205,
    CAN_PIT1_MOTOR_ID = 0x206,
  	CAN_PIT2_MOTOR_ID = 0x207,

		CAN_CAP_SET_ID = 0x210,
	  CAN_CAP_FEEDBACK_ID = 0x211,
} can_msg_id_e;

//RM���ͳһ���ݽṹ��
typedef struct
{
    uint16_t ecd;
    int16_t speed_rpm;
    int16_t given_current;
    uint8_t temperate;
    int16_t last_ecd;
	
	  //����Ϊ����ԭʼ���ݵ�������
		uint16_t	offset_ecd;
		int32_t		round_cnt;  //���ۿɼ�¼524352Ȧ
		int32_t		total_ecd;  //���ۿɼ�¼4294967295����ֵ
		
		int32_t		total_ecd_out_2006;  //���ۿɼ�¼14565Ȧ
		int32_t		total_ecd_out_3508;  //���ۿɼ�¼27305Ȧ
		int32_t		total_ecd_out_6020;  //���ۿɼ�¼524352Ȧ
	
} motor_measure_t;

//rm�����������ݽṹ��
typedef struct
{
    float Vin;
    float Vcap;
    float Iin;
    float Pset;
} cap_measure_t;

extern void CAN_CMD_CHASSIS_RESET_ID(void);

//������̨�����������revΪ�����ֽ�
extern void CAN_CMD_GIMBAL(int16_t yaw, int16_t pitch, int16_t shoot, int16_t rev);
//����Ħ���ֵ����������
extern void CAN_CMD_SHOOT(int16_t User_Motor1, int16_t User_Motor2, int16_t User_Motor3, int16_t User_Motor4);
//���͵��̵����������
extern void CAN_CMD_CHASSIS(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4);
//����2006�����������
extern void CAN_CMD_2006(int16_t User_Motor1, int16_t User_Motor2, int16_t User_Motor3, int16_t User_Motor4);
//���ͳ������ݿ�������
extern void CAN_CMD_CAP(int16_t temPower);

//����yaw���������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
extern const motor_measure_t *get_Yaw_Gimbal_Motor_Measure_Point(void);
//����pitch1���������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
extern const motor_measure_t *get_Pitch1_Gimbal_Motor_Measure_Point(void);
//����pitch2���������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
extern const motor_measure_t *get_Pitch2_Gimbal_Motor_Measure_Point(void);
//����trigger���������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
extern const motor_measure_t *get_Trigger_Motor_Measure_Point(void);
//����reel���������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
extern const motor_measure_t *get_Reel_Motor_Measure_Point(void);
//����shell���������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
extern const motor_measure_t *get_Shell_Motor_Measure_Point(void);
//����Fric1���������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
extern const motor_measure_t *get_Fric1_Motor_Measure_Point(void);
//����Fric2���������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
extern const motor_measure_t *get_Fric2_Motor_Measure_Point(void);
//���س������ݱ�����ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
extern const cap_measure_t *get_Supercap_Measure_Point(void);
//���ص��̵��������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����,i�ķ�Χ��0-3����Ӧ0x201-0x204,
extern const motor_measure_t *get_Chassis_Motor_Measure_Point(uint8_t i);
//���������󽮵��������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����,i�ķ�Χ��0-3����Ӧ0x201-0x204,
extern const motor_measure_t *get_User_Motor_Measure_Point(uint8_t i);

//�����������
extern motor_measure_t motor_yaw, motor_pit, motor_trigger, motor_reel, motor_shell, motor_user[4], motor_chassis[4], motor_fric1, motor_fric2;
//�����������ݱ���
extern cap_measure_t supercap;

#if GIMBAL_MOTOR_6020_CAN_LOSE_SLOVE
extern void GIMBAL_lose_slove(void);
#endif

#endif
