#include "headfile.h"


//void beep_on(void)
//{
//	HAL_GPIO_WritePin(GPIOB, BEEP_SIG_Pin, GPIO_PIN_SET);
//}

//void beep_off(void)
//{
//	HAL_GPIO_WritePin(GPIOB, BEEP_SIG_Pin, GPIO_PIN_RESET);
//}
void SG90_PWM_START(TIM_HandleTypeDef *htim, uint32_t Channel)
{
	HAL_TIM_PWM_Start(htim, Channel);
}

void SG90_PWM_CONTROL(TIM_HandleTypeDef *htim, uint32_t Channel , uint16_t degree)
{
	__HAL_TIM_SetCompare(htim,Channel,(uint16_t)(((float)degree/1800.0 +0.025)*SG90_TIM_COUNTER_PERIOD));
}
