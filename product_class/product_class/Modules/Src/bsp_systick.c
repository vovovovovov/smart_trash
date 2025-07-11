/**
 ******************************************************************************
 * @file    main.c
 * @author  fire
 * @version V1.0
 * @date    2024-xx-xx
 * @brief   systick
 ******************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:Ұ�� F103 STM32 ������
 * ��̳    :http://www.firebbs.cn
 * �Ա�    :https://fire-stm32.taobao.com
 *
 ******************************************************************************
 */
#include "headfile.h"

static __IO uint32_t TimingDelay;

void Systick_Init(void)
{
    /* SystemFrequency / 1000    1ms
     * SystemFrequency / 100000	 10us
     * SystemFrequency / 1000000 1us
     */
    if (HAL_SYSTICK_Config(SystemCoreClock / 1000))
    {
        /* Capture error */
        while (1)
            ;
    }
}

void Delay_1ms(__IO uint32_t nTime)
{
    TimingDelay = nTime;

    while (TimingDelay != 0)
        ;
}

void TimingDelay_Decrement(void)
{
    if (TimingDelay != 0x00)
    {
        TimingDelay--;
    }
}
