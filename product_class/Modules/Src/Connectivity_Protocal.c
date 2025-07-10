//Connectivity_Protocal.c
#include "headfile.h"

//帧头：2字节

//数据长度：2字节

//命令：1字节

//数据：可变长度（最多56字节）

//CRC16校验码：2字节

//帧尾：1字节

void Transmit_To_Data(Connectivity_Protocal_Struct *the_Connectivity_Protocal_Struct,uint8_t *Target,uint16_t length)
{
	Target[0] = the_Connectivity_Protocal_Struct->head[0];
	Target[1] = the_Connectivity_Protocal_Struct->head[1];
	
}
