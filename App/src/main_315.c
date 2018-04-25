//#include "common.h"
//#include "include.h"
//
//
//uint8 imgbuff[CAMERA_SIZE];                             //����洢����ͼ�������
//uint8 img[CAMERA_H][CAMERA_W];
//
//void PORTA_IRQHandler();
//void DMA0_IRQHandler();
//
//void  main()
//{
//    Site_t site     = {0, 0};                           //��ʾͼ�����Ͻ�λ��    
//    Size_t size;                                        //lcd��ʾ����ͼ���С
//    Size_t imgsize  = {CAMERA_W, CAMERA_H};             //��ѹͼ���С
//	int i = 0;
//
//	LED_show MODE = img_mode;
//	size.H = LCD_H;
//    size.W = LCD_W;
//
//    LCD_init();
//    camera_init(imgbuff);
//	gpio_init(PTE21, GPO, 0);
//	gpio_ddr(PTA8, GPO);
//
//	set_vector_handler(PORTA_VECTORn, PORTA_IRQHandler);   //���� PORTA ���жϷ�����Ϊ PORTA_IRQHandler
//	set_vector_handler(DMA0_VECTORn, DMA0_IRQHandler);     //���� DMA0 ���жϷ�����Ϊ PORTA_IRQHandler
//
//    while(1)
//    {
//        camera_get_img();                              //����ͷ��ȡͼ��
//                                                       //�ڰ�����ͷ
//        img_extract(img, imgbuff, CAMERA_SIZE);        //��ѹ
//		//Search_line();
//		//img_compress(img, imgbuff, CAMERA_SIZE);
//		LCD_Img_Binary_Z(site, size, imgbuff, imgsize);
//
//		i = 300;
//		while (i--);
//		gpio_turn(PTE21);
//    }
//}
//
//
//
//
///*!
//*  @brief      PORTA�жϷ�����
//*  @since      v5.0
//*/
//void PORTA_IRQHandler()
//{
//	uint8  n;    //���ź�
//	uint32 flag;
//
//	while (!PORTA_ISFR);
//	flag = PORTA_ISFR;
//	PORTA_ISFR = ~0;                                   //���жϱ�־λ
//
//	n = 29;                                             //���ж�
//	if (flag & (1 << n))                                 //PTA29�����ж�
//	{
//		camera_vsync();
//	}
//#if ( CAMERA_USE_HREF == 1 )                            //ʹ�����ж�
//	n = 28;
//	if (flag & (1 << n))                                 //PTA28�����ж�
//	{
//		camera_href();
//	}
//#endif
//
//
//}
//
///*!
//*  @brief      DMA0�жϷ�����
//*  @since      v5.0
//*/
//void DMA0_IRQHandler()
//{
//	camera_dma();
//}



/*!
*     COPYRIGHT NOTICE
*     Copyright (c) 2013,ɽ��Ƽ�
*     All rights reserved.
*     �������ۣ�ɽ����̳ http://www.vcan123.com
*
*     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
*     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
*
* @file       main.c
* @brief      ɽ��K60 ƽ̨������
* @author     ɽ��Ƽ�
* @version    v5.0
* @date       2013-08-28
*/

#include "common.h"
#include "include.h"


//��������
void PORTA_IRQHandler();
void DMA0_IRQHandler();


uint8 sharp_turn=0;
uint8 imgbuff[CAMERA_SIZE];                             //����洢����ͼ�������
uint8 img[CAMERA_H][CAMERA_W];


void  main(void)
{
	//int i = 0;
	//int j = 0;
	int degree_change_speed = 0;
	int degree_change = 0;
	Site_t site = { 0, 0 };             
	Size_t size;  
	Size_t imgsize = { CAMERA_W, CAMERA_H };
	     
	size.H = LCD_H;
	size.W = LCD_W;

	LCD_init();
	camera_init(imgbuff);
    ftm_pwm_init(FTM0, FTM_CH5, 300, 0);
	ftm_pwm_init(FTM0, FTM_CH6, 300, 43);

	set_vector_handler(PORTA_VECTORn, PORTA_IRQHandler);   //���� PORTA ���жϷ�����Ϊ PORTA_IRQHandler
	set_vector_handler(DMA0_VECTORn, DMA0_IRQHandler);     //���� DMA0 ���жϷ�����Ϊ PORTA_IRQHandler
	

	while (1)
	{
		camera_get_img();                                   
		img_extract(img, imgbuff, CAMERA_SIZE);

		Search_line();
		img_compress(img, imgbuff, CAMERA_SIZE);
		LCD_Img_Binary_Z(site, size, imgbuff, imgsize);


		if (average_offset < 5 && average_offset > -5)
		{
			ftm_pwm_duty(FTM0, FTM_CH6, 43 + average_offset);
		}
		else
		{
			degree_change = average_offset - average_offset_before;

					if (average_offset_before > 0)
					{
						if (degree_change > 0)
						{
							if (degree_change > 12) degree_change_speed = 7;
							else if (degree_change > 7) degree_change_speed = 4;
							else degree_change_speed = 2;
							steer_engine_degree += degree_change_speed;
						}
						if (degree_change < 0 && (abs(average_offset)<7)) steer_engine_degree -= 2;
					}
					else if (average_offset_before < 0)
					{
						if (degree_change < 0)
						{
							if (degree_change < -12) degree_change_speed = -7;
							else if (degree_change < -7) degree_change_speed = -4;
							else degree_change_speed = -2;
							steer_engine_degree += degree_change_speed;
						}
						if (degree_change > 0 && (abs(average_offset)<7)) steer_engine_degree += 2;
					}
	

					if (steer_engine_degree > 15) steer_engine_degree = 15;
					if (steer_engine_degree < -15) steer_engine_degree = -15;
		
					ftm_pwm_duty(FTM0, FTM_CH6, 43+ steer_engine_degree);
		}
		
		//ftm_pwm_duty(FTM0, FTM_CH6, 43 + average_offset_before);

		ftm_pwm_duty(FTM0, FTM_CH5,10 );                    //���
		//if (SHARP_TURN_LEFT == sharp_turn)
		//{
		//	ftm_pwm_duty(FTM0, FTM_CH6, 43 - TURN_DEGREE);
		//	sharp_turn = 0;
		//}
		//else if (SHARP_TURN_RIGHT == sharp_turn)
		//{
		//	ftm_pwm_duty(FTM0, FTM_CH6, 43+ TURN_DEGREE);
		//	sharp_turn = 0;
		//}
		//else
		//{
		//	ftm_pwm_duty(FTM0, FTM_CH6,43+ average_offset);           //���
		//}
		
	}
}



/*!
*  @brief      PORTA�жϷ�������������ͷ�й�
*  @since      v5.0
*/
void PORTA_IRQHandler()
{
	uint8  n;    //���ź�
	uint32 flag;

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

