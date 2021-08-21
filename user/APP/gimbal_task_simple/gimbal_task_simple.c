#include "Gimbal_Task_Simple.h"
#include "gimbal_behaviour_simple.h"
#include "imu901.h"
#include "User_Task.h"

#if INCLUDE_uxTaskGetStackHighWaterMark
uint32_t gimbal_task_simple_stack;
#endif

void GIMBAL_task_Simple(void *pvParameters)
{
	  //ģʽ��ʼ��
	  MODE = GIMBAL_ZERO_FORCE;
		Imu_feed_back();
	  YAW_AUTO_Zero = imu_feedback.total_angle_yaw;
    while (1)
    {
			//��ȡң����ָ�����
			GIMBAL_RC = get_remote_control_point();
			LAST_GIMBAL_RC = GIMBAL_RC;
			
			//��̨PID��ʼ��
			gimbal_init();
			if(GIMBAL_RC -> rc.s[1] == RC_SW_UP)
			{
				MODE = GIMBAL_RC_AUTO;
			}
			if(GIMBAL_RC -> rc.s[1] == RC_SW_MID)
			{
				MODE = GIMBAL_ZERO_FORCE;
			}
			if(GIMBAL_RC -> rc.s[1] == RC_SW_DOWN)
			{
				MODE = GIMBAL_RC_CONTROL;
			}

			gimbal_mode_behaviour();
			
			//ϵͳ��ʱ
      vTaskDelay(1);
			
#if INCLUDE_uxTaskGetStackHighWaterMark
        gimbal_task_simple_stack = uxTaskGetStackHighWaterMark(NULL);
#endif
    }
}
