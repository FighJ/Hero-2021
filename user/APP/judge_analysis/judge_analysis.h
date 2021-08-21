#ifndef JUDGE_ANALYSIS_H
#define JUDGE_ANALYSIS_H
#include "stm32f4xx.h"

#ifdef  __DRIVER_GLOBALS
#define __DRIVER_EXT
#else
#define __DRIVER_EXT extern
#endif

///************************ 2021年的裁判系统协议结构体 Start ***********************/
#define JudgeBufferLength       255

#define JudgeFrameHeader        0xA5        //帧头
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

/***************命令码ID********************/

/* 

	ID: 0x0001  Byte:  11    比赛状态数据       		    	发送频率 1Hz      
	ID: 0x0002  Byte:  1     比赛结果数据         		    比赛结束后发送      
	ID: 0x0003  Byte:  28    比赛机器人血量数据  	     	  1Hz发送  
  ID: 0x0004  Byte:  3     飞镖发射状态  		            飞镖发射后发送  
  ID: 0x0005  Byte:  11    人工智能挑战赛加成与惩罚状态  1Hz周期发送   
	ID: 0x0101  Byte:  4     场地事件数据   				       事件改变后发送
	ID: 0x0102  Byte:  3     场地补给站动作标识数据    	   动作改变后发送 
	ID: 0X0103  Byte:  2     请求补给站补弹数据            参赛队发送，10Hz 
  ID: 0X0104  Byte:  2     裁判警告数据                  警告发生后发送
  ID: 0X0105  Byte:  1     飞镖发射口倒计时              1Hz周期发送
	ID: 0X0201  Byte: 15     机器人状态数据        		     10Hz
	ID: 0X0202  Byte: 14     实时功率热量数据   			     50Hz       
	ID: 0x0203  Byte: 16     机器人位置数据           	   10Hz
	ID: 0x0204  Byte:  1     机器人增益数据           	   增益状态改变后发送
  ID: 0x0205  Byte:  3     空中机器人能量状态数据        10Hz  只有空中机器人主控发送
	ID: 0x0206  Byte:  1     伤害状态数据           		   伤害发生后发送
	ID: 0x0207  Byte:  6     实时射击数据           		   子弹发射后发送
  ID: 0x0208  Byte:  2     子弹剩余发射数                无人机及哨兵发送 10Hz 所有机器人
  ID: 0x0209  Byte:  4     机器人RFID状态                1HZ周期发送
  ID: 0x020A  Byte:  12    飞镖机器人客户端指令书        10HZ周期发送
	ID: 0x0301  Byte:  n     机器人间交互数据           	 发送方触发发送,10Hz
	ID: 0x0302  Byte:  n     自定义交互器交互数据          客户端触发发送,30Hz
	ID: 0x0303  Byte:  15    客户端小地图交互数据          触发发送,10Hz
	ID: 0x0304  Byte:  12    键盘、鼠标信息                图传串口发送,10Hz
	
*/



typedef enum
{
	ID_game_state       					= 0x0001,//比赛状态数据
	ID_game_result 	   						= 0x0002,//比赛结果数据
	ID_game_robot_HP       				= 0x0003,//比赛机器人存活数据
	ID_dart_state									= 0x0004,//飞镖发射状态
	ID_ICRA_buff									= 0x0005,//人工智能挑战赛加成与惩罚状态
	
	ID_event_data  								= 0x0101,//场地事件数据 
	ID_supply_projectile_action   = 0x0102,//场地补给站动作标识数据
	ID_referee_warning						=	0x0104,//裁判警告信息
	ID_dart_remaining_time				= 0x0105,//飞镖发射口倒计时
	
	Robot_Status_ID               =0x0201,//机器人状态数据
	power_heat_data_ID   	        =0x0202,//枪口热量 底盘功率
	game_robot_pos_ID        			= 0x0203,//机器人位置数据
	robot_buff_ID                 =0x0204,//机器人增益数据
	aerial_robot_energy_ID				= 0x0205,//空中机器人能量状态数据
	robot_hurt_ID                 =0x0206,//伤害状态数据
	shoot_data_ID	      					=0x0207,//实时射击数据
	bullet_remaining_ID           = 0x0208,//子弹剩余发射数  
	rfid_status_ID                = 0x0209,//RFID状态数据
	dart_attack_ID                = 0x020A,//飞镖机器人客户端指令数据

	ID_student_interactive        = 0x0301,//交互数据接收信息 画UI
	ID_robot_interactive          = 0x0302,//自定义交互器接收数据
	ID_robot_command              = 0x0303,//小地图交互数据
	ID_tuchuan_command            = 0x0304,//图传数据
	ID_map_command                = 0x0305,//小地图数据
} Judege_Cmd_ID;


