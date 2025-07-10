#ifndef Connectivity_Protocal_H
#define Connectivity_Protocal_H

#include "main.h"

//帧头：2字节

//数据长度：2字节

//命令：1字节

//数据：可变长度（最多56字节）

//CRC16校验码：2字节

//帧尾：1字节

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

