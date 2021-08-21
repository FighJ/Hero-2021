#include "User_Task.h"
#include "main.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "judge_analysis.h"
#include "laser.h"
#include "Remote_Control.h"
#include "imu901.h"
#include "CAN_Receive.h"

IMU_FEEDBACK_t imu_feedback;
cap_measure_t cap_feedback;

#define user_is_error() toe_is_error(errorListLength)

#if INCLUDE_uxTaskGetStackHighWaterMark
uint32_t UserTaskStack;
#endif

const RC_ctrl_t *USER_RC;
void UserTask(void *pvParameters)
{
		 //激光初始化
	  laser_configuration();
//		laser_on();
    while (1)
    {
			//板载绿色LED亮50ms
		  	led_green_on();
        vTaskDelay(50);
			//板载绿色LED灭50ms
        led_green_off();
        vTaskDelay(50);
			//向超级电容发送数据，设定现在的功率限制
				if(JudgementData.game_robot_state_t.chassis_power_limit!=0)
				{
					CAN_CMD_CAP(JudgementData.game_robot_state_t.chassis_power_limit*100);
				}
				else
				{
					CAN_CMD_CAP(4000);
				}
#if INCLUDE_uxTaskGetStackHighWaterMark
        UserTaskStack = uxTaskGetStackHighWaterMark(NULL);
#endif
    }
}

void Imu_feed_back(void)
{
	imu_feedback.last_angle_yaw = imu_feedback.angle_yaw;
	
	imu_feedback.angle_roll = attitude.pitch;
	imu_feedback.angle_pitch = attitude.roll;
	imu_feedback.angle_yaw = attitude.yaw;
	imu_feedback.speed_roll = gyroAccData.fgyroD[1];
	imu_feedback.speed_pitch = gyroAccData.fgyroD[2];
	imu_feedback.speed_yaw = gyroAccData.fgyroD[0];
	
		if(imu_feedback.angle_yaw - imu_feedback.last_angle_yaw > 180)
	{
		imu_feedback.angle_yaw_round_cnt --;
	}
		if(imu_feedback.angle_yaw - imu_feedback.last_angle_yaw < -180)
	{
		imu_feedback.angle_yaw_round_cnt ++;
	}
	imu_feedback.total_angle_yaw = imu_feedback.angle_yaw_round_cnt * 8191.f + (imu_feedback.angle_yaw + 180.f)*8191.f/360.f;
}

//void Imu_feed_back(void)
//{
//	imu_feedback.last_angle_yaw = imu_feedback.angle_yaw;
//	
//	imu_feedback.angle_roll = attitude.pitch;
//	imu_feedback.angle_pitch = attitude.roll;
//	imu_feedback.angle_yaw = attitude.yaw;
//	imu_feedback.speed_roll = gyroAccData.fgyroD[1];
//	imu_feedback.speed_pitch = gyroAccData.fgyroD[2];
//	imu_feedback.speed_yaw = gyroAccData.fgyroD[0];
//	
//		if(imu_feedback.angle_yaw - imu_feedback.last_angle_yaw > 180)
//	{
//		imu_feedback.angle_yaw_round_cnt --;
//	}
//		if(imu_feedback.angle_yaw - imu_feedback.last_angle_yaw < -180)
//	{
//		imu_feedback.angle_yaw_round_cnt ++;
//	}
//	imu_feedback.total_angle_yaw = imu_feedback.angle_yaw_round_cnt * 360 + imu_feedback.angle_yaw ;
//}
