#include "include.h"

uint8 lcd_mode = IMG_MODE;
uint8 key_on = 0;
int colour[MAX_OPTION]; //0Ԫ��Ҳ��������Ч����
Site_t tem_site_str[] = { 0, 0, 0, 20, 0, 40, 0, 60, 0, 80, 0, 100 };
Site_t tem_site_data[] = { 60, 0, 60, 20, 60, 40, 60, 60, 60, 80, 60, 100};

int page = 1;  //lcd��ǰ����ҳ
int current_row = 0; //��ǰ������

/*----------����״̬�¶�Ӧ��5�����Ĳ���--------*/
Lcd_State wait_middle = {
	quit_Lcd,        //�� �˳�lcd,��ʾͼ��
	goto_Begin,      //��
	goto_Begin,      //��
	ignore_Oprate,   //��
	ignore_Oprate    //��
};
Lcd_State wait_begin = {
	goto_Set,        //�� 
	goto_Wait,       //��
	goto_next,       //��
	data_Down,       //��
	data_Up          //��
};
Lcd_State wait_end = {
	goto_Set,        //�� �˳�lcd,��ʾͼ��
	goto_Before,     //��
	goto_Wait,       //��
	data_Down,       //��
	data_Up          //��
};
Lcd_State normal_page = {
	goto_Set,        //�� �˳�lcd,��ʾͼ��
	goto_Before,     //��
	goto_next,       //��
	data_Down,       //��
	data_Up          //��
};

Lcd_State *p_current_state = &wait_middle;


void PORTD_IRQHandler()
{
	uint32 flag;

	while (!PORTD_ISFR);
	flag = PORTD_ISFR;
	PORTD_ISFR = ~0;                                   //���жϱ�־λ


	if (IMG_MODE == lcd_mode)
	{
		if (flag & (1 << 13))  lcd_mode = UI_MODE;         //����м����£������uiģʽ //LCD_clear(WHITE);												  
	}
	else
	{
		if (flag & (1 << 13))   //�м�����
		{
			onpress_M();
		}
		else if (flag & (1 << 10))
		{
			onpress_U();
		}
		else if (flag & (1 << 14))
		{
			onpress_D();
		}
		else if (flag & (1 << 11))
		{
			onpress_L();
		}
		else if (flag & (1 << 12))
		{
			onpress_R();
		}
	}
	key_on = 1;
	disable_irq(PORTD_IRQn);
	//else
	//{
	//	if (flag & (1 << 13))   
	//	{
	//		if (ui_point)
	//		{
	//			if (GREEN == colour[ui_point])   //����ɫ�ı�
	//			{
	//				colour[ui_point] = RED;
	//			}
	//			else
	//			{
	//				colour[ui_point] = GREEN;
	//			}
	//		}
	//		else
	//		{
	//			lcd_mode = IMG_MODE;
	//		}
	//	}
	//	else if (flag & (1 << 12))  //�Ҽ�����
	//	{
	//		if (ui_point)
	//		{
	//			if (RED == colour[ui_point])  //����Ϊ��ɫ��ʾ���ԸĶ�Ӧֵ
	//			{
	//				ui_data[ui_point]++;
	//			}
	//		}
	//	}
	//	else if (flag & (1 << 11))  //�������
	//	{
	//		if (ui_point)
	//		{
	//			if (RED == colour[ui_point])  //����Ϊ��ɫ��ʾ���ԸĶ�Ӧֵ
	//			{
	//				ui_data[ui_point]--;
	//				if (ui_data[ui_point] < 0) ui_data[ui_point] = 0;
	//			}
	//		}
	//	}
	//	else if (flag & (1 << 10)) //�ϼ�����
	//	{
	//		if (GREEN == colour[ui_point] && ui_point != 0)
	//		{
	//			if (1 == ui_point)
	//			{
	//				colour[ui_point] = WHITE;
	//				ui_point--;
	//			}
	//			else
	//			{
	//				colour[ui_point] = WHITE;
	//				ui_point--;
	//				colour[ui_point] = GREEN;
	//			}
	//		}
	//	}
	//	else if (flag & (1 << 14)) //�¼�����
	//	{
	//		if (GREEN == colour[ui_point] && ui_point != ITEM_NUM)
	//		{
	//			if (0 == ui_point)
	//			{
	//				ui_point++;
	//				colour[ui_point] = GREEN;
	//			}
	//			else
	//			{
	//				colour[ui_point] = WHITE;
	//				ui_point++;
	//				colour[ui_point] = GREEN;
	//			}
	//		}

	//	}
	//	else {}

	//}
	//key_on = 1;
	//disable_irq(PORTD_IRQn);
}


