#include "Chassis_Task_Simple.h"
#include "judge_analysis.h"
#include "dac.h"
#include "user_lib.h"

first_order_filter_type_t chassis_cmd_slow_set_vx;
first_order_filter_type_t chassis_cmd_slow_set_vy;

const static fp32 chassis_x_order_filter[1] = {CHASSIS_ACCEL_X_NUM};
		
const static fp32 chassis_y_order_filter[1] = {CHASSIS_ACCEL_Y_NUM};

void chassis_task_Simple(void *pvParameters)
{
	//底盘PID初始化
	chassis_init();
	//用一阶滤波代替斜波函数生成
  first_order_filter_init(&chassis_cmd_slow_set_vx, CHASSIS_CONTROL_TIME, chassis_x_order_filter);
  first_order_filter_init(&chassis_cmd_slow_set_vy, CHASSIS_CONTROL_TIME, chassis_y_order_filter);
    while (1)
    {
			//获取遥控器指针变量
			CHASSIS_RC = get_remote_control_point();
			if(CHASSIS_RC -> rc.s[0] == RC_SW_UP)
			{
				//底盘跟随云台模式
				MODE_CHASSIS = CHASSIS_RC_AUTO;
			}
			if(CHASSIS_RC -> rc.s[0] == RC_SW_MID)
			{
				//底盘无力模式
				MODE_CHASSIS = CHASSIS_ZERO_FORCE;
			}
			if(CHASSIS_RC -> rc.s[0] == RC_SW_DOWN)
			{
				//底盘与云台独立控制模式
				MODE_CHASSIS = CHASSIS_RC_CONTROL;
			}

			chassis_mode_behaviour();
			
			//系统延时
      vTaskDelay(1);
    }
}
