#ifndef VOFA_H
#define VOFA_H

#include "main.h"

#define channel 7

typedef struct
{
	uint8_t message[(channel + 1) * 4];  // ���� = ͨ���� * ��ÿ�����ݵ��ֽ�����4�� +  ʶ��λ��4����
	uint8_t temp[4];  //���ڻ�������
	uint8_t tail[4];  //ʶ��λ

}Vofa_t;

typedef union  
{  
    float float_data;  
    unsigned long long_data;  

}FloatLongType; 

extern Vofa_t Vofa;

void Vofa_tail_init(void);

void Vofa_send_justfloat(float *send_message);
void Float_to_Byte(float f,unsigned char byte[]);
	
#endif
