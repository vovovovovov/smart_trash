#ifndef __SCHEDULE_H__
#define __SCHEDULE_H__

#include <stdint.h>


// ����ṹ��
struct TaskStruct
{
	uint16_t	TaskTickNow;      //���ڼ�ʱ
	uint16_t	TaskTickMax;      //���ü�ʱʱ��
	uint8_t	TaskStatus;       //�������б�־λ
	void (*FC)();         //������ָ��
};

extern struct TaskStruct TaskST[];	//����Ϊ�ṹ�����ݣ������������ʱ�������

//�����������ĺ�������
void PeachOSRun(void);
void OS_IT_RUN(void);


//����ʾ��������������
void RED_Led(void);
void GREEN_Led(void);


#endif
