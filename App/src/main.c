/*����ͷ60*80����Ļ128*128*/
#include "common.h"
#include "include.h"


//��������
void PORTA_IRQHandler();
void DMA0_IRQHandler();

int _temp = 0;

Screen_Data screen_data[] = {
{ "speed0",&(temp_s[0]),0.3 },
{ "speed1",&(temp_s[1]),99 },
{ "speed2",&(temp_s[2]),2 },
{ "speed3",&(temp_s[3]),3 },
{ "speed4",&(temp_s[4]),4 },
{ "speed5",&(temp_s[5]),5 },
{ "speed6",&(temp_s[6]),6 },
{ "speed7",&(temp_s[7]),7 },
{ "speed8",&(temp_s[8]),8 },
{ "speed9",&(temp_s[9]),9 }
};

/*
ͼ��ѹ��ֱ����ʾ
*/
void  main(void)
{
	Site_t site = { 0, 0 };             
	Size_t size;  
	Size_t imgsize = { CAMERA_W, CAMERA_H };
	     
	size.H = LCD_H;
	size.W = LCD_W;
	//size.H = LCD_H/2;
	//size.W = LCD_W/2;
	//size.H = 60;
	//size.W = 80;

	LCD_init();
	camera_init(imgbuff);
    ftm_pwm_init(FTM0, FTM_CH5, 300, 0);
	ftm_pwm_init(FTM0, FTM_CH6, 300, 43);
	UI_INIT();

	set_vector_handler(PORTA_VECTORn, PORTA_IRQHandler);   //���� PORTA ���жϷ�����Ϊ PORTA_IRQHandler
	set_vector_handler(DMA0_VECTORn, DMA0_IRQHandler);     //���� DMA0 ���жϷ�����Ϊ PORTA_IRQHandler
	set_vector_handler(PORTD_VECTORn, PORTD_IRQHandler);   //ui�����жϵĳ�ʼ��
	Quad_Init();                                           //�������ж�
	
	

	while (1)
	{    
		if (IMG_MODE == lcd_mode)
		{
			//_temp++;
            Update_Motor();
			camera_get_img();                            //�����ȡͼ��                               
			img_extract(img, imgbuff, CAMERA_SIZE);      //��ѹͼ��
			//temp_s[6] = Find_slope();
			Search_line();                               //����
			Negation();
			img_compress(img, imgbuff, CAMERA_SIZE);     //ͼ��ѹ��
			LCD_Img_Binary_Z(site, size, imgbuff, imgsize);//lcd��ʾͼ��
			//LCD_numf(tem_site_str[2], temp_s[0], GREEN, BLUE);  
			//LCD_numf(tem_site_data[2], temp_s[1], GREEN, BLUE);
			
			//LCD_numf(tem_site_str[3], iscross_flag, GREEN, BLUE);
			//LCD_numf(tem_site_data[3], temp_s[7] , GREEN, BLUE);
			//LCD_numf(tem_site_str[4], cross_distance_count, GREEN, BLUE);
			
			Control_core();

			if (1 == key_on) enable_irq(PORTD_IRQn);     //������ж�
		}
		else Open_UI(10);


		//ftm_pwm_duty(FTM0, FTM_CH5, (int)motor_speed);                    //���
		ftm_pwm_duty(FTM0, FTM_CH5, 0);
		ftm_pwm_duty(FTM0, FTM_CH6, 380 + (int)steer_engine_degree);       //��� 
        //ftm_pwm_duty(FTM0, FTM_CH6, 390);
	}//while
}



/*!
*  @brief      PORTA�жϷ�������������ͷ�й�
*  @since      v5.0
*/
void PORTA_IRQHandler()
{
	uint8  n;    //���ź�
	uint32 flag;
	_temp++;
	while (!PORTA_ISFR);
	flag = PORTA_ISFR;
	PORTA_ISFR = ~0;                                   //���жϱ�־λ

	n = 29;                                             //���ж�
	if (flag & (1 << n))                                 //PTA29�����ж�
	{
		camera_vsync();
	}
#if ( CAMERA_USE_HREF == 1 )                            //ʹ�����ж�
	n = 28;
	if (flag & (1 << n))                                 //PTA28�����ж�
	{
		camera_href();
	}
#endif


}

/*!
*  @brief      DMA0�жϷ�������������ͷ�й�
*  @since      v5.0
*/
void DMA0_IRQHandler()
{
	camera_dma();
}






