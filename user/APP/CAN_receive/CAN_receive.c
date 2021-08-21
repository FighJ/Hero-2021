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

#include "CAN_Receive.h"
#include "stm32f4xx.h"


//���̵�����ݶ�ȡ
#define get_motor_measure(ptr, rx_message)                                                                   \
    {                                                                                                        \
        (ptr)->last_ecd = (ptr)->ecd;                                                                        \
        (ptr)->ecd = (uint16_t)((rx_message)->Data[0] << 8 | (rx_message)->Data[1]);                         \
        (ptr)->speed_rpm = (uint16_t)((rx_message)->Data[2] << 8 | (rx_message)->Data[3]);                   \
        (ptr)->given_current = (uint16_t)((rx_message)->Data[4] << 8 | (rx_message)->Data[5]);               \
        (ptr)->temperate = (rx_message)->Data[6];                                                            \
			                                                                                                       \
				/*����Ϊ����ԭʼ���ݵ�����*/																													                 \
																																														                 \
				if((ptr)->ecd - (ptr)->last_ecd > 4096)                                                              \
					(ptr)->round_cnt --;                                                                               \
				else if ((ptr)->ecd - (ptr)->last_ecd < -4096)                                                       \
					(ptr)->round_cnt ++;                                                                               \
				(ptr)->total_ecd = (ptr)->round_cnt * 8192 + (ptr)->ecd - (ptr)->offset_ecd;                         \
																																															               \
				(ptr)->total_ecd_out_2006 = ((ptr)->round_cnt * 8192 + (ptr)->ecd - (ptr)->offset_ecd) / 36.f;             \
				(ptr)->total_ecd_out_3508 = ((ptr)->round_cnt * 8192 + (ptr)->ecd - (ptr)->offset_ecd) * 187.f / 3591.f;   \
				(ptr)->total_ecd_out_6020 = (ptr)->total_ecd;           																		 							 \
   }
		
//����0x201-0x204�û��󽮵�����ݶ�ȡ
#define get_user_motor_measure(ptr, rx_message)                                                     \
    {                                                                                          \
        (ptr)->last_ecd = (ptr)->ecd;                                                          \
        (ptr)->ecd = (uint16_t)((rx_message)->Data[0] << 8 | (rx_message)->Data[1]);           \
        (ptr)->speed_rpm = (uint16_t)((rx_message)->Data[2] << 8 | (rx_message)->Data[3]);     \
        (ptr)->given_current = (uint16_t)((rx_message)->Data[4] << 8 | (rx_message)->Data[5]); \
        (ptr)->temperate = (rx_message)->Data[6];                                              \
    }

//��̨������ݶ�ȡ
#define get_gimbal_motor_measure(ptr, rx_message)                                              \
    {                                                                                          \
        (ptr)->last_ecd = (ptr)->ecd;                                                          \
        (ptr)->ecd = (uint16_t)((rx_message)->Data[0] << 8 | (rx_message)->Data[1]);           \
        (ptr)->speed_rpm = (uint16_t)((rx_message)->Data[2] << 8 | (rx_message)->Data[3]); \
        (ptr)->given_current = (uint16_t)((rx_message)->Data[4] << 8 | (rx_message)->Data[5]);     \
        (ptr)->temperate = (rx_message)->Data[6];                                              \
    }

//ͳһ����can���պ���
static void CAN1_hook(CanRxMsg *rx_message);
static void CAN2_hook(CanRxMsg *rx_message);
		
//�����������
motor_measure_t motor_yaw, motor_pit1, motor_pit2, motor_trigger, motor_reel, motor_shell, motor_user[4], motor_chassis[4], motor_fric1, motor_fric2;
//�����������ݱ���
cap_measure_t supercap;
		
static CanTxMsg GIMBAL_TxMessage;

//can1�ж�
CanRxMsg rx1_message;
void CAN1_RX0_IRQHandler(void)
{
//    static CanRxMsg rx1_message;

    if (CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET)
    {
        CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
        CAN_Receive(CAN1, CAN_FIFO0, &rx1_message);
        CAN1_hook(&rx1_message);
    }
}

//can2�ж�
CanRxMsg rx2_message;
void CAN2_RX1_IRQHandler(void)
{
//    static CanRxMsg rx2_message;
    if (CAN_GetITStatus(CAN2, CAN_IT_FMP1) != RESET)
    {
        CAN_ClearITPendingBit(CAN2, CAN_IT_FMP1);
        CAN_Receive(CAN2, CAN_FIFO1, &rx2_message);
			
        CAN2_hook(&rx2_message);
    }
}


