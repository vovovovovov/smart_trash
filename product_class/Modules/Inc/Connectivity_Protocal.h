#ifndef Connectivity_Protocal_H
#define Connectivity_Protocal_H

#include "main.h"

//֡ͷ��2�ֽ�

//���ݳ��ȣ�2�ֽ�

//���1�ֽ�

//���ݣ��ɱ䳤�ȣ����56�ֽڣ�

//CRC16У���룺2�ֽ�

//֡β��1�ֽ�

#define EXP8266_UART	huart1

typedef struct 
{
	uint8_t head[2];
	uint8_t length[2];
	uint8_t cmd;
	uint8_t data[56] ;
	uint8_t judgement[2];
	uint8_t back;
}Connectivity_Protocal_Struct;



#endif

