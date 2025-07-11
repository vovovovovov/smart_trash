#include "headfile.h"

// 超声波检测模块
ultra_sound_struct ultra_sound = {0,0,0,0};



// 发送高电平
void CS100A_TRIG_START(void)
{
	HAL_GPIO_WritePin(GPIOA, TRIG_Pin, GPIO_PIN_SET);
	// 软延时，72MHz，72个指令周期为1微秒，一个for循环为8个指令周期 所以大概10微秒大概需要90个循环
	for(uint16_t i = 0; i < 100 ; i++);
	HAL_GPIO_WritePin(GPIOA, TRIG_Pin, GPIO_PIN_RESET);
}

void Get_distance()
{
	// 清零计数器
	__HAL_TIM_SET_COUNTER(&htim2, 0);
	HAL_TIM_IC_Start(&htim2, TIM_CHANNEL_1);    // 间接捕获，下降沿
	HAL_TIM_IC_Start(&htim2, TIM_CHANNEL_2);    // 直接捕获，上升沿
	CS100A_TRIG_START();
	// 等待上升沿捕获
	while(__HAL_TIM_GET_FLAG(&htim2, TIM_FLAG_CC2) == RESET);
	// 清除上升沿标志位
	__HAL_TIM_CLEAR_FLAG(&htim2, TIM_FLAG_CC2);
	ultra_sound.start_time = HAL_TIM_ReadCapturedValue(&htim2, TIM_CHANNEL_2);
	// 等待下降沿捕获
	while(__HAL_TIM_GET_FLAG(&htim2, TIM_FLAG_CC1) == RESET);
	// 清除上升沿标志位
	__HAL_TIM_CLEAR_FLAG(&htim2, TIM_FLAG_CC1);
	ultra_sound.end_time = HAL_TIM_ReadCapturedValue(&htim2, TIM_CHANNEL_1);
	// 计数差
	uint32_t delte = ultra_sound.end_time - ultra_sound.start_time;
	ultra_sound.pulse_width = (float)delte * 1e-6 ;  // 单位（秒）
	ultra_sound.distance = ultra_sound.pulse_width * 340.0 / 2.0f;
}
