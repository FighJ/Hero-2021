#ifndef JUDGE_ANALYSIS_H
#define JUDGE_ANALYSIS_H
#include "stm32f4xx.h"

#ifdef  __DRIVER_GLOBALS
#define __DRIVER_EXT
#else
#define __DRIVER_EXT extern
#endif

///************************ 2021��Ĳ���ϵͳЭ��ṹ�� Start ***********************/
#define JudgeBufferLength       255

#define JudgeFrameHeader        0xA5        //֡ͷ
#define FrameHeader_Len         5 

#define CMD_LEN      2    //cmdid bytes
#define CRC_LEN      2    //crc16 bytes
#define HEADER_LEN   sizeof(FrameHeader)  //frame length


typedef __packed struct
{
  uint8_t   SOF;//0xA5
  uint16_t  DataLenth;
	uint8_t   Seq;
  uint8_t   CRC8;
}FrameHeader;

/***************������ID********************/

/* 

	ID: 0x0001  Byte:  11    ����״̬����       		    	����Ƶ�� 1Hz      
	ID: 0x0002  Byte:  1     �����������         		    ������������      
	ID: 0x0003  Byte:  28    ����������Ѫ������  	     	  1Hz����  
  ID: 0x0004  Byte:  3     ���ڷ���״̬  		            ���ڷ������  
  ID: 0x0005  Byte:  11    �˹�������ս���ӳ���ͷ�״̬  1Hz���ڷ���   
	ID: 0x0101  Byte:  4     �����¼�����   				       �¼��ı����
	ID: 0x0102  Byte:  3     ���ز���վ������ʶ����    	   �����ı���� 
	ID: 0X0103  Byte:  2     ���󲹸�վ��������            �����ӷ��ͣ�10Hz 
  ID: 0X0104  Byte:  2     ���о�������                  ���淢������
  ID: 0X0105  Byte:  1     ���ڷ���ڵ���ʱ              1Hz���ڷ���
	ID: 0X0201  Byte: 15     ������״̬����        		     10Hz
	ID: 0X0202  Byte: 14     ʵʱ������������   			     50Hz       
	ID: 0x0203  Byte: 16     ������λ������           	   10Hz
	ID: 0x0204  Byte:  1     ��������������           	   ����״̬�ı����
  ID: 0x0205  Byte:  3     ���л���������״̬����        10Hz  ֻ�п��л��������ط���
	ID: 0x0206  Byte:  1     �˺�״̬����           		   �˺���������
	ID: 0x0207  Byte:  6     ʵʱ�������           		   �ӵ��������
  ID: 0x0208  Byte:  2     �ӵ�ʣ�෢����                ���˻����ڱ����� 10Hz ���л�����
  ID: 0x0209  Byte:  4     ������RFID״̬                1HZ���ڷ���
  ID: 0x020A  Byte:  12    ���ڻ����˿ͻ���ָ����        10HZ���ڷ���
	ID: 0x0301  Byte:  n     �����˼佻������           	 ���ͷ���������,10Hz
	ID: 0x0302  Byte:  n     �Զ��彻������������          �ͻ��˴�������,30Hz
	ID: 0x0303  Byte:  15    �ͻ���С��ͼ��������          ��������,10Hz
	ID: 0x0304  Byte:  12    ���̡������Ϣ                ͼ�����ڷ���,10Hz
	
*/



typedef enum
{
	ID_game_state       					= 0x0001,//����״̬����
	ID_game_result 	   						= 0x0002,//�����������
	ID_game_robot_HP       				= 0x0003,//���������˴������
	ID_dart_state									= 0x0004,//���ڷ���״̬
	ID_ICRA_buff									= 0x0005,//�˹�������ս���ӳ���ͷ�״̬
	
	ID_event_data  								= 0x0101,//�����¼����� 
	ID_supply_projectile_action   = 0x0102,//���ز���վ������ʶ����
	ID_referee_warning						=	0x0104,//���о�����Ϣ
	ID_dart_remaining_time				= 0x0105,//���ڷ���ڵ���ʱ
	
	Robot_Status_ID               =0x0201,//������״̬����
	power_heat_data_ID   	        =0x0202,//ǹ������ ���̹���
	game_robot_pos_ID        			= 0x0203,//������λ������
	robot_buff_ID                 =0x0204,//��������������
	aerial_robot_energy_ID				= 0x0205,//���л���������״̬����
	robot_hurt_ID                 =0x0206,//�˺�״̬����
	shoot_data_ID	      					=0x0207,//ʵʱ�������
	bullet_remaining_ID           = 0x0208,//�ӵ�ʣ�෢����  
	rfid_status_ID                = 0x0209,//RFID״̬����
	dart_attack_ID                = 0x020A,//���ڻ����˿ͻ���ָ������

	ID_student_interactive        = 0x0301,//�������ݽ�����Ϣ ��UI
	ID_robot_interactive          = 0x0302,//�Զ��彻������������
	ID_robot_command              = 0x0303,//С��ͼ��������
	ID_tuchuan_command            = 0x0304,//ͼ������
	ID_map_command                = 0x0305,//С��ͼ����
} Judege_Cmd_ID;


