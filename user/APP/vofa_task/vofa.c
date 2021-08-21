#include "vofa.h"

Vofa_t Vofa;

//初始化帧尾数据
void Vofa_tail_init(void)
{
	Vofa.tail[0] = 0x00;
	Vofa.tail[1] = 0x00;
	Vofa.tail[2] = 0x80;
	Vofa.tail[3] = 0x7F;
}

//如需改变通道数量 只需将channel值改变
void Vofa_send_justfloat(float *send_message)
{
	//对每个通道进行操作
	for(uint8_t count1 = 0; count1 <= (channel - 1); count1 ++ )  
	{
		Float_to_Byte(send_message[count1], Vofa.temp);

		//将拆分后的字节存入buf数组
		for(uint8_t count2 = 0; count2 <= 3; count2 ++ )  
		{
			Vofa.message[(count1 * 4) + count2] = Vofa.temp[count2];
		}
	}
	
	//对帧尾打包
	for(uint8_t count3 = 0; count3 <= 3; count3 ++)  
	{
		Vofa.message[channel * 4 + count3] = Vofa.tail[count3];
	}
	
}

//将浮点数f转化为4个字节数据存放在byte[4]中 
void Float_to_Byte(float f,unsigned char byte[])  
{  
    FloatLongType Float;  
    Float.float_data=f;  
	
    byte[0]=(unsigned char)Float.long_data;  
    byte[1]=(unsigned char)(Float.long_data>>8);  
    byte[2]=(unsigned char)(Float.long_data>>16);  
    byte[3]=(unsigned char)(Float.long_data>>24);  
}  
