#ifndef __SCHEDULE_H__
#define __SCHEDULE_H__

#include <stdint.h>


// 任务结构体
struct TaskStruct
{
	uint16_t	TaskTickNow;      //用于计时
	uint16_t	TaskTickMax;      //设置计时时间
	uint8_t	TaskStatus;       //任务运行标志位
	void (*FC)();         //任务函数指针
};

extern struct TaskStruct TaskST[];	//声明为结构体数据，那样多个任务时方便管理

//框架运行所需的函数声明
void PeachOSRun(void);
void OS_IT_RUN(void);


//用于示例的任务函数声明
void RED_Led(void);
void GREEN_Led(void);


#endif
