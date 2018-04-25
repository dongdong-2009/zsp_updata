#include "searchroad.h"

#include "common.h"
#include "include.h"

      
float average_slope = 0;
float average_offset = 0;                          //ƫ��ȣ�Ϊƽ��ƫ���


void Search_line_init()
{
	//send_init();
	//motorcontrol_int();
}


void Search_line()
{
	int left_black[CAMERA_H];                          //��ߺ�������
	int left_black_before = CAMERA_W / 2;              //��һ�����ɨ�赽�ĺ���λ��
	int left_next_line = 0;                            //�ж�ɨ���Ƿ�������µ�һ��
	int left_find_flag = 0;                            //����Ƿ��ҵ����߱�־
	int left_slope[CAMERA_H] = { 0 };                  //��ƫ���
	int jw_left;                                       //���������ĵ�ǰ��

	int right_black[CAMERA_H];                         //�ұߺ�������
	int right_black_before = CAMERA_W / 2;
	int right_next_line = 0;
	int right_find_flag = 0;
	int right_slope[CAMERA_H] = { 0 };       
	int jw_right;

	int jh;                                            //�в���
	int middleline[CAMERA_H] = { 0 };                  //�洢����λ�õ�����
	int offset = 0;                                    //ƫ��ȣ�Ϊ����ƫ���
	int slope[CAMERA_H] = { 0 };                       //���ÿ�м����б�ȵ�����
	int count = 0;
	int sumslope = 0;                                  //б�ȵ��ܺ�   

	average_slope = 0;
	average_offset = 0;
	left_black_before = CAMERA_W / 2;
	right_black_before = CAMERA_W / 2;
	jh = (CAMERA_H - BLANK_LINE_NUM);                         //BLANK_LINE_NUMΪ10,��ʾ�����Ϸ���Ч���е���Ŀ

	while (jh>(CAMERA_H - LINE_NUM - BLANK_LINE_NUM))         //����������������ߣ���ÿһ��ͼ����ж�ɨ��һ��,LINE_NUM��ʾҪɨ������
	{
		if (0 == left_next_line)                              //left_black_next������־�Ƿ�����µ�һ�У���Ϊ0���ǽ����µ�һ��
		{
			jw_left = left_black_before;                      //�����µ�һ�п�ʼ����ϴ��ҵ����ߵ�λ�ÿ�ʼ�Һ���
			left_next_line = 1;                               //��־������λ
		}
			
		if (jw_left>0 && (0 == left_find_flag))               //���ɨ�軹û�е���Ե��֮ǰ��ɨ��δ�ҵ��ڵ�
		{
			//if (1> (img[jh][jw_left] + img[jh][jw_left-1]))  //�ڵ�Ϊ0��flagΪ0��û�ҵ���������������3���ڵ�����Ϊ�ҵ�����
			if ((img[jh][jw_left]) < 1)
			{
				left_find_flag = 1;                           //ɨ�赽���ߣ���־��1
				left_black[jh] = jw_left;                     //��������Ϊjw_left
				left_black_before = jw_left + OFF_CENTER;     //��һ��ɨ�����һ�κ��ߵ�����ƫ��15�����صĵط���ʼɨ��
			}
			jw_left--;                                        //��û��ɨ�赽���߾��ƶ�һ�����ؼ���ɨ��
		}
		if ((0 == jw_left) && (0 == left_find_flag))          //����ҵ����һ��������Ȼδ�ҵ����Ϻ������������أ�����Ϊ��һ��Ϊ����
		{
			left_find_flag = 1;                       
			left_black[jh] = jw_left;
			left_black_before = jw_left + OFF_CENTER+20;
		}

		/*���Ҳ��ɨ���Һڵ�*/
		if (0 == right_next_line)
		{
			jw_right = right_black_before;
			right_next_line = 1;
		}
			
		if ((jw_right< (CAMERA_W - 1)) && (0 == right_find_flag))            //��Ϊ��0λҲ�������ص㣬���Դ������ص�����һλ��CAMERA_W-1
		{
			//if ( (img[jh][jw_right]+ img[jh][jw_right+1])<1)                        //gai
			if ((img[jh][jw_right])<1)
			{
				right_find_flag = 1;
				right_black[jh] = jw_right;
				right_black_before = jw_right - OFF_CENTER-20;
			}
			jw_right++;
		}
		if (jw_right == (CAMERA_W - 1) && (0 == right_find_flag))            //�ұߺڵ�δ�ҵ�
		{
			right_find_flag = 1;
			right_black[jh] = jw_right;
			right_black_before = jw_right - OFF_CENTER;
		}


		if ((1 == left_find_flag) && (1 == right_find_flag))                 //���ߺ��߶��ҵ�ʱ��ʼ���㣬������ifʱ��Ϊһ���ҵ����߲��ܽ�����㣬Ҫ���߶��ҵ�
		{

			if (jh == (CAMERA_H - BLANK_LINE_NUM))
				middleline[jh] = (left_black[jh] + right_black[jh]) / 2;
			if (jh < (CAMERA_H - BLANK_LINE_NUM))
			{
				//right_slope[jh] = right_black[jh] - right_black[jh + 1];
				//left_slope[jh] = left_black[jh] - left_black[jh + 1];
				//middleline[jh] = middleline[jh + 1] + right_slope[jh] + left_slope[jh];     //����ƫ�Ѱ������
				middleline[jh] = (left_black[jh] + right_black[jh]) / 2;
				//img[jh][middleline[jh]] = !img[jh][middleline[jh]];

				if (middleline[jh + 1] - middleline[jh] > DEGREE_OF_ERROR || middleline[jh + 1] - middleline[jh] < -DEGREE_OF_ERROR) {} //����б�ʹ��󣨼�ת�䣩�����ϴ�flag��1

				else
				{
					slope[jh] = (middleline[jh + 1] - middleline[jh]);               //б��
					sumslope += slope[jh];
					offset = offset - (CAMERA_W / 2 - middleline[jh]);             //offset�ǲ�����������������������ƫ��
					count++;
					img[jh][middleline[jh]] = !img[jh][middleline[jh]];
				}
			}

			if ((middleline[jh]< CLOSE_TO_EDGE) || (middleline[jh]>(CAMERA_W - CLOSE_TO_EDGE))) //���߽ӽ�ͼ���Ե,�˳�ɨ��
			{
				jh = (CAMERA_H - LINE_NUM - BLANK_LINE_NUM + 1);
			}

			left_next_line = 0;                                                 //�����ع�
			right_next_line = 0;
			left_find_flag = 0;
			right_find_flag = 0;
			jh--;                                                             //������һ�е�ɨ��
		} //if  
	} //while

	average_slope = (sumslope * 100) / count;
	average_offset = (offset / count);
}