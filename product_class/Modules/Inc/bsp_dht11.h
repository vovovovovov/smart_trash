#ifndef __DHT11_H
#define	__DHT11_H



#include "stm32f1xx.h"


/************************** DHT11 �������Ͷ���********************************/
typedef struct
{
	uint8_t  humi_int;		//ʪ�ȵ���������
	uint8_t  humi_deci;	 	//ʪ�ȵ�С������
	uint8_t  temp_int;	 	//�¶ȵ���������
	uint8_t  temp_deci;	 	//�¶ȵ�С������
	uint8_t  check_sum;	 	//У���
		                 
} DHT11_Data_TypeDef;



/************************** DHT11 �������Ŷ���********************************/
#define      DHT11_Dout_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOB_CLK_ENABLE()

#define      DHT11_Dout_GPIO_PORT                      GPIOB
#define      DHT11_Dout_GPIO_PIN                       GPIO_PIN_0



/************************** DHT11 �����궨��********************************/
#define      DHT11_Dout_0	                            HAL_GPIO_WritePin ( DHT11_Dout_GPIO_PORT, DHT11_Dout_GPIO_PIN, GPIO_PIN_RESET ) 
#define      DHT11_Dout_1	                            HAL_GPIO_WritePin ( DHT11_Dout_GPIO_PORT, DHT11_Dout_GPIO_PIN, GPIO_PIN_SET ) 

#define      DHT11_Dout_IN()	                        HAL_GPIO_ReadPin ( DHT11_Dout_GPIO_PORT, DHT11_Dout_GPIO_PIN ) 



/************************** DHT11 �������� ********************************/
void                     DHT11_Init                      ( void );
uint8_t                  DHT11_Read_TempAndHumidity      ( DHT11_Data_TypeDef * DHT11_Data );

;

#endif /* __DHT11_H */







