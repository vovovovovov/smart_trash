#include "headfile.h"

// ���������ģ��
ultra_sound_struct ultra_sound = {0,0,0,0};



// ���͸ߵ�ƽ
void CS100A_TRIG_START(void)
{
	HAL_GPIO_WritePin(GPIOA, TRIG_Pin, GPIO_PIN_SET);
	// ����ʱ��72MHz��72��ָ������Ϊ1΢�룬һ��forѭ��Ϊ8��ָ������ ���Դ��10΢������Ҫ90��ѭ��
	for(uint16_t i = 0; i < 100 ; i++);
	HAL_GPIO_WritePin(GPIOA, TRIG_Pin, GPIO_PIN_RESET);
}

void Get_distance()
{
	// ���������
	__HAL_TIM_SET_COUNTER(&htim2, 0);
	HAL_TIM_IC_Start(&htim2, TIM_CHANNEL_1);    // ��Ӳ����½���
	HAL_TIM_IC_Start(&htim2, TIM_CHANNEL_2);    // ֱ�Ӳ���������
	CS100A_TRIG_START();
	// �ȴ������ز���
	while(__HAL_TIM_GET_FLAG(&htim2, TIM_FLAG_CC2) == RESET);
	// ��������ر�־λ
	__HAL_TIM_CLEAR_FLAG(&htim2, TIM_FLAG_CC2);
	ultra_sound.start_time = HAL_TIM_ReadCapturedValue(&htim2, TIM_CHANNEL_2);
	// �ȴ��½��ز���
	while(__HAL_TIM_GET_FLAG(&htim2, TIM_FLAG_CC1) == RESET);
	// ��������ر�־λ
	__HAL_TIM_CLEAR_FLAG(&htim2, TIM_FLAG_CC1);
	ultra_sound.end_time = HAL_TIM_ReadCapturedValue(&htim2, TIM_CHANNEL_1);
	// ������
	uint32_t delte = ultra_sound.end_time - ultra_sound.start_time;
	ultra_sound.pulse_width = (float)delte * 1e-6 ;  // ��λ���룩
	ultra_sound.distance = ultra_sound.pulse_width * 340.0 / 2.0f;
}
