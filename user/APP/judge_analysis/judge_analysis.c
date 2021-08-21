#define __DRIVER_GLOBALS
#include "judge_analysis.h"
#include "string.h"
#include "crc_check.h"
void judgeCalculate(uint8_t JudgeDataBuffer[JudgeBufferLength])//裁判系统解算
{	
	static uint16_t start_pos=0,next_start_pos=0;
	while(1)
	{
		memcpy(&JudgementData.frameHeader, &JudgeDataBuffer[start_pos],FrameHeader_Len);
		/*先校验头帧0xA5 然后crc8校验帧头 再crc16位校验整包*/
		if((JudgementData.frameHeader.SOF==(uint16_t)JudgeFrameHeader)&&(1==Verify_CRC8_Check_Sum(&JudgeDataBuffer[start_pos],FrameHeader_Len))&&(1==Verify_CRC16_Check_Sum(&JudgeDataBuffer[start_pos], JudgementData.frameHeader.DataLenth+FrameHeader_Len+4)))//数据位长度+帧头长度+命令码长度+校验码长度
		{
			memcpy(&JudgementData.rxCmdId, (&JudgeDataBuffer[start_pos]+5), sizeof(JudgementData.rxCmdId));
			JudgeDataBuffer[start_pos]++;//每处理完一次就在帧头加一防止再次处理这帧数据
			next_start_pos=start_pos+9+JudgementData.frameHeader.DataLenth;//9为 5位帧头 2位数据长度 2校验位
			
			switch(JudgementData.rxCmdId)
			{
				case ID_game_state://读取比赛状态数据 001
				{
					memcpy(&JudgementData.GameState,(&JudgeDataBuffer[start_pos]+7),JudgementData.frameHeader.DataLenth);//把数组中的数据复制到对应的结构体中去
				}
				break;
				case ID_game_result://读取比赛结果 002
				{
					memcpy(&JudgementData.GameResult,(&JudgeDataBuffer[start_pos]+7),JudgementData.frameHeader.DataLenth);//把数组中的数据复制到对应的结构体中去
				}
				break;
				case ID_game_robot_HP://读取机器人血量数据003
				{
					memcpy(&JudgementData.GameRobotHP,(&JudgeDataBuffer[start_pos]+7),JudgementData.frameHeader.DataLenth);//把数组中的数据复制到对应的结构体中去
				}
				break;
				case ID_event_data://读取场地事件 101
				{
					memcpy(&JudgementData.EventData,(&JudgeDataBuffer[start_pos]+7),JudgementData.frameHeader.DataLenth);//把数组中的数据复制到对应的结构体中去
				}
				break;
				case ID_supply_projectile_action://读取补给站数据 102
				{
					memcpy(&JudgementData.SupplyProjectileAction,(&JudgeDataBuffer[start_pos]+7),JudgementData.frameHeader.DataLenth);//把数组中的数据复制到对应的结构体中去
				}
				break;
				case Robot_Status_ID://读取机器人等级血量等 201
				{
					memcpy(&JudgementData.game_robot_state_t,(&JudgeDataBuffer[start_pos]+7),JudgementData.frameHeader.DataLenth);//把数组中的数据复制到对应的结构体中去
				}
				break;
				case power_heat_data_ID://读取机器人枪口热量、底盘功率 202
				{	
					memcpy(&JudgementData.power_heat_data_t,(&JudgeDataBuffer[start_pos]+7),JudgementData.frameHeader.DataLenth);//把数组中的数据复制到对应的结构体中去
			  }					
				break;
				case game_robot_pos_ID://读取机器人位置数据 203
				{	
					memcpy(&JudgementData.GameRobotPos,(&JudgeDataBuffer[start_pos]+7),JudgementData.frameHeader.DataLenth);//把数组中的数据复制到对应的结构体中去
			  }					
				break;
				case robot_buff_ID://机器人增益数据 204
				{	
					memcpy(&JudgementData.Buff,(&JudgeDataBuffer[start_pos]+7),JudgementData.frameHeader.DataLenth);//把数组中的数据复制到对应的结构体中去
			  }					
				break;
				case aerial_robot_energy_ID://空中机器人能量状态数据 205
				{	
					memcpy(&JudgementData.AerialRobotEnergy,(&JudgeDataBuffer[start_pos]+7),JudgementData.frameHeader.DataLenth);//把数组中的数据复制到对应的结构体中去
			  }					
				break;
        case robot_hurt_ID://读取机器人伤害 206
				{
					memcpy(&JudgementData.robot_hurt_t,(&JudgeDataBuffer[start_pos]+7),JudgementData.frameHeader.DataLenth);//把数组中的数据复制到对应的结构体中去			
				}		
				break;
				case shoot_data_ID://读取机器人射频射速 207
				{
					memcpy(&JudgementData.shoot_data_t,(&JudgeDataBuffer[start_pos]+7),JudgementData.frameHeader.DataLenth);//把数组中的数据复制到对应的结构体中去			
				}		
				break;
			  case bullet_remaining_ID://读取子弹剩余发射数 208
				{
					memcpy(&JudgementData.Bulletrema,(&JudgeDataBuffer[start_pos]+7),JudgementData.frameHeader.DataLenth);//把数组中的数据复制到对应的结构体中去			
				}		
				break;
			  			
				case ID_student_interactive:
        {
					memcpy(&JudgementData.userinfo,(&JudgeDataBuffer[start_pos]+7),JudgementData.frameHeader.DataLenth);//把数组中的数据复制到对应的结构体中去			
				}
			}
			start_pos=next_start_pos;
		}
		else
		{
			start_pos=0;
			break;				
		}		
		/**如果头指针越界了退出循环**/
		if(start_pos>JudgeBufferLength)
		{
			start_pos=0;
			break;
		}
	}
}


