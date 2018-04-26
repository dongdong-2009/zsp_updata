#ifndef _LCD_SHOW_H_
#define _LCD_SHOW_H_

#include "common.h"
#include "include.h"

#define IMG_MODE 1
#define UI_MODE  2
#define ITEM_NUM 3

#define MAX_OPTION 30 //lcd��ʹ�õ����Ĳ�������


typedef struct Screen_Data //���νṹ��
{
	char *data_name;  //��������
	float *data_value; //����ֵ
	float icrement;   //�����ۼ��� �����100����on\off
}Screen_Data;




/*����lcd����*/
typedef struct Lcd_State //״̬�ṹ��
{
	struct Lcd_State *(*press_M)(struct Lcd_State *pThis); //����ָ�룬press_Mָ��һ������ֵ��Lcd_Stateָ�����ͣ�������Lcd_Stateָ�����͵ĺ���
	struct Lcd_State *(*press_U)(struct Lcd_State *pThis); //ָ���Ͻ�����ʱ�Ĵ�����
	struct Lcd_State *(*press_D)(struct Lcd_State *pThis);
	struct Lcd_State *(*press_L)(struct Lcd_State *pThis);
	struct Lcd_State *(*press_R)(struct Lcd_State *pThis);
}Lcd_State;

void onpress_M(); //�м䰴��ʱ�Ĵ�����
void onpress_U();
void onpress_D();
void onpress_L();
void onpress_R();

extern Lcd_State *p_current_state;//ָ��ǰ״̬��ָ��

Lcd_State *quit_Lcd(Lcd_State *pThis);
Lcd_State *goto_Begin(Lcd_State *pThis);
Lcd_State *ignore_Oprate(Lcd_State *pThis);
Lcd_State *goto_Set(Lcd_State *pThis);
Lcd_State *goto_Wait(Lcd_State *pThis);
Lcd_State *goto_next(Lcd_State *pThis);
Lcd_State *data_Down(Lcd_State *pThis);
Lcd_State *data_Up(Lcd_State *pThis);
Lcd_State *goto_Before(Lcd_State *pThis);

extern Lcd_State wait_middle, wait_begin, wait_end, normal_page; //lcd�ȴ�����ʱ��4��״̬


extern int page;
extern int current_row;
extern Screen_Data screen_data[];

extern int _temp;
extern uint8 key_on;
extern uint8 lcd_mode;
extern uint8 ui_point;
extern int colour[];
extern int ui_data[];
extern Site_t tem_site_str[];
extern Site_t tem_site_data[];



void PORTD_IRQHandler();
void UI_INIT();
void Open_UI(int n);



#endif