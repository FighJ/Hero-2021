#include "vofa.h"

Vofa_t Vofa;

//��ʼ��֡β����
void Vofa_tail_init(void)
{
	Vofa.tail[0] = 0x00;
	Vofa.tail[1] = 0x00;
	Vofa.tail[2] = 0x80;
	Vofa.tail[3] = 0x7F;
}

//����ı�ͨ������ ֻ�轫channelֵ�ı�
void Vofa_send_justfloat(float *send_message)
{
	//��ÿ��ͨ�����в���
	for(uint8_t count1 = 0; count1 <= (channel - 1); count1 ++ )  
	{
		Float_to_Byte(send_message[count1], Vofa.temp);

		//����ֺ���ֽڴ���buf����
		for(uint8_t count2 = 0; count2 <= 3; count2 ++ )  
		{
			Vofa.message[(count1 * 4) + count2] = Vofa.temp[count2];
		}
	}
	
	//��֡β���
	for(uint8_t count3 = 0; count3 <= 3; count3 ++)  
	{
		Vofa.message[channel * 4 + count3] = Vofa.tail[count3];
	}
	
}

//��������fת��Ϊ4���ֽ����ݴ����byte[4]�� 
void Float_to_Byte(float f,unsigned char byte[])  
{  
    FloatLongType Float;  
    Float.float_data=f;  
	
    byte[0]=(unsigned char)Float.long_data;  
    byte[1]=(unsigned char)(Float.long_data>>8);  
    byte[2]=(unsigned char)(Float.long_data>>16);  
    byte[3]=(unsigned char)(Float.long_data>>24);  
}  
