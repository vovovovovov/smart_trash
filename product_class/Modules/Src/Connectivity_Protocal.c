//Connectivity_Protocal.c
#include "headfile.h"

//֡ͷ��2�ֽ�

//���ݳ��ȣ�2�ֽ�

//���1�ֽ�

//���ݣ��ɱ䳤�ȣ����56�ֽڣ�

//CRC16У���룺2�ֽ�

//֡β��1�ֽ�

void Transmit_To_Data(Connectivity_Protocal_Struct *the_Connectivity_Protocal_Struct,uint8_t *Target,uint16_t length)
{
	Target[0] = the_Connectivity_Protocal_Struct->head[0];
	Target[1] = the_Connectivity_Protocal_Struct->head[1];
	
}