/*************************************����CmdID��Ӧ�Ľṹ��*************************/
//������ã�ע������.c��switch������
//����״̬���� ������ID 0x0001  ����Ƶ�ʣ�1Hz
typedef __packed struct
{
	uint8_t game_type:4;     //0-3bit:�������� 1����ʦ�� 2:������ 3���˹�������ս�� 4��3V3 5:1V1
	uint8_t game_progress:4; //4-7bit:�����׶� 0��δ��ʼ���� 1��׼���׶� 2���Լ�׶� 3:5s����ʱ 4����ս�� 5������������
	uint16_t stage_remain_time; //��ǰ�׶�ʣ��ʱ�� /s
	
	uint64_t SyncTimeStamp;
}ext_game_status_t;

//����������� ������ID 0x0002  ����Ƶ�ʣ�������������
typedef __packed struct
{
	uint8_t winner;  //0:ƽ�� 1:�췽ʤ�� 2������ʤ��
}ext_game_result_t;


//������Ѫ������ ������ID 0x0003  ����Ƶ�ʣ�1Hz
typedef __packed struct
{
	uint16_t red_1_robot_HP; //��1Ӣ�ۻ�����Ѫ��  δ�ϳ��Լ�����Ѫ��Ϊ0
	uint16_t red_2_robot_HP; //��2���̻�����Ѫ�� 
	uint16_t red_3_robot_HP; //��3����������Ѫ�� 
	uint16_t red_4_robot_HP; //��4����������Ѫ�� 
	uint16_t red_5_robot_HP; //��5����������Ѫ�� 
	uint16_t red_7_robot_HP; //��5�ڱ�������Ѫ�� 
	
	uint16_t red_outpost_HP; //�췽ǰ��վ
	uint16_t red_base_HP; //�췽����
	
	uint16_t blue_1_robot_HP; //��1Ӣ�ۻ�����Ѫ��  δ�ϳ��Լ�����Ѫ��Ϊ0
	uint16_t blue_2_robot_HP; //��2���̻�����Ѫ�� 
	uint16_t blue_3_robot_HP; //��3����������Ѫ�� 
	uint16_t blue_4_robot_HP; //��4����������Ѫ�� 
	uint16_t blue_5_robot_HP; //��5����������Ѫ�� 
	uint16_t blue_7_robot_HP; //��5�ڱ�������Ѫ�� 
	
	uint16_t blue_outpost_HP; //����ǰ��վ
	uint16_t blue_base_HP; //��������
}ext_game_robot_HP_t;

//���ڷ���״̬ ������ID 0x0004  ����Ƶ�ʣ����ڷ������  ���ͷ�Χ�����л�����
typedef __packed struct
{
	uint8_t dart_belong; //������ڵĶ���: 1���췽����  2����������
	uint8_t stage_remaining_time; //����ʱʣ��ı���ʱ��  /s
}ext_dart_status;

//�����¼����� ������ID 0x0101  ����Ƶ�ʣ��¼��ı����
/*
bit0: ��������վ 1�Ų�Ѫ��ռ��״̬ 1Ϊ��ռ��
bit1: ��������վ 2�Ų�Ѫ��ռ��״̬ 1Ϊ��ռ��
bit2: ��������վ 3�Ų�Ѫ��ռ��״̬ 1Ϊ��ռ��
bit3-5��������������״̬��
bit3�������ռ��״̬ 1Ϊ��ռ��
bit4��С�������ؼ���״̬ 1Ϊ�Ѽ���
bit5�����������ؼ���״̬ 1Ϊ�Ѽ���
bit6������R2���θߵ�ռ��״̬ 1Ϊ��ռ��
bit7������R3���θߵ�ռ��״̬ 1Ϊ��ռ��
bit8������R4���θߵ�ռ��״̬ 1Ϊ��ռ�� 
bit9���������ػ���״̬��
			1Ϊ���������⻤��Ѫ��
			0Ϊ���������⻤��Ѫ��
bit10������ǰ��ս״̬��
			1Ϊǰ��ս���
			0Ϊǰ��ս������
bit10-31������

*/
typedef __packed struct
{
	uint32_t event_type;
}ext_event_data_t;


