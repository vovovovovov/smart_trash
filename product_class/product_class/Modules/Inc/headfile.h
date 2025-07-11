#ifndef __HEADFILE_H_
#define __HEADFILE_H_

#include "ultra_sound.h"
#include "schedule.h"
#include "beep.h"
#include "bsp_dht11.h"
#include "core_delay.h" 

// oled
#include "bsp_iic_debug.h"
#include "bsp_oled_debug.h"
//#include "bsp_oled_codetab.h"
#include "bsp_systick.h"

#include "SG90.h"
#include "TB6612.h"

#include "gpio.h"
#include "tim.h"
#include "stdint.h"
#include "i2c.h"
#include "Connectivity_Protocal.h"
#include "string.h"
extern uint8_t  Transmit_Data[64];
extern Connectivity_Protocal_Struct Receive_data;
extern Connectivity_Protocal_Struct Transmit_data;

#endif
