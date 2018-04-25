#ifndef _searchroad_H_
#define _searchroad_H_

#include "common.h"
#include "include.h"

#define	LINE_NUM         60      //��Ҫɨ�������
#define BLANK_LINE_NUM   0       //��ʼʱ�����ʼ�ļ���
#define OFF_CENTER       5       //��һ��ɨ��ƫ����һ�����ĵ����ظ���
#define DEGREE_OF_ERROR  5       //��������֮��������߲�ֵ������ֵʱ�����ݽ��ᱻ����
#define CLOSE_TO_EDGE    5       //�����߽ӽ�ͼ��ı�Ե������ɨ��                             ���ֵ�������б���ĺ�ҲҪ����

#define CAMERA_HARDWARE_ERROR  4          //����ͷ��ƫ�����ߵ��µ�Ӳ��ƫ��,��ƫΪ��
#define TRAPEZOID_CORRECT   0.35     //���ν��� ��ֵԽ���������Խ��
#define ROAD_WIDE	        39    //·���һ��
//#define DIP_CORRECT         1.414   //��ǽ���

/*���µĺ�����Ժ��ò���*/
#define ROAD_BEGIN       65      //����ROAD_BEGIN��ROAD_END֮�����δɨ�赽�����ж���ת��
#define ROAD_END         45
#define TURN_DEGREE      13      //
#define REFLECT_LIGHT_AREA   30  //��ⷴ�������ӵ�40�п�ʼ
#define REFLECT_LIGHT_ERROR  4   //���ڷ��⵼�µ�����ƫ�����ֵʱ��֮�������������


extern uint8 img[][CAMERA_W];     //��Ž�ѹ��ͼ�������
extern uint8 imgbuff[];        //����洢����ͼ�������
extern float average_offset[11];

extern int iscross_flag;      //�ж��Ƿ���ʮ�ֵ��ı�־
extern int iscross_count;     //ʮ�ֵ��������

extern int isisland_flag;     //�Ƿ��ǻ������ж� 0��ʾû�н���
extern int isisland_flag1;
extern int isisland_count;    //�����������

extern float temp_s[10];

//extern int rode_wide[CAMERA_H];




extern  void Search_line_init();
extern  void Search_line();
extern float Find_slope();
extern void Negation();
extern void nomal_middle(int left_black[], int right_black[], int middleline[]);

#endif