/*************************************具体CmdID对应的结构体*************************/
//按需调用，注意请在.c的switch语句添加
//比赛状态数据 命令码ID 0x0001  发送频率：1Hz
typedef __packed struct
{
	uint8_t game_type:4;     //0-3bit:比赛类型 1：大师赛 2:单项赛 3：人工智能挑战赛 4；3V3 5:1V1
	uint8_t game_progress:4; //4-7bit:比赛阶段 0：未开始比赛 1：准备阶段 2：自检阶段 3:5s倒计时 4：对战中 5：比赛结算中
	uint16_t stage_remain_time; //当前阶段剩余时间 /s
	
	uint64_t SyncTimeStamp;
}ext_game_status_t;

//比赛结果数据 命令码ID 0x0002  发送频率：比赛结束后发送
typedef __packed struct
{
	uint8_t winner;  //0:平局 1:红方胜利 2：蓝方胜利
}ext_game_result_t;


//机器人血量数据 命令码ID 0x0003  发送频率：1Hz
typedef __packed struct
{
	uint16_t red_1_robot_HP; //红1英雄机器人血量  未上场以及罚下血量为0
	uint16_t red_2_robot_HP; //红2工程机器人血量 
	uint16_t red_3_robot_HP; //红3步兵机器人血量 
	uint16_t red_4_robot_HP; //红4步兵机器人血量 
	uint16_t red_5_robot_HP; //红5步兵机器人血量 
	uint16_t red_7_robot_HP; //红5哨兵机器人血量 
	
	uint16_t red_outpost_HP; //红方前哨站
	uint16_t red_base_HP; //红方基地
	
	uint16_t blue_1_robot_HP; //蓝1英雄机器人血量  未上场以及罚下血量为0
	uint16_t blue_2_robot_HP; //蓝2工程机器人血量 
	uint16_t blue_3_robot_HP; //蓝3步兵机器人血量 
	uint16_t blue_4_robot_HP; //蓝4步兵机器人血量 
	uint16_t blue_5_robot_HP; //蓝5步兵机器人血量 
	uint16_t blue_7_robot_HP; //蓝5哨兵机器人血量 
	
	uint16_t blue_outpost_HP; //蓝方前哨站
	uint16_t blue_base_HP; //蓝方基地
}ext_game_robot_HP_t;

//飞镖发射状态 命令码ID 0x0004  发送频率：飞镖发射后发送  发送范围：所有机器人
typedef __packed struct
{
	uint8_t dart_belong; //发射飞镖的队伍: 1：红方飞镖  2：蓝方飞镖
	uint8_t stage_remaining_time; //发射时剩余的比赛时间  /s
}ext_dart_status;

//场地事件数据 命令码ID 0x0101  发送频率：事件改变后发送
/*
bit0: 己方补给站 1号补血点占领状态 1为已占领
bit1: 己方补给站 2号补血点占领状态 1为已占领
bit2: 己方补给站 3号补血点占领状态 1为已占领
bit3-5：己方能量机关状态：
bit3：打击点占领状态 1为已占领
bit4：小能量机关激活状态 1为已激活
bit5：大能量机关激活状态 1为已激活
bit6：己方R2环形高地占领状态 1为已占领
bit7：己方R3梯形高地占领状态 1为已占领
bit8：己方R4梯形高地占领状态 1为已占领 
bit9：己方基地护盾状态：
			1为基地有虚拟护盾血量
			0为基地无虚拟护盾血量
bit10：己方前哨战状态：
			1为前哨战存活
			0为前哨战被击毁
bit10-31：保留

*/
typedef __packed struct
{
	uint32_t event_type;
}ext_event_data_t;


