#include "headfile.h"


void beep_on(void)
{
	HAL_GPIO_WritePin(GPIOB, BEEP_SIG_Pin, GPIO_PIN_SET);
}

void beep_off(void)
{
	HAL_GPIO_WritePin(GPIOB, BEEP_SIG_Pin, GPIO_PIN_RESET);
}