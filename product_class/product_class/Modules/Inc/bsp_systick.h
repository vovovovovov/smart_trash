#ifndef __BSP_SYSTICK_H
#define __BSP_SYSTICK_H

#include "stm32f1xx.h"

void Systick_Init(void);
void Delay_1ms(__IO uint32_t nTime);
void TimingDelay_Decrement(void);


#endif // !__BSP_SYSTICK_H