//补给站动作标识 命令码ID 0x0102  发送频率：动作改变后发送 发送范围：己方机器人
typedef __packed struct
{
	uint8_t supply_projectile_id; //补给口ID 1:1号补给口  2：2号补给口
	uint8_t supply_robot_id;   //补弹机器人|D：0为当前无机器人补弹，1为红方英雄机器人补弹，2为红方工程机器人补弹，3/4/5为红方步兵机器人补弹，
	                           //101为蓝方英雄机器人补弹，102为蓝方工程机器人补弹，103/104/105为蓝方步兵机器人补弹
	
	uint8_t supply_projectile_step;  //出弹口开闭状态：0为关闭，1为子弹准备中，2为子弹下落
	 
	uint8_t supply_projectile_num;  //补弹数量50: 50颗子弹 100:100颗子弹 150:150颗子弹 200:200颗子弹。
}ext_supply_projectile_action_t;


//裁判警告信息 命令码ID 0x0104  发送频率：发生警告后发送 
typedef __packed struct
{
	uint8_t level;  //警告等级 1：黄牌  2：红牌  3：判负
	uint8_t foul_robot_id;  //犯规机器人ID 判负时，机器人ID为0  黄牌、红牌时，机器人ID为犯规机器人ID
}ext_referee_warning_t;


//飞镖发射口倒计时 命令码ID 0x0105  发送频率：1Hz  发送范围：己方机器人
typedef __packed struct
{
	uint8_t dart_remaining_time;  //15s倒计时
}ext_dart_remaining_time_t;


//比赛机器人状态 命令码ID 0x0201  发送频率：10Hz 
/*本机器人|D 
1：红方英雄机器人 2：红方工程机器人 3/4/5：红方步兵机器人 6：红方空中机器人
7：红方哨兵机器人 8：红方飞镖机器人 9：红方雷达站
101：蓝方英雄机器人 102：蓝方工程机器人 103/104/105：蓝方步兵机器人 106: 蓝方空中机器人
107：蓝方哨兵机器人 108：蓝方飞镖机器人 109：蓝方雷达站。
*/
typedef __packed struct
{
	uint8_t robot_id;
	uint8_t robot_level; //机器人等级 1：一级；2：二级：3：三级。
	uint16_t remain_HP;  //机器人剩余血量
	uint16_t max_HP;     //机器人上限血量
	uint16_t shooter_id1_17mm_cooling_rate; //机器人1号17mm枪口每秒冷却值
	uint16_t shooter_id1_17mm_cooling_limit;//机器人1号17mm枪口热量上限
	uint16_t shooter_id1_17mm_speed_limit;  //机器人1号17mm枪口上限速度 m/s
	
	uint16_t shooter_id2_17mm_cooling_rate; //机器人2号17mm枪口每秒冷却值
	uint16_t shooter_id2_17mm_cooling_limit;//机器人2号17mm枪口每秒冷却值
	uint16_t shooter_id2_17mm_speed_limit;  //机器人2号17mm枪口上限速度 m/s

	uint16_t shooter_id2_42mm_cooling_rate;//机器人42mm枪口每秒冷却值
	uint16_t shooter_id2_42mm_cooling_limit;//机器人42mm枪口热量上限
	uint16_t shooter_id2_42mm_speed_limit;//机器人42mm枪口上限速度 m/s 
	
	uint16_t chassis_power_limit;  //机器人底盘功率限制
	uint8_t mains_power_gimbal_output:1;  //0bt:gmba口输出：1为有24V输出，0为无24v输出：                                     
	uint8_t mains_power_chassis_output:1; // 1bit:chassis口输出：1为有24V输出，0为无24v输出：
	uint8_t mains_power_shooter_output:1; //2bit:shooter口输出：1为有24V输出，0为无24v输出：
} ext_game_robot_state_t;


//实时功率热量数据 命令码ID 0x0202  发送频率：50Hz 
typedef __packed struct
{
	uint16_t chassis_volt;  //底盘输出电压 毫伏
	uint16_t chassis_current; //底盘输出电流 毫安
	float chassis_power;  //底盘输出功率 W
	uint16_t chassis_power_buffer; //底盘功率缓冲  J 飞坡根据规则增加至250J
	uint16_t shooter_id1_17mm_cooling_heat; //1号17mm 枪口热量
	
	uint16_t shooter_id2_17mm_cooling_heat; //2号17mm 枪口热量
	
	uint16_t shooter_id1_42mm_cooling_heat; //42mm 枪口热量
} ext_power_heat_data_t;

//机器人位置 命令码ID 0x0203  发送频率：10Hz 
typedef __packed struct
{
	float x; // m
	float y;
	float z; 
	float yaw;  //度
}ext_game_robot_pos_t;