//���ͳ������ݿ����������revΪ�����ֽ�
void CAN_CMD_CAP(int16_t temPower)
{
	  CanTxMsg CAP_TxMessage;
    CAP_TxMessage.StdId = CAN_CAP_SET_ID;
    CAP_TxMessage.IDE = CAN_ID_STD;
    CAP_TxMessage.RTR = CAN_RTR_DATA;
    CAP_TxMessage.DLC = 0x08;
    CAP_TxMessage.Data[0] = (temPower >> 8);
    CAP_TxMessage.Data[1] = temPower;
    CAN_Transmit( CAN1, &CAP_TxMessage);
}

//������̨�����������revΪ�����ֽ�
void CAN_CMD_GIMBAL(int16_t yaw, int16_t pitch1, int16_t pitch2, int16_t rev)
{
    GIMBAL_TxMessage.StdId = CAN_GIMBAL_ALL_ID;
    GIMBAL_TxMessage.IDE = CAN_ID_STD;
    GIMBAL_TxMessage.RTR = CAN_RTR_DATA;
    GIMBAL_TxMessage.DLC = 0x08;
    GIMBAL_TxMessage.Data[0] = (yaw >> 8);
    GIMBAL_TxMessage.Data[1] = yaw;
    GIMBAL_TxMessage.Data[2] = (pitch1 >> 8);
    GIMBAL_TxMessage.Data[3] = pitch1;
    GIMBAL_TxMessage.Data[4] = (pitch2 >> 8);
    GIMBAL_TxMessage.Data[5] = pitch2;
    GIMBAL_TxMessage.Data[6] = (rev >> 8);
    GIMBAL_TxMessage.Data[7] = rev;

    CAN_Transmit( GIMBAL_CAN,  &GIMBAL_TxMessage );
}

void CAN_CMD_2006(int16_t User_Motor1, int16_t User_Motor2, int16_t User_Motor3, int16_t User_Motor4)
{
    CanTxMsg TxMessage;
    TxMessage.StdId = CAN_USER_ALL_ID;
    TxMessage.IDE = CAN_ID_STD;
    TxMessage.RTR = CAN_RTR_DATA;
    TxMessage.DLC = 0x08;
    TxMessage.Data[0] = User_Motor1 >> 8;
    TxMessage.Data[1] = User_Motor1;
    TxMessage.Data[2] = User_Motor2 >> 8;
    TxMessage.Data[3] = User_Motor2;
	  TxMessage.Data[4] = User_Motor3 >> 8;
    TxMessage.Data[5] = User_Motor3;
    TxMessage.Data[6] = User_Motor4 >> 8;
    TxMessage.Data[7] = User_Motor4;

    CAN_Transmit(CAN1, &TxMessage);
}

//����0x201-0x204�����󽮵����������
void CAN_CMD_SHOOT(int16_t User_Motor1, int16_t User_Motor2, int16_t User_Motor3, int16_t User_Motor4)
{
    CanTxMsg TxMessage;
    TxMessage.StdId = CAN_SHOOT_ALL_ID;
    TxMessage.IDE = CAN_ID_STD;
    TxMessage.RTR = CAN_RTR_DATA;
    TxMessage.DLC = 0x08;
    TxMessage.Data[0] = User_Motor1 >> 8;
    TxMessage.Data[1] = User_Motor1;
    TxMessage.Data[2] = User_Motor2 >> 8;
    TxMessage.Data[3] = User_Motor2;
	  TxMessage.Data[4] = User_Motor3 >> 8;
    TxMessage.Data[5] = User_Motor3;
    TxMessage.Data[6] = User_Motor4 >> 8;
    TxMessage.Data[7] = User_Motor4;

    CAN_Transmit(CAN2, &TxMessage);
}

//CAN ���� 0x700��ID�����ݣ�������M3508�����������IDģʽ
void CAN_CMD_CHASSIS_RESET_ID(void)
{
    CanTxMsg TxMessage;
    TxMessage.StdId = 0x700;
    TxMessage.IDE = CAN_ID_STD;
    TxMessage.RTR = CAN_RTR_DATA;
    TxMessage.DLC = 0x08;
    TxMessage.Data[0] = 0;
    TxMessage.Data[1] = 0;
    TxMessage.Data[2] = 0;
    TxMessage.Data[3] = 0;
    TxMessage.Data[4] = 0;
    TxMessage.Data[5] = 0;
    TxMessage.Data[6] = 0;
    TxMessage.Data[7] = 0;

    CAN_Transmit(CAN2, &TxMessage);
}

