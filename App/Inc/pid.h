#ifndef __PID_H__
#define __PID_H__

#include "include.h"
#include "common.h"

#define	DEGREE_MAX    120     //������ƫ��
#define M_QUITE_AREA  7     //����ƫ��ľ�̬������������ʹ�þ�̬����
#define M_BACK1       3.5      //�������ߵ��ٶ�Ϊ1��ʱ
#define M_BACK1_SPEED 3      //�����ٶ�Ϊ1��
#define M_BACK2       2
#define M_BACK2_SPEED 2
#define M_BACK_SPEED  1
#define M_BUFF_AREA   12
#define M_BUFF_SPEED  2

#define STEER_KP     3.0 
#define STEER_KI     4.0
#define STEER_KD     3.0


typedef enum Speed_mode
{
	STOP_SPEED,
	LOW_SPEED,
	MIDDLE_SPEED,
	HIGH_SPEED,
	CHECH,
	OTHER
}Speed_mode;

extern Speed_mode car_mode;
extern float steer_engine_degree;        //���ת���ĽǶ�
extern int motor_speed;                //���ת��


extern void Steer_Pid();
void Control_core();





#endif  