
#ifndef __SG90_H_
#define __SG90_H_
#include "tim.h"
//请在这里修改pwm的总计数
#define PWM_ALL 23999
#define SG90_TIM htim3
#define SG90_TIM_CHANNEL TIM_CHANNEL_1
#define SG90_TIM_COUNTER_PERIOD PWM_ALL

void SG90_PWM_START(TIM_HandleTypeDef *htim, uint32_t Channel);

void SG90_PWM_CONTROL(TIM_HandleTypeDef *htim, uint32_t Channel , uint16_t dushu);


//void beep_on(void);
//void beep_off(void);


#endif