void Open_UI(int n)
{
	int i = 0;
	int m = 0;
	
	colour[MAX_OPTION - 1] = 300*n; //��¼Ҫ��������ݸ���
	if (1 == key_on)//�а������²�ˢ��
	{
		m = 6 * (page - 1);
		LCD_clear(WHITE);//������ֹ�ϴ����²�Ӱ
		for (i = 0; i < 6; i++)
		{
			if (m + i >= n)
			{
				break;
			}
			if (99 == screen_data[m + i].icrement)
			{
				LCD_str(tem_site_str[i], screen_data[m + i].data_name, BLACK, colour[m + i]);  //�ǵû�������ɫ
				LCD_str(tem_site_data[i], (((int)(*(screen_data[m + i].data_value))) % 2 )? "ON" : "OFF", BLACK, WHITE);
			}
			else
			{
				LCD_str(tem_site_str[i], screen_data[m + i].data_name, BLACK, colour[m + i]);//�ǵû�������ɫ
				LCD_numf(tem_site_data[i], (float)(*(screen_data[m + i].data_value)), BLACK, WHITE);
			}
		}
		key_on = 0;
		i = 100000;
		while (i--);
		enable_irq(PORTD_IRQn);
	}
	//if (1 == key_on)
	//{
	//	LCD_clear(WHITE);
	//	LCD_str(tem_site_str[0], "dgree:", BLACK, colour[1]);
	//	LCD_str(tem_site_str[1], "ANG:", BLACK, colour[2]);
	//	LCD_str(tem_site_str[2], "STA:", BLACK, colour[3]);
	//	LCD_num(tem_site_data[0], _temp, BLACK, WHITE);
	//	//LCD_num(tem_site_data[0], quad_val, BLACK, WHITE);
	//	LCD_num(tem_site_data[1], ui_data[2], BLACK, WHITE);
	//	LCD_num(tem_site_data[2], ui_data[3], BLACK, WHITE);
	//	LCD_num(tem_site_data[3], temp_s[7], BLACK, WHITE);
	//	key_on = 0;
	//	enable_irq(PORTD_IRQn);
	//}
}


void UI_INIT()
{
	int i = 0;

	for (i = 0; i < MAX_OPTION; i++)
	{
		colour[i] = WHITE;
	}
	/*���ø����������ж�*/
	port_init(PTD14, ALT1 | IRQ_FALLING | PULLUP);  //��
	port_init(PTD13, ALT1 | IRQ_FALLING | PULLUP);  //��
	port_init(PTD12, ALT1 | IRQ_FALLING | PULLUP);      //��
	port_init(PTD11, ALT1 | IRQ_FALLING | PULLUP);      //��
	port_init(PTD10, ALT1 | IRQ_FALLING | PULLUP);      //��
	enable_irq(PORTD_IRQn);                //ʹ��d��Ӧ�Ķ˿�Ҳ���ǰ�����port
}





/*-----------------�������ܵĺ���-----------------*/
Lcd_State *quit_Lcd(Lcd_State *pThis) //�˳�lcdģʽ
{
	page = 1;
	current_row = 0;
	lcd_mode = IMG_MODE;
	return &wait_middle;
}