//机器人增益 命令码ID 0x0204  发送频率：1Hz 
typedef __packed struct
{
  uint8_t power_rune_buff;//bit0：机器人血量补血状态 bit1：枪口热量冷却加速 bit2：机器人防御加成 bit3：机器人攻击加成 其他bt保留
}ext_buff_t;

//空中机器人能量状态 命令码ID 0x0205  发送频率：10Hz
typedef __packed struct
{
  uint8_t attack_time;//可攻击时间 s。 30s递减
}aerial_robot_energy_t;


//伤害状态 命令码ID 0x0206  发送频率：伤害发生后发送
/*
bit0-3 当血量变化类型为装甲伤害，代表装甲ID，其中数值0-4号代表机器人的五个装甲片
，其他血量变化类型，该变量数值为0。
bit4-7 血量变化类型
0x0 装甲模块扣血
0x1 模块掉线扣血
0x2 超射速扣血
0x3 超枪口热量扣血
0x4 超底盘功率扣血
0x5 装甲撞击扣血
*/
typedef __packed struct
{
uint8_t armor_id : 4;
uint8_t hurt_type : 4;
} ext_robot_hurt_t;

//实时射击信息 命令码ID 0x0207  发送频率：射击后发送
typedef __packed struct
{
  uint8_t bullet_type;//子弹类型1:17mm弹丸 2：42mm弹丸
	uint8_t shooter_id;//发射机构ID 1:1号17mm发射机构 2:2号17mm发射机构 3:42mm发射机构
	uint8_t bullet_freq;//子弹射频  HZ
	float bullet_speed;//子弹射速 m/s
}ext_shoot_data_t;



//子弹剩余发射数 命令码ID 0x0208  发送频率：10Hz周期发送，所有机器人发送
typedef __packed struct
{
	uint16_t bullet_remaining_num_17mm;
	uint16_t bullet_remaining_num_42mm;
	uint16_t coin_remaining_num;//剩余金币数量
}ext_bullet_remaining_t;


//机器人RFID状态 命令码ID 0x0209  发送频率：1Hz 发送范围：单一机器人
/*
bit0 基地增益点RFID状态
bit1 高地增益点RFID状态
bit2 能量机关增益点RFID状态
bit3 飞坡增益点RFID状态
bit4 前哨岗增益点RFID状态
bit5 资源岛增益点RFID状态
bit6 补血点增益点RFID状态
bit7 工程机器人补血点RFID状态
*/
typedef __packed struct
{
  uint32_t rfid_status;
}ext_rfid_status_t;


//飞镖机器人客户端指令数据 命令码ID 0x020A  发送频率：10Hz 发送范围：单一机器人

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







/*用户自定义部分显示在屏幕上*/
/*0-1 数据内容ID 0xD180
	2-3 发送者的ID
*/

/* 
	
	交互数据，包括一个统一的数据段头结构，
	包含了内容 ID，发送者以及接受者的 ID 和内容数据段，
	整个交互数据的包总共长最大为 128 个字节，
	减去 frame_header,cmd_id,frame_tail 以及数据段头结构的 6 个字节，
	故而发送的内容数据段最大为 113。
	整个交互数据 0x0301 的包上行频率为 10Hz。

	机器人 ID：
	1，英雄(红)；
	2，工程(红)；
	3/4/5，步兵(红)；
	6，空中(红)；
	7，哨兵(红)；
	11，英雄(蓝)；
	12，工程(蓝)；
	13/14/15，步兵(蓝)；
	16，空中(蓝)；
	17，哨兵(蓝)。 
	客户端 ID： 
	0x0101 为英雄操作手客户端( 红) ；
	0x0102 ，工程操作手客户端 ((红 )；
	0x0103/0x0104/0x0105，步兵操作手客户端(红)；
	0x0106，空中操作手客户端((红)； 
	0x0111，英雄操作手客户端(蓝)；
	0x0112，工程操作手客户端(蓝)；
	0x0113/0x0114/0x0115，操作手客户端步兵(蓝)；
	0x0116，空中操作手客户端(蓝)。 
*/
/* 交互数据接收信息：0x0301  */

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

//机器人之间相互通信
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
}SelfDefineInfo_t;   //学生上传自定义数据 (0x0005)  

 


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