uint8_t JudgeUARTtemp;
//Judge
void UART4_IRQHandler(void)
{
    JudgeUARTtemp = UART4->DR;
    JudgeUARTtemp = UART4->SR;
    
    DMA_Cmd(DMA1_Stream2, DISABLE);
			
	  judgeCalculate(JudgeDataBuffer);
		
    //重启DMA
    DMA_ClearFlag(DMA1_Stream2, DMA_FLAG_TCIF2 | DMA_FLAG_HTIF2);
    while(DMA_GetCmdStatus(DMA1_Stream2) != DISABLE);
    DMA_SetCurrDataCounter(DMA1_Stream2, JudgeBufferLength);
    DMA_Cmd(DMA1_Stream2, ENABLE);
}

void USART4_SendBuff (uint8_t *buf,uint16_t len)
{
  int i;
	for(i=0; i<len;i++)
	{
    USART_SendData(UART4,*((u8*)&buf+i));
	}
while( USART_GetFlagStatus(UART4 ,USART_FLAG_TC) == RESET );
        
}

tFrame Frame = {.FrameHeader.SOF = 0xA5};
uint8_t tx_buf[200];
void Send_FrameData(uint16_t cmdid, uint8_t * pchMessage,uint16_t dwLength)
{	
	uint16_t frame_length = HEADER_LEN + CMD_LEN + dwLength + CRC_LEN;
	
	FrameHeader *p_header = (FrameHeader*)tx_buf;
  
  p_header->SOF          = 0xA5;
  p_header->DataLenth   = dwLength;
  p_header->Seq          = 0;
	 
	memcpy(&tx_buf[HEADER_LEN], (uint8_t*)&cmdid, CMD_LEN); 
  Append_CRC8_Check_Sum(tx_buf, HEADER_LEN);  
  memcpy(&tx_buf[HEADER_LEN + CMD_LEN], pchMessage, dwLength);
  Append_CRC16_Check_Sum(tx_buf, frame_length);
	
  USART4_SendBuff((uint8_t *)&tx_buf,frame_length);               //帧发送	
} 


extern uint8_t client_custom_Send[28]={0xA5,0x13,0,0,0x6D,  0x01,0x03,  0x80,0xD1,   0x00,0x00,  0x00,0x00,  0,0,0,0, 0,0,0,0, 0,0,0,0, 0,    0,0};
extern uint8_t robot_custom_Send[28]={0xA5,0x13,0,0,0x6D,  0x01,0x03,  0x00,0x02,   0x00,0x00,  0x00,0x02,  0,0,0,0, 0,0,0,0, 0,0,0,0, 0,    0,0};
	
void client_send(uint8_t * data)
{
		JudgementData.userinfo.id.send_id=JudgementData.game_robot_state_t.robot_id;
	
		if(JudgementData.game_robot_state_t.robot_id>10)
			JudgementData.userinfo.id.receive_id=0x0106+JudgementData.userinfo.id.send_id;
		else
			JudgementData.userinfo.id.receive_id=0x0100+JudgementData.userinfo.id.send_id;


		JudgementData.userinfo.data3=8;

		memcpy(data,&JudgementData.userinfo,28);
		Append_CRC16_Check_Sum(data,28);
		USART4_SendBuff(data,28);
}

void robot_send(uint8_t * data)
{
		JudgementData.robot_data_t.id.send_id=JudgementData.game_robot_state_t.robot_id;
		if(JudgementData.game_robot_state_t.robot_id<10)
			JudgementData.robot_data_t.id.receive_id=3;
		else 
			JudgementData.robot_data_t.id.receive_id=17;		
		memcpy(data,&JudgementData.robot_data_t,28);
		Append_CRC16_Check_Sum(data,28);
		USART4_SendBuff(data,28);
}

void client_init(uint8_t * data1,uint8_t * data2)
{
		memcpy(&JudgementData.userinfo,data1,28);
		memcpy(&JudgementData.robot_data_t,data2,28);	
}

//void get_chassis_power_and_buffer(float *power, float *buffer, uint16_t *powmax)
//{
//    *power = JudgementData.power_heat_data_t.chassis_power;
//    *buffer = JudgementData.power_heat_data_t.chassis_power_buffer;
//		*powmax = JudgementData.game_robot_state_t.chassis_power_limit;
//}
