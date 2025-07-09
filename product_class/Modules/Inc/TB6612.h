
#ifndef __TB6612_H_
#define __TB6612_H_
#include "gpio.h"
//请在这里修改pwm的总计数
#define PWM_ALL_TB6612 23999
#define TB6612_TIM htim3
#define TB6612_TIM_CHANNEL_A TIM_CHANNEL_3
#define TB6612_TIM_CHANNEL_B TIM_CHANNEL_4
#define TB6612_TIM_COUNTER_PERIOD PWM_ALL_TB6612
#define TB6612_PORT_A GPIOA
#define TB6612_PORT_B GPIOB
#define TB6612_PORT_A_IN_1 GPIO_PIN_4
#define TB6612_PORT_A_IN_2 GPIO_PIN_5
#define TB6612_PORT_B_IN_1 GPIO_PIN_13
#define TB6612_PORT_B_IN_2 GPIO_PIN_14
#define TB6612_PORT GPIOA
#define TB6612_PORT_PIN GPIO_PIN_7
#define TB6612_WORK GPIO_PIN_SET
#define TB6612_OVER GPIO_PIN_RESET
#define TB6612_STOP 0
#define TB6612_UP 1
#define TB6612_DOWN 2




void TB6612_SET(GPIO_TypeDef* GPIOx,uint16_t GPIO_PIN,GPIO_PinState PinState);

void TB6612_SET_DIRECTION(GPIO_TypeDef* GPIOx,uint16_t TB6612_State);

void TB6612_SET_START(TIM_HandleTypeDef *htim, uint32_t Channel);

void TB6612_SET_SPEED(TIM_HandleTypeDef *htim, uint32_t Channel , uint16_t degree);

//void beep_on(void);
//void beep_off(void);


#endif
