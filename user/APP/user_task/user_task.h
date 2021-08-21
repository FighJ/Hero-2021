#ifndef USER_TASK_H
#define USER_TASK_H

typedef struct
{
	float angle_yaw_round_cnt;
	float last_angle_yaw;
	float total_angle_yaw;
	
  float angle_pitch;
	float angle_yaw;
	float angle_roll;
	
	float speed_pitch;
	float speed_yaw;
	float speed_roll;

} IMU_FEEDBACK_t;


extern IMU_FEEDBACK_t imu_feedback;

extern void UserTask(void *pvParameters);

void Imu_feed_back(void);
#endif
