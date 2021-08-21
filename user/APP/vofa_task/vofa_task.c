#include "Vofa_Task.h"
#include "main.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "uart_vofa.h"
#include "math.h"
#include "imu901.h"
#include "vofa.h"
#include "CAN_Receive.h"
#include "user_task.h"
#include "chassis_behaviour_simple.h"
#include "INS_Task.h"
#include "mpu6500driver.h"
#include "gimbal_behaviour_simple.h"
#define user_is_error() toe_is_error(errorListLength)

#if INCLUDE_uxTaskGetStackHighWaterMark
uint32_t Vofa_TaskStack;
#endif

int strlength(char *str,int max)
{
  int temp=0;
  for(int i=0;i<max;i++)
  {
    if(str[i]!='\0')
      temp=i+1;
  }
  return temp;
}

float send_message[channel];

void Vofa_Task(void *pvParameters)
{
  Vofa_tail_init();
	
	while(1)
	{
		message_send_buf();
		
		Vofa_send_justfloat(send_message);
		
		Dma_Send_Enable(DMA1_Stream1,sizeof(Vofa.message));
		
		vTaskDelay(5);
		
			
#if INCLUDE_uxTaskGetStackHighWaterMark
        Vofa_TaskStack = uxTaskGetStackHighWaterMark(NULL);
#endif
    }
}

void message_send_buf(void)
{
	send_message[0] = moto_pitch1 -> ecd;
	send_message[1] = moto_yaw -> ecd;
	send_message[2] = moto_pitch1 -> given_current;
	send_message[3] = moto_yaw -> given_current;
	send_message[4] = attitude.yaw;
	send_message[5] = imu_feedback.total_angle_yaw;
	send_message[6] = YAW_AUTO_OUT+YAW_AUTO_Zero;
}
