#ifndef __ULTRA__SOUND_H_
#define __ULTRA__SOUND_H_

#include "main.h"


// ????????????
void CS100A_TRIG_START(void);

typedef struct 
{
	uint32_t start_time;
	uint32_t end_time;
	float pulse_width;
	float distance ;
}ultra_sound_struct;

extern ultra_sound_struct ultra_sound;

void Get_distance(void);



#endif

