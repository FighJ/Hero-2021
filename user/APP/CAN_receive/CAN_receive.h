/**
  ****************************(C) COPYRIGHT 2016 DJI****************************
  * @file       can_receive.c/h
  * @brief      完成can设备数据收发函数，该文件是通过can中断完成接收
  * @note       该文件不是freeRTOS任务
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Dec-26-2018     RM              1. 完成
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

//RM电机统一数据结构体
typedef struct
{
    uint16_t ecd;
    int16_t speed_rpm;
    int16_t given_current;
    uint8_t temperate;
    int16_t last_ecd;
	
	  //以下为根据原始数据的推算量
		uint16_t	offset_ecd;
		int32_t		round_cnt;  //理论可记录524352圈
		int32_t		total_ecd;  //理论可记录4294967295编码值
		
		int32_t		total_ecd_out_2006;  //理论可记录14565圈
		int32_t		total_ecd_out_3508;  //理论可记录27305圈
		int32_t		total_ecd_out_6020;  //理论可记录524352圈
	
} motor_measure_t;

//rm超级电容数据结构体
typedef struct
{
    float Vin;
    float Vcap;
    float Iin;
    float Pset;
} cap_measure_t;

extern void CAN_CMD_CHASSIS_RESET_ID(void);

//发送云台控制命令，其中rev为保留字节
extern void CAN_CMD_GIMBAL(int16_t yaw, int16_t pitch, int16_t shoot, int16_t rev);
//发送摩擦轮电机控制命令
extern void CAN_CMD_SHOOT(int16_t User_Motor1, int16_t User_Motor2, int16_t User_Motor3, int16_t User_Motor4);
//发送底盘电机控制命令
extern void CAN_CMD_CHASSIS(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4);
//发送2006电机控制命令
extern void CAN_CMD_2006(int16_t User_Motor1, int16_t User_Motor2, int16_t User_Motor3, int16_t User_Motor4);
//发送超级电容控制命令
extern void CAN_CMD_CAP(int16_t temPower);

//返回yaw电机变量地址，通过指针方式获取原始数据
extern const motor_measure_t *get_Yaw_Gimbal_Motor_Measure_Point(void);
//返回pitch1电机变量地址，通过指针方式获取原始数据
extern const motor_measure_t *get_Pitch1_Gimbal_Motor_Measure_Point(void);
//返回pitch2电机变量地址，通过指针方式获取原始数据
extern const motor_measure_t *get_Pitch2_Gimbal_Motor_Measure_Point(void);
//返回trigger电机变量地址，通过指针方式获取原始数据
extern const motor_measure_t *get_Trigger_Motor_Measure_Point(void);
//返回reel电机变量地址，通过指针方式获取原始数据
extern const motor_measure_t *get_Reel_Motor_Measure_Point(void);
//返回shell电机变量地址，通过指针方式获取原始数据
extern const motor_measure_t *get_Shell_Motor_Measure_Point(void);
//返回Fric1电机变量地址，通过指针方式获取原始数据
extern const motor_measure_t *get_Fric1_Motor_Measure_Point(void);
//返回Fric2电机变量地址，通过指针方式获取原始数据
extern const motor_measure_t *get_Fric2_Motor_Measure_Point(void);
//返回超级电容变量地址，通过指针方式获取原始数据
extern const cap_measure_t *get_Supercap_Measure_Point(void);
//返回底盘电机变量地址，通过指针方式获取原始数据,i的范围是0-3，对应0x201-0x204,
extern const motor_measure_t *get_Chassis_Motor_Measure_Point(uint8_t i);
//返回其他大疆电机变量地址，通过指针方式获取原始数据,i的范围是0-3，对应0x201-0x204,
extern const motor_measure_t *get_User_Motor_Measure_Point(uint8_t i);

//声明电机变量
extern motor_measure_t motor_yaw, motor_pit, motor_trigger, motor_reel, motor_shell, motor_user[4], motor_chassis[4], motor_fric1, motor_fric2;
//声明超级电容变量
extern cap_measure_t supercap;

#if GIMBAL_MOTOR_6020_CAN_LOSE_SLOVE
extern void GIMBAL_lose_slove(void);
#endif

#endif
