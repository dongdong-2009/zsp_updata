/*
����Ϊ1������Ϊ0
*/
#include "searchroad.h"
#include "common.h"
#include "include.h"


float average_offset[11] = { 0,0,0,0,0,0,0,0,0,0,0 };                 //ƫ��ȣ�Ϊƽ��ƫ���
uint8 imgbuff[CAMERA_SIZE];                                         //����洢����ͼ�������
uint8 img[CAMERA_H][CAMERA_W];

int iscross_flag = 0;            //�ж��Ƿ���ʮ�ֵ��ı�־ 0��ʾû�н���1��ʾ�����жϲ���ʼ��������
int iscross_count = 0;           //ʮ�ֵ��������

int isisland_flag = 0;           //�Ƿ��ǻ������ж� 0��ʾû�н���1��ʾ�����жϲ���ʼ��������
int isisland_flag1 = 0;
int isisland_count = 0;          //�����������



float temp_s[10] = { 0 };



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
	int jw_left;                                       //���������ĵ�ǰ��

	int right_black[CAMERA_H];                         //�ұߺ�������
	int right_black_before = CAMERA_W / 2;
	int right_next_line = 0;
	int right_find_flag = 0;
	int jw_right;

	int jh;                                            //�в���
	int middleline[CAMERA_H] = { 0 };                  //�洢����λ�õ�����
	float offset = 0;                                   //ƫ��ȣ�Ϊ����ƫ���
													   //int slope[CAMERA_H] = { 0 };                       //���ÿ�м����б�ȵ�����
	int count = 0;
	int i = 0;
	int j = 0;
	int m = 0;

	int cross_temp[2] = { -1,-1 };

	left_black_before = CAMERA_W / 2;
	right_black_before = CAMERA_W / 2;
	jh = LINE_NUM-1;


	/*-----------------------ɨ�����ص��ȡͼ����Ϣ------------------------*/
	/*
		1.������ֱ������Բ�����ж�
		2.��Ҫ���µ�������left_black��right_black���飬����-1��ʾ�˱�δ�ҵ����ߣ�-2��ʾ����Ϊ����ֹͣɨ�裨δ����ȫ�ڣ�ֻ�ǽ��ƺ��ߣ�
	*/
	while (jh>=0)                                             //����������������ߣ���ÿһ��ͼ����ж�ɨ��һ��,LINE_NUM��ʾҪɨ������
	{
		/*-----------------���ɨ��--------------------*/
		if (0 == left_next_line)                              //left_black_next������־�Ƿ�����µ�һ�У���Ϊ0���ǽ����µ�һ��
		{
			jw_left = left_black_before;                      //�����µ�һ�п�ʼ����ϴ��ҵ����ߵ�λ�ÿ�ʼ�Һ���
			left_next_line = 1;                               //��־������λ
		}

		if (jw_left > 0 && (0 == left_find_flag))              //���ɨ�軹û�е���Ե��֮ǰ��ɨ��δ�ҵ��ڵ�
		{
			if ((img[jh][jw_left]) < 1)                 //ɨ�赽�ڵ�
			{
				if (left_black_before == jw_left && jh < 30)       //�����һ��ɨ���ɨ���ڵ��ҵ�ǰ������Ļ���ϰ벿��
				{
					while (0 == img[jh][jw_left] && jw_left < CAMERA_W - 1)  //���������� �˴�����<=��Ϊ�����Ļ�jw_left++��ͻ�ʹ�������jw_leftʱ����Խ��
					{
						jw_left++;
					}
					if (CAMERA_W - 1 == jw_left)         //��������Ǻڵľ�ֹͣɨ��
					{
						left_black[jh] = -2;             //���֮����ж�Ϊ��Ч��
						right_black[jh] = -2;
						break;
					}
					else
					{
						left_find_flag = 1;                           //ɨ�赽���ߣ���־��1
						left_black[jh] = jw_left - 1;                   //��������Ϊjw_left
						left_black_before = jw_left + OFF_CENTER;     //��һ��ɨ�����һ�κ��ߵ�����ƫ��5�����صĵط���ʼɨ��
						if (left_black_before > CAMERA_W - 1) left_black_before = CAMERA_W - 1;
					}
				}
				else
				{
					left_find_flag = 1;                           //ɨ�赽���ߣ���־��1
					left_black[jh] = jw_left;                     //��������Ϊjw_left
					left_black_before = jw_left + OFF_CENTER;     //��һ��ɨ�����һ�κ��ߵ�����ƫ��5�����صĵط���ʼɨ��
					if (left_black_before > CAMERA_W - 1) left_black_before = CAMERA_W - 1;
				}
			}
			jw_left--;                                        //��û��ɨ�赽���߾��ƶ�һ�����ؼ���ɨ��
		}
		if ((0 == jw_left) && (0 == left_find_flag))          //����ҵ����һ��������Ȼδ�ҵ����Ϻ������������أ�����Ϊ��һ��Ϊ����
		{
			if (jh < 50 && left_black[jh + 1] != -1 )  //��ɨ���ж��Ƿ���Բ�� ���ǰһ������
			{
				if (left_black[jh + 1] - 2 >= 0 && left_black[jh + 1] + 3 <= CAMERA_W - 1)  //��ֹ�����img[jh + 1][left_black[jh + 1] - 2]����Խ��
				{
					if (1 == img[jh + 1][left_black[jh + 1] - 2] && 1 == img[jh + 1][left_black[jh + 1] + 3])  //�������һ�е�ͻ�����������
					{
						for (i = 2; i < 5; i++)  //����ɨ����5�п��Ƿ��������
						{
							j = left_black[jh + i] - 1; //��j��¼�����i�еı��������
							while (j > 0 && img[jh + i][j] != 1) //������0��1�����䣬������1������0
							{
								j--;
							}
							if (j <= 0) break; //ֻҪ�����i����һ�в�����Ҫ���ֱ�������������Ļ�i�Ͳ����ܵ���5���Ͳ������ӱ��
						}
						if (5 == i)
						{
							isisland_flag1++;
							//isisland_flag = 1;
						}
					}
				}
				else
				{
					for (i = 2; i < 5; i++)
					{
						j = left_black[jh + i] - 1;
						while (j > 0 && img[jh + i][j] != 1)
						{
							j--;
						}
						if (j <= 0) break;
					}
					if (5 == i)
					{
						isisland_flag1++;
						//isisland_flag = 1;
					}
				}
			}
			left_find_flag = 1;
			left_black[jh] = -1;                              //-1����ɨ�赽����ʱ��û�ҵ�����
			left_black_before = jw_left + OFF_CENTER;  
			if (left_black_before > CAMERA_W - 1) left_black_before = CAMERA_W - 1;
		}

		/*-----------------�Ҳ�ɨ��--------------------*/
		if (0 == right_next_line)
		{
			jw_right = right_black_before;
			right_next_line = 1;
		}

		if ((jw_right< (CAMERA_W - 1)) && (0 == right_find_flag))            //��Ϊ��0λҲ�������ص㣬���Դ������ص�����һλ��CAMERA_W-1
		{           
			if ((img[jh][jw_right])<1)
			{
				if (right_black_before == jw_right && jh < 30)
				{
					while(jw_right > 0 && 0 == img[jh][jw_right])
					{
						jw_right--;
					}
					if (0 == jw_right)
					{
						left_black[jh] = -2;
						right_black[jh] = -2;
						break;
					}
					else
					{
						right_find_flag = 1;
						right_black[jh] = jw_right + 1;
						right_black_before = jw_right - OFF_CENTER;
						if (right_black_before < 0) right_black_before = 0;
					}
				}
				else
				{
					right_find_flag = 1;
					right_black[jh] = jw_right;
					right_black_before = jw_right - OFF_CENTER;
					if (right_black_before < 0) right_black_before = 0;
				}
			}
			jw_right++;
		}
		if (jw_right == (CAMERA_W - 1) && (0 == right_find_flag))            //�ұߺڵ�δ�ҵ�
		{
			if (jh < 50 && right_black[jh + 1] != -1)                        //��ɨ���ж��Ƿ���Բ��
			{
				if (right_black[jh + 1] + 2 <= CAMERA_W - 1 && right_black[jh + 1] - 3 > 0)
				{
					if (1 == img[jh + 1][right_black[jh + 1] + 2] && 1 == img[jh + 1][right_black[jh + 1] - 3])
					{
						for (i = 2; i < 5; i++)
						{
							j = right_black[jh + i] + 1;
							while (j > 0 && j < CAMERA_W - 1 && img[jh + i][j] != 1)  //����Ҫ����߶��һ���жϣ���Ϊ��right_black[jh + i]ֵΪ-1ʱ
							{
								j++;
							}
							if (j >= (CAMERA_W - 1) || j <= 0) break;
						}
						if (5 == i)
						{
							isisland_flag1++;
							//isisland_flag = 1;
						}
					}	
				}
				else
				{
					for (i = 2; i < 5; i++)
					{
						j = right_black[jh + i] + 1;
						while (j > 0 && j < CAMERA_W - 1 && img[jh + i][j] != 1)  //����Ҫ����߶��һ���жϣ���Ϊ��right_black[jh + i]ֵΪ-1ʱ
						{
							j++;
						}
						if (j >= (CAMERA_W - 1) || j <= 0) break;
					}
					if (5 == i)
					{
						isisland_flag1++;
						//isisland_flag = 1;
					}
				}
			}
			right_find_flag = 1;
			right_black[jh] = -1;
			right_black_before = jw_right - OFF_CENTER;
			if (right_black_before < 0) right_black_before = 0;
		}	

		if ((1 == left_find_flag) && (1 == right_find_flag))           //�����ع�
		{
			left_next_line = 0;                                                
			right_next_line = 0;
			left_find_flag = 0;
			right_find_flag = 0;
			jh--;
		}
	} //while







	/*----------------����ͼ���ж�·��--------------------*/
	/*
		1.�ж����Բ�� ��isisland_flag �ڶ���λ��
		2.�ж�ʮ�ֵ� ��һ��λ��
	*/
	jh = LINE_NUM - 6;
	while (jh >= 5 && left_black[jh] != -2 && right_black[jh] != -2)
	{
		if (left_black[jh + 3] - left_black[jh + 4] > 0 &&           //�ж��Ƿ���Բ��
			left_black[jh + 2] - left_black[jh + 3] > 0 &&
			left_black[jh + 1] - left_black[jh + 2] > 0 &&
			left_black[jh + 0] - left_black[jh + 1] > 0 &&
			left_black[jh - 1] - left_black[jh - 0] < 0 &&
			left_black[jh - 2] - left_black[jh - 1] < 0 &&
			left_black[jh - 3] - left_black[jh - 2] < 0 &&
			left_black[jh - 4] - left_black[jh - 3] < 0
			)
		{
			
			if (left_black[jh + 0] - left_black[jh + 4] > 6 && left_black[jh + 0] - left_black[jh - 4] > 6)//ͻ���㹻����
			{
				isisland_flag++;
				//temp_s[0] = left_black[jh];
				//temp_s[1] = left_black[jh + 4];
				//temp_s[2] = left_black[jh - 4];
				//LCD_numf(tem_site_str[2], jh, GREEN, BLUE);
			}
		}

		j = 0;
		m = 0;
		if (-1 == left_black[jh] && -1 == right_black[jh] && iscross_flag!=2)   //�ж��Ƿ���ʮ�� iscross_flagΪ2ʱ�Ͳ���ʶ����
		{
			for (i = 1; i < 20; i++)
			{
				if (jh + i > LINE_NUM - 1) break;
				if (left_black[jh + i] != -1)
				{
					if (left_black[jh + i] > 1)
					{
						j = 1;
					}
					break;
				}
			}
			for (i = 1; i < 20; i++)
			{
				if (jh + i > LINE_NUM - 1) break;
				if (right_black[jh + i] != -1)
				{
					if (right_black[jh + i] < CAMERA_W - 2)
					{
						m = 1;
					}
					break;
				}
			}
			if (1 == j && 1 == m)
			{
				if (0 == iscross_flag)   //�����һ��ɨ�赽���������ͳ���Ϊ1
				{
					iscross_flag = 1;
				}
				else if (1 == iscross_flag && cross_distance_count > 4 && cross_distance_count < 8) //����Ѿ����гɹ���ʮ������4��8���׺�ĸ����гɹ��ж���ʮ���Ǿ���Ϊ��ʮ��
				{
					iscross_flag = 2;
				}
			}
		}
		jh--;
	}







	/*---------------------���ĵ�ļ���------------------------*/
	/*
		1.�������ұ߽�ȷ�����ĵ��λ��
		2.�����Ч���ĵ�
	*/
	jh = LINE_NUM - 1;

	if (2 == iscross_flag)
	{
		while (jh >= 0 && left_black[jh] != -2 && right_black[jh] != -2)
		{
			if (jh == (LINE_NUM - 1))
			{
				if (-1 == right_black[jh])
				{
					middleline[jh] = (left_black[jh] + CAMERA_W) / 2;
					cross_temp[1] = LINE_NUM - 1;   //��ʼ��ȫ���߾����ҵ��½�
				}	
				else if (-1 == left_black[jh])
				{
					middleline[jh] = (left_black[jh] + right_black[jh]) / 2;
					cross_temp[1] = LINE_NUM - 1;   //��ʼ��ȫ���߾����ҵ��½�
				}
				else
					middleline[jh] = (left_black[jh] + right_black[jh]) / 2;
			}
				
			else
			{
				/*�������濪ʼɨ�赱����һ�߶���ʱ��һֱ�������if��ʮ���Ͻ� */
				if ((-1 == left_black[jh] || -1 == right_black[jh] || cross_temp[1] != -1) && cross_temp[0] != -2)
				{
					if (-1 == cross_temp[1])
					{
						cross_temp[1] = jh + 1;   //��¼�����½�
						temp_s[5] = cross_temp[1];
						temp_s[7] = middleline[cross_temp[1]];
					}
					if (jh <= 0)  //δ�ҵ��Ͻ�
					{
						for (i = 0; i < cross_temp[1]; i++)
						{
							//middleline[i] = middleline[LINE_NUM - 1] + (middleline[cross_temp[1]] - middleline[LINE_NUM - 1])*(LINE_NUM - 1 -i)/(LINE_NUM - 1 - cross_temp[1]);
							middleline[i] = (CAMERA_W / 2 + middleline[cross_temp[1]]) / 2;
						}
					}
					else if (left_black[jh] != -1 && right_black[jh] != -1)
					{
						cross_temp[0] = jh;   //��¼�����Ͻ�
						temp_s[4] = cross_temp[0];
						
						middleline[jh] = (left_black[jh] + right_black[jh]) / 2;
						middleline[jh] += ((CAMERA_W / 2 - middleline[jh]) / 2);
						temp_s[6] = middleline[cross_temp[0]];
						for (i = cross_temp[0] + 1; i < cross_temp[1]; i++)
						{
							middleline[i] = middleline[cross_temp[1]] + (middleline[cross_temp[0]] - middleline[cross_temp[1]])*(i - cross_temp[1]) / (cross_temp[1] - cross_temp[0]);
							//middleline[i] = (middleline[cross_temp[0]] + middleline[cross_temp[1]]) / 2;
							//temp_s[9] = 10;
						}
						cross_temp[0] = -2; //���ڽ������if
					}
				}
				else
				{
					middleline[jh] = (left_black[jh] + right_black[jh]) / 2;
				}
			}
			jh--;
		}
		if (left_black[jh] == -2 || right_black[jh] != -2)
		{
			middleline[jh] = -2;
		}
	}
	else
	{
		while (jh >= 0 && left_black[jh] != -2 && right_black[jh] != -2)
		{
			if (jh == (LINE_NUM - 1))
			{
				if (-1 == right_black[jh])
					middleline[jh] = (left_black[jh] + CAMERA_W) / 2;
				else
					middleline[jh] = (left_black[jh] + right_black[jh]) / 2;
			}
			else
			{
				if (-1 == left_black[jh] || -1 == right_black[jh])
				{
					if (-1 == left_black[jh] && -1 == right_black[jh]) middleline[jh] = CAMERA_W / 2;
					else
					{
						if (-1 == right_black[jh]) middleline[jh] = left_black[jh] + 36 / (1 + (60 - jh)*TRAPEZOID_CORRECT / 40);
						else middleline[jh] = right_black[jh] - 36 / (1 + (60 - jh)*TRAPEZOID_CORRECT / 40);
					}
				}
				else
				{
					middleline[jh] = (left_black[jh] + right_black[jh]) / 2;
				}
			}
			jh--;
		}
		if (left_black[jh] == -2 || right_black[jh] != -2)
		{
			middleline[jh] = -2;
		}
	}
	






	/*--------------------------����·���µ�Ԥ������-------------------------*/
	//if (iscross_flag != 0)
	//{

	//}
	//else if (isisland_flag != 0)
	//{

	//}




    /*------------------------ƫ��ȼ��㼰����չ��-----------------------------*/
	/*
		1.�����ĵ����ƫ���
	*/
	for (i = LINE_NUM - 1; i >= 0; i--)
	{
		if (-2 == middleline[i])
			break;
		else if (-1 == middleline[i]) {}
		else
		{
			offset = offset + ((float)(middleline[i] - CAMERA_W / 2)*(1 + (60 - i)*TRAPEZOID_CORRECT / 40));          //offset�ǲ�����������������������ƫ��,<0ƫ��
			count++;
			if (middleline[i] > CAMERA_W - 1)middleline[i] = CAMERA_W - 1;
			if (middleline[i] < 0)middleline[i] = 0;
			img[i][middleline[i]] = !img[i][middleline[i]];
		}
	}





	/*---------------����ƫ��ȶ���---------------------*/
	for (i = 10; i > 1; i--)
	{
		average_offset[i] = average_offset[i - 1];
	}
	average_offset[1] = ((float)offset / (float)count);
	average_offset[1] -= CAMERA_HARDWARE_ERROR;
}


float Find_slope()       //�ҵ����ε�б��
{
	int j = 79;     //��ǰ��
	int h[3] = { 2,3,4 };

	while (0 == img[50][j] && j >= 0)
	{	
		j--;
	}
	h[1] = j;

	j = 79;
	while (0 == img[10][j])
	{
		j--;
	}
	h[2] = j;

	LCD_num(tem_site_str[3], h[1], GREEN, BLUE);
	LCD_num(tem_site_str[4], h[2], GREEN, BLUE);
	//LCD_numf(tem_site_data[4], h[1]-h[2], GREEN, BLUE);

	return (float)(abs(h[1] - h[2])) / 40.0;
}


void Negation()
{
	int i = 0;
	int j = 0;

	for (i = 0; i <CAMERA_H; i++)
	{
		for (j = 0; j <CAMERA_W; j++)
		{
			img[i][j] = !img[i][j];
		}
	}
}


