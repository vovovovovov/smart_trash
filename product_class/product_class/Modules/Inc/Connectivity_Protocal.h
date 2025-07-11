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
#define COMMOND 1
#define RESEND 2
#define REQUIRE 3

#define USE_SG90 'SG90_USE'//����������������� ����Ϊ8 �����ÿո�����
#define TEMP 'TEMP    '
#define ultra_sou (uint8_t*)"ultra_so"



typedef struct 
{
	uint8_t head[2];
	uint8_t length[2];
	uint8_t cmd;
	uint8_t data[56] ;
	uint8_t judgement[2];
	uint8_t back;
}Connectivity_Protocal_Struct;


void Struct_To_Data(Connectivity_Protocal_Struct *the_Connectivity_Protocal_Struct,uint8_t *Target);
void Data_To_Struct(Connectivity_Protocal_Struct *the_Connectivity_Protocal_Struct,uint8_t *Target);

void Set_Struct(Connectivity_Protocal_Struct *the_Connectivity_Protocal_Struct,uint8_t cd);
void float2u8Arry(uint8_t *u8Arry, float *floatdata);
//�涨ǰ8Ϊ�������
void Set_Data_Float(Connectivity_Protocal_Struct *the_Connectivity_Protocal_Struct,uint8_t *waishe ,float *datamath ,uint8_t number);

void Set_Data_uint8_t(Connectivity_Protocal_Struct *the_Connectivity_Protocal_Struct,uint8_t *waishe ,uint8_t *datamath,uint8_t number,uint8_t start);






#endif