Lcd_State *goto_Begin(Lcd_State *pThis) //�ӵȴ�ģʽ���뱾ҳ��һ��
{
	current_row = 1;
	colour[6 * (page - 1) + current_row - 1] = GREEN; //ѡ�е��б����ɫ
	if (1 == page) return &wait_begin;
	else return &normal_page;
	
}

Lcd_State *ignore_Oprate(Lcd_State *pThis)
{
	return pThis;
}

Lcd_State *goto_Set(Lcd_State *pThis)
{
	if (GREEN == colour[6 * (page - 1) + current_row - 1]) colour[6 * (page - 1) + current_row - 1] = RED;
	else colour[6 * (page - 1) + current_row - 1] = GREEN;
	return pThis;
}

Lcd_State *goto_Wait(Lcd_State *pThis)
{
	if (GREEN == colour[6 * (page - 1) + current_row - 1]) //��δѡ�е�����²�����
	{
		colour[6 * (page - 1) + current_row - 1] = WHITE;
		return &wait_middle;
	}
	else return pThis;	
}

Lcd_State *goto_next(Lcd_State *pThis)
{
	int i = 0;

	if (GREEN == colour[6 * (page - 1) + current_row - 1])
	{
		colour[6 * (page - 1) + current_row - 1] = WHITE;
		if (1 == colour[MAX_OPTION - 1] / 300 || 6 * (page - 1) + current_row - 1 == ((colour[MAX_OPTION - 1] / 300) - 1)) //���ֻ��һ������Ҫ������ߵ�ǰҳ��ֻ��һ��
		{
			return &wait_middle;
		}
		if (6 * (page - 1) + current_row - 1 == ((colour[MAX_OPTION - 1] / 300) - 2)) //���������һ�о������һ��
		{
			i = 1;
		}
		if (6 == current_row)
		{
			page++;
			current_row = 1;
		}
		else current_row++;
		colour[6 * (page - 1) + current_row - 1] = GREEN;
		if(1==i) return &wait_end;
		else return &normal_page;
	}
	else return pThis;
}

Lcd_State *goto_Before(Lcd_State *pThis)
{
	int i = 0;

	if (GREEN == colour[6 * (page - 1) + current_row - 1])
	{
		colour[6 * (page - 1) + current_row - 1] = WHITE;
		if (1 == colour[MAX_OPTION - 1] / 300 || 6 * (page - 1) + current_row - 1 == 0)
		{
			return &wait_middle;
		}
		if (6 * (page - 1) + current_row - 1 == 1)
		{
			i = 1;
		}
		if (1 == current_row)
		{
			page--;
			current_row = 6;
		}
		else current_row--;
		colour[6 * (page - 1) + current_row - 1] = GREEN;
		if (1 == i) return &wait_begin;
		else return &normal_page;
	}
	else return pThis;
}

Lcd_State *data_Down(Lcd_State *pThis)
{
	if (RED == colour[6 * (page - 1) + current_row - 1])
	{
		*(screen_data[6 * (page - 1) + current_row - 1].data_value) -= screen_data[6 * (page - 1) + current_row - 1].icrement;
	}
	return pThis;
}

Lcd_State *data_Up(Lcd_State *pThis)
{
	if (RED == colour[6 * (page - 1) + current_row - 1])
	{
		*(screen_data[6 * (page - 1) + current_row - 1].data_value) += screen_data[6 * (page - 1) + current_row - 1].icrement;
	}
	return pThis;
}


void onpress_M()
{
	p_current_state = p_current_state->press_M(p_current_state);
}

void onpress_U()
{
	p_current_state = p_current_state->press_U(p_current_state);
}

void onpress_D()
{
	p_current_state = p_current_state->press_D(p_current_state);
}

void onpress_L()
{
	p_current_state = p_current_state->press_L(p_current_state);
}

void onpress_R()
{
	p_current_state = p_current_state->press_R(p_current_state);
}