//����վ������ʶ ������ID 0x0102  ����Ƶ�ʣ������ı���� ���ͷ�Χ������������
typedef __packed struct
{
	uint8_t supply_projectile_id; //������ID 1:1�Ų�����  2��2�Ų�����
	uint8_t supply_robot_id;   //����������|D��0Ϊ��ǰ�޻����˲�����1Ϊ�췽Ӣ�ۻ����˲�����2Ϊ�췽���̻����˲�����3/4/5Ϊ�췽���������˲�����
	                           //101Ϊ����Ӣ�ۻ����˲�����102Ϊ�������̻����˲�����103/104/105Ϊ�������������˲���
	
	uint8_t supply_projectile_step;  //�����ڿ���״̬��0Ϊ�رգ�1Ϊ�ӵ�׼���У�2Ϊ�ӵ�����
	 
	uint8_t supply_projectile_num;  //��������50: 50���ӵ� 100:100���ӵ� 150:150���ӵ� 200:200���ӵ���
}ext_supply_projectile_action_t;


//���о�����Ϣ ������ID 0x0104  ����Ƶ�ʣ������������ 
typedef __packed struct
{
	uint8_t level;  //����ȼ� 1������  2������  3���и�
	uint8_t foul_robot_id;  //���������ID �и�ʱ��������IDΪ0  ���ơ�����ʱ��������IDΪ���������ID
}ext_referee_warning_t;


//���ڷ���ڵ���ʱ ������ID 0x0105  ����Ƶ�ʣ�1Hz  ���ͷ�Χ������������
typedef __packed struct
{
	uint8_t dart_remaining_time;  //15s����ʱ
}ext_dart_remaining_time_t;


//����������״̬ ������ID 0x0201  ����Ƶ�ʣ�10Hz 
/*��������|D 
1���췽Ӣ�ۻ����� 2���췽���̻����� 3/4/5���췽���������� 6���췽���л�����
7���췽�ڱ������� 8���췽���ڻ����� 9���췽�״�վ
101������Ӣ�ۻ����� 102���������̻����� 103/104/105���������������� 106: �������л�����
107�������ڱ������� 108���������ڻ����� 109�������״�վ��
*/
typedef __packed struct
{
	uint8_t robot_id;
	uint8_t robot_level; //�����˵ȼ� 1��һ����2��������3��������
	uint16_t remain_HP;  //������ʣ��Ѫ��
	uint16_t max_HP;     //����������Ѫ��
	uint16_t shooter_id1_17mm_cooling_rate; //������1��17mmǹ��ÿ����ȴֵ
	uint16_t shooter_id1_17mm_cooling_limit;//������1��17mmǹ����������
	uint16_t shooter_id1_17mm_speed_limit;  //������1��17mmǹ�������ٶ� m/s
	
	uint16_t shooter_id2_17mm_cooling_rate; //������2��17mmǹ��ÿ����ȴֵ
	uint16_t shooter_id2_17mm_cooling_limit;//������2��17mmǹ��ÿ����ȴֵ
	uint16_t shooter_id2_17mm_speed_limit;  //������2��17mmǹ�������ٶ� m/s

	uint16_t shooter_id2_42mm_cooling_rate;//������42mmǹ��ÿ����ȴֵ
	uint16_t shooter_id2_42mm_cooling_limit;//������42mmǹ����������
	uint16_t shooter_id2_42mm_speed_limit;//������42mmǹ�������ٶ� m/s 
	
	uint16_t chassis_power_limit;  //�����˵��̹�������
	uint8_t mains_power_gimbal_output:1;  //0bt:gmba�������1Ϊ��24V�����0Ϊ��24v�����                                     
	uint8_t mains_power_chassis_output:1; // 1bit:chassis�������1Ϊ��24V�����0Ϊ��24v�����
	uint8_t mains_power_shooter_output:1; //2bit:shooter�������1Ϊ��24V�����0Ϊ��24v�����
} ext_game_robot_state_t;


//ʵʱ������������ ������ID 0x0202  ����Ƶ�ʣ�50Hz 
typedef __packed struct
{
	uint16_t chassis_volt;  //���������ѹ ����
	uint16_t chassis_current; //����������� ����
	float chassis_power;  //����������� W
	uint16_t chassis_power_buffer; //���̹��ʻ���  J ���¸��ݹ���������250J
	uint16_t shooter_id1_17mm_cooling_heat; //1��17mm ǹ������
	
	uint16_t shooter_id2_17mm_cooling_heat; //2��17mm ǹ������
	
	uint16_t shooter_id1_42mm_cooling_heat; //42mm ǹ������
} ext_power_heat_data_t;

