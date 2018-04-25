#include "common.h"


//ѹ����ֵ��ͼ���ѹ���ռ� �� ʱ�� ��ѹ��
//srclen �Ƕ�ֵ��ͼ���ռ�ÿռ��С
//��ӥ�۽�ѹ��ӥ��ͼ���ѹ��תΪ ��ά���� - ���ܳ������� - ɽ����̳ http://vcan123.com/forum.php?mod=viewthread&tid=17&ctid=6
//��ѹ��ʱ�������и����飬���úڡ��׶�Ӧ��ֵ�Ƕ��١�
void img_extract(void *dst, void *src, uint32_t srclen)
{
    uint8_t colour[2] = {1, 0}; //0 �� 1 �ֱ��Ӧ����ɫ
    uint8_t * mdst = dst;
    uint8_t * msrc = src;
    //ע��ɽ�������ͷ 0 ��ʾ ��ɫ��1��ʾ ��ɫ
    uint8_t tmpsrc;
    while(srclen --)
    {
        tmpsrc = *msrc++;
        *mdst++ = colour[ (tmpsrc >> 7 ) & 0x01 ];
        *mdst++ = colour[ (tmpsrc >> 6 ) & 0x01 ];
        *mdst++ = colour[ (tmpsrc >> 5 ) & 0x01 ];
        *mdst++ = colour[ (tmpsrc >> 4 ) & 0x01 ];
        *mdst++ = colour[ (tmpsrc >> 3 ) & 0x01 ];
        *mdst++ = colour[ (tmpsrc >> 2 ) & 0x01 ];
        *mdst++ = colour[ (tmpsrc >> 1 ) & 0x01 ];
        *mdst++ = colour[ (tmpsrc >> 0 ) & 0x01 ];
    }
}


/*ͼ���ѹ��*/
void img_compress(void *img, void *imgbuff, uint32_t srclen)
{
	uint8_t * big = img;
	uint8_t * small = imgbuff;
	uint8_t i = 0;
	uint8_t temp = 0;

	while (srclen--)
	{
		temp = 0;

		for (i = 0; i < 8; i++)
		{
			temp = (temp | *big++);
			if(i<7)
			    temp = (temp << 1);
		}
		*small = temp;
		small++;
	}
}


/*ͼ��ת�ã����ѵ���������*/
void img_transposition(void *img)
{
	//int i = 59;     //ͼ��Ϊ60*80����Ϊ��0���������ݣ������±�Ϊ59��79
	//int j = 79;

	uint8_t temp = 0;
	uint8_t *start = img;
	uint8_t *end;

	end = start + 4799;

	while (start != (end-1))
	{
		temp = (*start);
		(*start) = (*end);
		(*end) = temp;
		start++;
		end--;
	}
}