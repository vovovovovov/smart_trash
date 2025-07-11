//Connectivity_Protocal.c
#include "headfile.h"

//帧头：2字节

//数据长度：2字节

//命令：1字节

//数据：（最多56字节）

//CRC16校验码：2字节

//帧尾：1字节

void Struct_To_Data(Connectivity_Protocal_Struct *the_Connectivity_Protocal_Struct,uint8_t *Target)
{
	Target[0] = the_Connectivity_Protocal_Struct->head[0];
	Target[1] = the_Connectivity_Protocal_Struct->head[1];
	
	Target[2] = the_Connectivity_Protocal_Struct->length[0];
	Target[3] = the_Connectivity_Protocal_Struct->length[1];
	
	Target[4] = the_Connectivity_Protocal_Struct->cmd;
	
	for(int i = 0 ; i < 56 ; i ++)
	{
		Target[5+i] = the_Connectivity_Protocal_Struct->data[i];
	}
	Target[61] = the_Connectivity_Protocal_Struct->judgement[0];
	Target[62] = the_Connectivity_Protocal_Struct->judgement[1];
	Target[63] = the_Connectivity_Protocal_Struct->back;
	
}
void Data_To_Struct(Connectivity_Protocal_Struct *the_Connectivity_Protocal_Struct,uint8_t *Target)
{
	the_Connectivity_Protocal_Struct->head[0] = Target[0];
	the_Connectivity_Protocal_Struct->head[1] = Target[1];
	
	the_Connectivity_Protocal_Struct->length[0] = Target[2];
	the_Connectivity_Protocal_Struct->length[1] = Target[3];
	
	the_Connectivity_Protocal_Struct->cmd = Target[4];
	
	for(int i = 0 ; i < 56 ; i ++)
	{
		the_Connectivity_Protocal_Struct->data[i] = Target[5+i];
	}
	the_Connectivity_Protocal_Struct->judgement[0] = Target[61];
	the_Connectivity_Protocal_Struct->judgement[1] = Target[62];
	the_Connectivity_Protocal_Struct->back = Target[63];
}

void Set_Struct(Connectivity_Protocal_Struct *the_Connectivity_Protocal_Struct,uint8_t cd)
{
	the_Connectivity_Protocal_Struct->head [0]= 0xa5;
	the_Connectivity_Protocal_Struct->head [1]= 0;//表示为mcu发出的信息
	
	the_Connectivity_Protocal_Struct->length[0] = '0';//暂时不用
	the_Connectivity_Protocal_Struct->length[1] = '0';//暂时不用
	
	the_Connectivity_Protocal_Struct->cmd = cd;//命令类型 COMMOND RESEND REQUIRE
	uint8_t judege0=0 , judege1=0;
	for(int i = 0 ; i < 56 ; i ++)
	{
		judege0 = (judege0 + the_Connectivity_Protocal_Struct->data[i]%2)%2;
	}
	for(int i = 0 ; i < 56 ; i ++)
	{
		judege0 = (judege0 + the_Connectivity_Protocal_Struct->data[i]%2)%2;
	}
	for(int i = 0 ; i < 56 ; i ++)
	{
		judege1 = (judege1 + the_Connectivity_Protocal_Struct->data[i]%3)%3;
	}
	the_Connectivity_Protocal_Struct->judgement[0] = judege0 ;
	the_Connectivity_Protocal_Struct->judgement[1] = judege1 ;
	the_Connectivity_Protocal_Struct->back = 'o';
}

void float2u8Arry(uint8_t *u8Arry, float *floatdata)
{
    uint8_t farray[4];
    *(float *)farray = *floatdata;
    if (1)
    {
        u8Arry[3] = farray[0];
        u8Arry[2] = farray[1];
        u8Arry[1] = farray[2];
        u8Arry[0] = farray[3];
    }
    else
    {
        u8Arry[0] = farray[0];
        u8Arry[1] = farray[1];
        u8Arry[2] = farray[2];
        u8Arry[3] = farray[3];
    }
}
//规定前8为外设对象
void Set_Data_Float(Connectivity_Protocal_Struct *the_Connectivity_Protocal_Struct,uint8_t *waishe ,float *datamath ,uint8_t number)
{
	for(int i = 0 ; i < 56 ; i ++)
	{
		the_Connectivity_Protocal_Struct->data[i] = 0;
	}
	uint8_t u8Arry[4];
	//memcpy(the_Connectivity_Protocal_Struct->data , waishe , 8);
	for(int i = 0 ; i < number ; i ++)
	{
		float2u8Arry(u8Arry,datamath);
		memcpy(the_Connectivity_Protocal_Struct->data  +4 * i, u8Arry , 4);
	}

}

void Set_Data_uint8_t(Connectivity_Protocal_Struct *the_Connectivity_Protocal_Struct,uint8_t *waishe ,uint8_t *datamath,uint8_t number,uint8_t start)
{
//	for(int i = 0 ; i < 56 ; i ++)
//	{
//		the_Connectivity_Protocal_Struct->data[i] = 0;
//	}
	uint8_t u8Arry[4];
	//memcpy(the_Connectivity_Protocal_Struct->data , waishe , 8);
	//float2u8Arry(u8Arry,datamath);
	//memcpy(the_Connectivity_Protocal_Struct->data + 8 , u8Arry , 4);
	memcpy(the_Connectivity_Protocal_Struct->data + start  , datamath , number);
	
}


