//������λ�� ������ID 0x0203  ����Ƶ�ʣ�10Hz 
typedef __packed struct
{
	float x; // m
	float y;
	float z; 
	float yaw;  //��
}ext_game_robot_pos_t;

//���������� ������ID 0x0204  ����Ƶ�ʣ�1Hz 
typedef __packed struct
{
  uint8_t power_rune_buff;//bit0��������Ѫ����Ѫ״̬ bit1��ǹ��������ȴ���� bit2�������˷����ӳ� bit3�������˹����ӳ� ����bt����
}ext_buff_t;

//���л���������״̬ ������ID 0x0205  ����Ƶ�ʣ�10Hz
typedef __packed struct
{
  uint8_t attack_time;//�ɹ���ʱ�� s�� 30s�ݼ�
}aerial_robot_energy_t;


//�˺�״̬ ������ID 0x0206  ����Ƶ�ʣ��˺���������
/*
bit0-3 ��Ѫ���仯����Ϊװ���˺�������װ��ID��������ֵ0-4�Ŵ�������˵����װ��Ƭ
������Ѫ���仯���ͣ��ñ�����ֵΪ0��
bit4-7 Ѫ���仯����
0x0 װ��ģ���Ѫ
0x1 ģ����߿�Ѫ
0x2 �����ٿ�Ѫ
0x3 ��ǹ��������Ѫ
0x4 �����̹��ʿ�Ѫ
0x5 װ��ײ����Ѫ
*/
typedef __packed struct
{
uint8_t armor_id : 4;
uint8_t hurt_type : 4;
} ext_robot_hurt_t;

//ʵʱ�����Ϣ ������ID 0x0207  ����Ƶ�ʣ��������
typedef __packed struct
{
  uint8_t bullet_type;//�ӵ�����1:17mm���� 2��42mm����
	uint8_t shooter_id;//�������ID 1:1��17mm������� 2:2��17mm������� 3:42mm�������
	uint8_t bullet_freq;//�ӵ���Ƶ  HZ
	float bullet_speed;//�ӵ����� m/s
}ext_shoot_data_t;



//�ӵ�ʣ�෢���� ������ID 0x0208  ����Ƶ�ʣ�10Hz���ڷ��ͣ����л����˷���
typedef __packed struct
{
	uint16_t bullet_remaining_num_17mm;
	uint16_t bullet_remaining_num_42mm;
	uint16_t coin_remaining_num;//ʣ��������
}ext_bullet_remaining_t;


//������RFID״̬ ������ID 0x0209  ����Ƶ�ʣ�1Hz ���ͷ�Χ����һ������
/*
bit0 ���������RFID״̬
bit1 �ߵ������RFID״̬
bit2 �������������RFID״̬
bit3 ���������RFID״̬
bit4 ǰ�ڸ������RFID״̬
bit5 ��Դ�������RFID״̬
bit6 ��Ѫ�������RFID״̬
bit7 ���̻����˲�Ѫ��RFID״̬
*/
typedef __packed struct
{
  uint32_t rfid_status;
}ext_rfid_status_t;


//���ڻ����˿ͻ���ָ������ ������ID 0x020A  ����Ƶ�ʣ�10Hz ���ͷ�Χ����һ������

typedef __packed struct
{
  uint8_t dart_launch_status;
	uint8_t dart_attack_target;
	uint16_t target_change_time;
	uint8_t first_dart_speed;
	uint8_t second_dart_speed;
	uint8_t third_dart_speed;
	uint8_t fourth_dart_speed;
	uint16_t last_dart_speed;
	uint16_t operate_launch_cmd_time;
}ext_dart_client_cmd_t;







/*�û��Զ��岿����ʾ����Ļ��*/
/*0-1 ��������ID 0xD180
	2-3 �����ߵ�ID
*/

/* 
	
	�������ݣ�����һ��ͳһ�����ݶ�ͷ�ṹ��
	���������� ID���������Լ������ߵ� ID ���������ݶΣ�
	�����������ݵİ��ܹ������Ϊ 128 ���ֽڣ�
	��ȥ frame_header,cmd_id,frame_tail �Լ����ݶ�ͷ�ṹ�� 6 ���ֽڣ�
	�ʶ����͵��������ݶ����Ϊ 113��
	������������ 0x0301 �İ�����Ƶ��Ϊ 10Hz��

	������ ID��
	1��Ӣ��(��)��
	2������(��)��
	3/4/5������(��)��
	6������(��)��
	7���ڱ�(��)��
	11��Ӣ��(��)��
	12������(��)��
	13/14/15������(��)��
	16������(��)��
	17���ڱ�(��)�� 
	�ͻ��� ID�� 
	0x0101 ΪӢ�۲����ֿͻ���( ��) ��
	0x0102 �����̲����ֿͻ��� ((�� )��
	0x0103/0x0104/0x0105�����������ֿͻ���(��)��
	0x0106�����в����ֿͻ���((��)�� 
	0x0111��Ӣ�۲����ֿͻ���(��)��
	0x0112�����̲����ֿͻ���(��)��
	0x0113/0x0114/0x0115�������ֿͻ��˲���(��)��
	0x0116�����в����ֿͻ���(��)�� 
*/
/* �������ݽ�����Ϣ��0x0301  */

