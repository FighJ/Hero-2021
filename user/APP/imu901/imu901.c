#include "imu901.h"
#include "uart_blt566.h"

/* 模块主动上传的数据(串口解析后) */
attitude_t attitude;		/*!< 姿态角 */
quaternion_t quaternion;
gyroAcc_t gyroAccData;
mag_t magData;
baro_t baroData;
ioStatus_t iostatus;

/* 模块寄存器参数值 */
regValue_t imu901Param;

/* 串口接收解析成功的数据包 */
atkp_t rxPacket;

/* 陀螺仪加速度量程表 */
const uint16_t gyroFsrTable[4] = {250, 500, 1000, 2000};
const uint8_t accFsrTable[4] = {2, 4, 8, 16};

/**
  * @brief  接收串口数据解包流程
  */
static enum
{
	waitForStartByte1,
	waitForStartByte2,
	waitForMsgID,
	waitForDataLength,
	waitForData,
	waitForChksum1,
	
} rxState = waitForStartByte1;

/**
  * @brief  imu901模块串口数据解析（串口接收的每一个数据需传入处理）
  *	@note	此函数需要实时调用
  * @param  ch: 串口接收的单个数据
  * @retval uint8_t: 0 无包 1 有效包
  */
uint8_t imu901_unpack(uint8_t ch)
{
	static uint8_t cksum = 0, dataIndex = 0;

	switch (rxState)
	{
		case waitForStartByte1:
				if (ch == UP_BYTE1)
				{
						rxState = waitForStartByte2;
						rxPacket.startByte1 = ch;
				}

				cksum = ch;
				break;

		case waitForStartByte2:
				if (ch == UP_BYTE2 || ch == UP_BYTE2_ACK)
				{
						rxState = waitForMsgID;
						rxPacket.startByte2 = ch;
				}
				else
				{
						rxState = waitForStartByte1;
				}

				cksum += ch;
				break;

		case waitForMsgID:
				rxPacket.msgID = ch;
				rxState = waitForDataLength;
				cksum += ch;
				break;

		case waitForDataLength:
				if (ch <= ATKP_MAX_DATA_SIZE)
				{
						rxPacket.dataLen = ch;
						dataIndex = 0;
						rxState = (ch > 0) ? waitForData : waitForChksum1;	/*ch=0,数据长度为0，校验1*/
						cksum += ch;
				}
				else
				{
						rxState = waitForStartByte1;
				}

				break;

		case waitForData:
				rxPacket.data[dataIndex] = ch;
				dataIndex++;
				cksum += ch;

				if (dataIndex == rxPacket.dataLen)
				{
						rxState = waitForChksum1;
				}

				break;

		case waitForChksum1:
				if (cksum == ch)	/*!< 校准正确返回1 */
				{
						rxPacket.checkSum = cksum;

						return 1;
				}
				else	/*!< 校验错误 */
				{
						rxState = waitForStartByte1;
				}

				rxState = waitForStartByte1;
				break;

		default:
				rxState = waitForStartByte1;
				break;
	}

  return 0;
}

/**
  * @brief  ATKP数据包解析
  * @param  packet: atkp数据包
  * @retval None
  */
void atkpParsing(atkp_t *packet)
{
	/* 姿态角 */
	if (packet->msgID == UP_ATTITUDE)
	{
			int16_t data = (int16_t) (packet->data[1] << 8) | packet->data[0];
			attitude.roll = (float) data / 32768 * 180;

			data = (int16_t) (packet->data[3] << 8) | packet->data[2];
			attitude.pitch = (float) data / 32768 * 180;

			data = (int16_t) (packet->data[5] << 8) | packet->data[4];
			attitude.yaw = (float) data / 32768 * 180;
	}

//	/* 四元数 */
//	else if (packet->msgID == UP_QUAT)
//	{
//			int16_t data = (int16_t) (packet->data[1] << 8) | packet->data[0];
//			quaternion.q0 = (float) data / 32768;

//			data = (int16_t) (packet->data[3] << 8) | packet->data[2];
//			quaternion.q1 = (float) data / 32768;

//			data = (int16_t) (packet->data[5] << 8) | packet->data[4];
//			quaternion.q2 = (float) data / 32768;

//			data = (int16_t) (packet->data[7] << 8) | packet->data[6];
//			quaternion.q3 = (float) data / 32768;
//	}

//	/* 陀螺仪加速度数据 */
//	else if (packet->msgID == UP_GYROACCDATA)
//	{
//			gyroAccData.acc[0] = (int16_t) (packet->data[1] << 8) | packet->data[0];
//			gyroAccData.acc[1] = (int16_t) (packet->data[3] << 8) | packet->data[2];
//			gyroAccData.acc[2] = (int16_t) (packet->data[5] << 8) | packet->data[4];

//			gyroAccData.gyro[0] = (int16_t) (packet->data[7] << 8) | packet->data[6];
//			gyroAccData.gyro[1] = (int16_t) (packet->data[9] << 8) | packet->data[8];
//			gyroAccData.gyro[2] = (int16_t) (packet->data[11] << 8) | packet->data[10];

//			gyroAccData.faccG[0] = (float)gyroAccData.acc[0] / 32768 * 4; 		/*!< 4代表4G，上位机设置好的量程 */
//			gyroAccData.faccG[1] = (float)gyroAccData.acc[1] / 32768 * 4;
//			gyroAccData.faccG[2] = (float)gyroAccData.acc[2] / 32768 * 4;
//		
//			gyroAccData.fgyroD[0] = (float)gyroAccData.gyro[0] / 32768 * 2000; 	/*!< 2000代表2000°/S，上位机设置好的量程 */
//			gyroAccData.fgyroD[1] = (float)gyroAccData.gyro[1] / 32768 * 2000;
//			gyroAccData.fgyroD[2] = (float)gyroAccData.gyro[2] / 32768 * 2000;
//	}

//	/* 磁场数据 */
//	else if (packet->msgID == UP_MAGDATA)
//	{
//			magData.mag[0] = (int16_t) (packet->data[1] << 8) | packet->data[0];
//			magData.mag[1] = (int16_t) (packet->data[3] << 8) | packet->data[2];
//			magData.mag[2] = (int16_t) (packet->data[5] << 8) | packet->data[4];

//			int16_t data = (int16_t) (packet->data[7] << 8) | packet->data[6];
//			magData.temp = (float) data / 100;
//	}

//	/* 气压计数据 */
//	else if (packet->msgID == UP_BARODATA)
//	{
//			baroData.pressure = (int32_t) (packet->data[3] << 24) | (packet->data[2] << 16) |
//													(packet->data[1] << 8) | packet->data[0];

//			baroData.altitude = (int16_t) (packet->data[5] << 8) | packet->data[4];

//			int16_t data = (int16_t) (packet->data[7] << 8) | packet->data[6];
//			baroData.temp = (float) data / 100;
//	}

//	/* 端口状态数据 */
//	else if (packet->msgID == UP_D03DATA)
//	{
//			iostatus.d03data[0] = (uint16_t) (packet->data[1] << 8) | packet->data[0];
//			iostatus.d03data[1] = (uint16_t) (packet->data[3] << 8) | packet->data[2];
//			iostatus.d03data[2] = (uint16_t) (packet->data[5] << 8) | packet->data[4];
//			iostatus.d03data[3] = (uint16_t) (packet->data[7] << 8) | packet->data[6];
//	}
}
