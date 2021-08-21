#ifndef UART_BLT566_H
#define UART_BLT566_H
#include "main.h"

//extern uint32_t data_message;//Êý¾ÝÎ»
extern uint8_t blt566_sum_check_result;

#define UART8_BUFF_SIZE 9  //BLT566=14 C=9

extern uint8_t Blt566_receive_temp_buf[UART8_BUFF_SIZE];
extern uint8_t Blt566_receive_buf[UART8_BUFF_SIZE];

extern void UART8_Blt566_Config(void);
extern void Blt566_Data_handle(void);

extern void Blt566_Cache(void);
extern void Blt566_Sum_Check(void);

#endif