//���͵��̵����������
void CAN_CMD_CHASSIS(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4)
{
    CanTxMsg TxMessage;
    TxMessage.StdId = CAN_CHASSIS_ALL_ID;
    TxMessage.IDE = CAN_ID_STD;
    TxMessage.RTR = CAN_RTR_DATA;
    TxMessage.DLC = 0x08;
    TxMessage.Data[0] = motor1 >> 8;
    TxMessage.Data[1] = motor1;
    TxMessage.Data[2] = motor2 >> 8;
    TxMessage.Data[3] = motor2;
    TxMessage.Data[4] = motor3 >> 8;
    TxMessage.Data[5] = motor3;
    TxMessage.Data[6] = motor4 >> 8;
    TxMessage.Data[7] = motor4;

    CAN_Transmit(CHASSIS_CAN, &TxMessage);
}

//����yaw���������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
const motor_measure_t *get_Yaw_Gimbal_Motor_Measure_Point(void)
{
    return &motor_yaw;
}
//����pitch1���������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
const motor_measure_t *get_Pitch1_Gimbal_Motor_Measure_Point(void)
{
    return &motor_pit1;
}
//����pitch2���������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
const motor_measure_t *get_Pitch2_Gimbal_Motor_Measure_Point(void)
{
    return &motor_pit2;
}
//����trigger���������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
const motor_measure_t *get_Trigger_Motor_Measure_Point(void)
{
    return &motor_trigger;
}
//����reel���������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
const motor_measure_t *get_Reel_Motor_Measure_Point(void)
{
    return &motor_reel;
}
//����trigger���������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
const motor_measure_t *get_Shell_Motor_Measure_Point(void)
{
    return &motor_shell;
}
//����Fric1���������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
const motor_measure_t *get_Fric1_Motor_Measure_Point(void)
{
    return &motor_fric1;
}
//����Fric2���������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
const motor_measure_t *get_Fric2_Motor_Measure_Point(void)
{
    return &motor_fric2;
}
//���س������ݱ�����ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
const cap_measure_t *get_Supercap_Measure_Point(void)
{
    return &supercap;
}
//����0x201-0x204�����󽮵��������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
const motor_measure_t *get_User_Motor_Measure_Point(uint8_t i)
{
	return &motor_user[(i | 0x00)];
}
//���ص��̵��������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
const motor_measure_t *get_Chassis_Motor_Measure_Point(uint8_t i)
{
    return &motor_chassis[(i & 0x04)];
}

//����can1�жϺ��������Ҽ�¼�������ݵ�ʱ�䣬��Ϊ�����ж�����
static void CAN1_hook(CanRxMsg *rx_message)
{
    switch (rx_message->StdId)
    {
		case CAN_3508_M1_ID:
    case CAN_3508_M2_ID:
    case CAN_3508_M3_ID:
    case CAN_3508_M4_ID:
    {
        static uint8_t i = 0;
        //������ID��
        i = rx_message->StdId - CAN_3508_M1_ID;
        //���������ݺ꺯��
        get_user_motor_measure(&motor_user[i], rx_message);
        break;
    }		

		    case CAN_YAW_REEL_ID:
    {
        //���������ݺ꺯��
        get_motor_measure(&motor_reel, rx_message);
        break;
    }
		
				 case CAN_YAW_SHELL_ID:
    {
        //���������ݺ꺯��
        get_motor_measure(&motor_shell, rx_message);
        break;
    }
				case CAN_CAP_FEEDBACK_ID:
    {
				uint16_t * pPowerdata = (uint16_t *) (rx_message)->Data;
				supercap.Vin = (float)pPowerdata[0] / 100.f;
				supercap.Vcap = (float)pPowerdata[1] / 100.f;
				supercap.Iin = (float)pPowerdata[2] / 100.f;
				supercap.Pset = (float)pPowerdata[3] / 100.f;
        break;
    }
			default:
			{
					break;
			}
    }
}

static void CAN2_hook(CanRxMsg *rx_message)
{
	  switch (rx_message->StdId)
    {	
			
			case CAN_YAW_MOTOR_ID:
			{
					//���������ݺ꺯��
					get_gimbal_motor_measure(&motor_yaw, rx_message);
					break;
			}
			case CAN_PIT1_MOTOR_ID:
			{
					//���������ݺ꺯��
					get_gimbal_motor_measure(&motor_pit1, rx_message);
					break;
			}
			case CAN_PIT2_MOTOR_ID:
			{
					//���������ݺ꺯��
					get_gimbal_motor_measure(&motor_pit2, rx_message);
					break;
			}
			 case CAN_FRIC1_ID:
			{
					//���������ݺ꺯��
					get_gimbal_motor_measure(&motor_fric1, rx_message);
					break;
			}
					 case CAN_FRIC2_ID:
			{
					//���������ݺ꺯��
					get_gimbal_motor_measure(&motor_fric2, rx_message);
					break;
			}
					 case CAN_TRIGGER_MOTOR_ID:
			{
					//���������ݺ꺯��
					get_gimbal_motor_measure(&motor_trigger, rx_message);
					break;
			}
			default:
			{
					break;
			}
    }
	
}