typedef __packed struct
{
	uint16_t rxCmdId;
	uint16_t data_id;
	uint16_t send_id;
	uint16_t receive_id;
} id_data_t;

typedef __packed struct
{
	FrameHeader Header;
	id_data_t    id;
	float data1;
	float data2;
	float data3;
	uint8_t masks;
	uint16_t crc_16;
} client_custom_data_t;



//typedef __packed struct
//{
//	uint16_t rxCmdId;
//	uint16_t data_id;
//	uint16_t send_id;
//	uint16_t receive_id;
//} id_data_t;

//������֮���໥ͨ��
typedef __packed struct
{
	uint8_t data[112];
} robot_interactive_data_t;


typedef __packed struct
{
	uint16_t data_cmd_id;
	uint16_t send_ID;
	uint16_t receiver_ID;
}ext_student_interactive_header_data_t;


typedef  struct
{
  FrameHeader          							frameHeader;
  uint16_t              						rxCmdId;
	
	ext_game_status_t   						  GameState;				        	//0x0001
	ext_game_result_t     						GameResult;			        	  //0x0002
	ext_game_robot_HP_t   						GameRobotHP;			          //0x0003
	ext_dart_status      							GameDart;                   //0x0004
	
	ext_event_data_t        			   	EventData;					        //0x0101		
	ext_supply_projectile_action_t	 	SupplyProjectileAction;		  //0x0102
	ext_referee_warning_t            	RefereeWarn;                //0x0104
	ext_dart_remaining_time_t        	DartRemain;                 //0x0105
	
	
  ext_game_robot_state_t 						game_robot_state_t;         //0x0201
  ext_power_heat_data_t 						power_heat_data_t;          //0x0202
	ext_game_robot_pos_t							GameRobotPos;				        //0x0203
	ext_buff_t            						Buff;                       //0x0204
	aerial_robot_energy_t							AerialRobotEnergy;			    //0x0205
	ext_robot_hurt_t      						robot_hurt_t;               //0x0206
  ext_shoot_data_t		  						shoot_data_t;               //0x0207
  ext_bullet_remaining_t          	Bulletrema;                 //0x0208


	
//  robot_interactive_data_t robot_data_t;
	client_custom_data_t robot_data_t;
	client_custom_data_t userinfo;
}JudgementDataTypedef;




typedef __packed struct
{
	float data1;
	float data2;
	float data3;
	uint8_t mask;
}tSelfDefineInfo;  

typedef  __packed struct
{
	float data1;
	float data2;
	float data3;
	uint8_t mask;
}SelfDefineInfo_t;   //ѧ���ϴ��Զ������� (0x0005)  

 


typedef  struct
{
  FrameHeader        frameHeader;
  uint16_t              rxCmdId;
		__packed union 
	{
  ext_power_heat_data_t power_heat_data_t;
  ext_shoot_data_t		  shoot_data_t;
	ext_robot_hurt_t      robot_hurt_t;
	ext_game_robot_state_t game_robot_state_t;
  robot_interactive_data_t robot_data_t;
	client_custom_data_t userinfo;
		}Data;
	uint16_t        CRC16;
	uint16_t        CRC16_2 ;
}FRAME;



typedef __packed struct
{
	FrameHeader    FrameHeader;
//	tCmdID          CmdID;
	tSelfDefineInfo SelfDefineInfo; 
	uint16_t        CRC16;         
}tFrame;  


__DRIVER_EXT uint8_t JudgeDataBuffer[JudgeBufferLength];
__DRIVER_EXT SelfDefineInfo_t SelfDefineInfo;
__DRIVER_EXT JudgementDataTypedef JudgementData;

void Send_FrameData(uint16_t cmdid, uint8_t * pchMessage,uint16_t dwLength);
void client_send(uint8_t * data);
void client_init(uint8_t * data1,uint8_t * data2);
void get_chassis_power_and_buffer(float *power, float *buffer, uint16_t *powmax);


#endif

