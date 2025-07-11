/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
extern unsigned char RX_USART_1[64];
extern uint16_t RX_USART_1_LEN;
extern uint16_t RX_FLAG;


extern uint8_t  myUsbRxData[64];   // ���յ�������
extern uint16_t myUsbRxNum ;
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_RED_Pin GPIO_PIN_1
#define LED_RED_GPIO_Port GPIOA
#define LED_GREEN_Pin GPIO_PIN_2
#define LED_GREEN_GPIO_Port GPIOA
#define LED_BLUE_Pin GPIO_PIN_3
#define LED_BLUE_GPIO_Port GPIOA
#define TRIG_Pin GPIO_PIN_15
#define TRIG_GPIO_Port GPIOA
#define ECHO_Pin GPIO_PIN_3
#define ECHO_GPIO_Port GPIOB
#define BEEP_SIG_Pin GPIO_PIN_9
#define BEEP_SIG_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
