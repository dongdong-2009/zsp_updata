/*****************************************
 *         ����һ�����Դ��Ӹ�Ĵ���
 *         ����ע����yl������
 *         ϣ�����ϲ��
******************************************/
if (i == 22)
{
  //������Ҫ�жϵ�һ���G�ߣ�
  //          ��   cur_R_ready_flag   �жϣ��ɹ����־λ��1    
  //���������ߵ���22��֮�󣬷ֱ��ж�22�к�24���Ƿ�����߲��G�ߣ��ұ߁G�ߵ����(��������ǰ�� �Ǻ� ��û��)��
  //���һ�׷��������������(45,75)֮����ж�
  //street_len������������������
  if (!Left_Add_Flag[i] && !Left_Add_Flag[i + 2] && Right_Add_Flag[i] && Right_Add_Flag[i + 2] && street_len > 47 && Width_Real[i + 2] > 45 && Width_Real[i + 2] < 75) //
  {
    cur_R_ready_flag = 1; //��Բ��Ԥʶ��       �󲻶��� �Ҷ���
  }
  else
    cur_R_ready_flag = 0; //������  ֻҪ���˵�һ�ζ��� ���˺�����в���ȥ


  if (cur_R_ready_flag || cur_R_ready_delay_flag)
  {
    //��1��ʱ��־λ
    cur_R_ready_delay_flag = 1; //��һ�ζ����жϺ��� "1"  ��Ϊ��Բ�жϵ�ǰ������
    
    //�����жϺ�Բ��22��24��λ��
    //�˴��ж�22��24�����Ҿ� û�� �G�ߣ�����������С��40
    //�ɹ�����Բ��־��1
    if ((!Right_Add_Flag[i] && !Right_Add_Flag[i + 2]) && !Left_Add_Flag[i] && !Left_Add_Flag[i + 2] && cur_R_ready_time_flag > 10 && street_len > 40) //
    {
      cur_R_ready_rest_flag = 1; //��Բ�жϱ�־
      aa = 1;
    }
    else
      ;

    //��һ�ζ���0.5��� ��һ�α�־����Բ��־����
    //��߁G�ߣ�����G�����岻��
    if (cur_R_ready_time_flag > 200 || Left_Add_Flag[i] || Left_Add_Flag[i + 2]) /*�˴�ʱ����Ҫ����*/
    {
      cur_R_ready_delay_flag = 0; //��һ�ζ��߱�־����
      cur_R_ready_rest_flag = 0;  //��Բ��־����
    }

    //��һ�ζ���0.5����  �ҵ��ڶ��ζ���  ��Բ�͵�һ�α�־����
    else if (cur_R_ready_rest_flag && Right_Add_Flag[i] && Right_Add_Flag[i + 2] && !Left_Add_Flag[i] && !Left_Add_Flag[i + 2] && street_len > 40)
    {
      cur_R_ready_delay_flag = 0; //��һ�ζ��߱�־����
      cur_R_ready_rest_flag = 0;  //��Բ��־����
      cur_R_real_flag = 1;        //�ڶ��ζ��߱�־
      bb = 1;
    }
    else
      ;
  }
  else //�����־������
  {
    cur_R_ready_delay_flag = 0;
    cur_R_ready_time_flag = 0;
    cur_R_ready_rest_flag = 0;
  }
}
/*��׼ȷʶ��*/
if (cur_R_real_flag || cur_R_real_delay_flag) //�ڶ��α�־��
{
  cur_R_real_flag = 0;       //�ڶ��α�־����
  cur_R_real_delay_flag = 1; //������ "1" �ڶ��α�־
  led(LED1, LED_ON);
  //����Ӧ��ת���ˣ����ս�Բ�������Ա���ǰ���ֱ����ɁG����
  if (i == 32 && cur_R_real_time_flag > 80 && (Left_Add_Flag[i + 2] && !Right_Add_Flag[i + 2])) //�ڵڶ��ζ���0.5��� �����22 24����
  {
    cur_R_real_rest_flag = 1; //��λ��־
    xx = 1;
  }
  else
    ;
  if (cur_R_real_rest_flag)
  {
    cur_R_real_time_flag1++;
  }
  else
    ;
  if (cur_R_real_time_flag1 > 200)
  {
    cur_R_real_time_flag1 = 200;
  }
  else
    ;
  if (i == 22 && cur_R_real_time_flag1 > 60 && !Left_Add_Flag[i] && !Left_Add_Flag[i + 2] && !Right_Add_Flag[i] && !Right_Add_Flag[i + 2])
  {
    cur_R_real_delay_flag = 0; //����ڶ��ζ��߱�־   ����ʱ��ֵΪ�ڶ��ζ��߱�־��
  }
  else
    ;
}
else
{
  cur_R_real_delay_flag = 0;
  cur_R_real_time_flag = 0;
  cur_R_real_time_flag1 = 0;
  cur_R_real_rest_flag = 0;
}
//����ʽPID ��� ����
int32 PID_Realize(PID *sptr, int32 ActualSpeed, int32 SetSpeed)
{
  //��ǰ������Ϊ�Ĵ���������ֻ���������ͺ��ַ��ͱ�������������ٶ�
  int32 iError, //��ǰ���
      Increase; //���ó���ʵ������

  iError = SetSpeed - ActualSpeed; //���㵱ǰ���

  Increase = (int)(sptr->KP * (iError - sptr->LastError) + sptr->KI * iError + sptr->KD * (iError - 2 * sptr->LastError + sptr->PrevError)); //���ٶ� ********************ǿ��װ���������� ��ֹ���ݳ���*********************
  sptr->PrevError = sptr->LastError;                                                                                                         //����ǰ�����
  sptr->LastError = iError;                                                                                                                  //�����ϴ����

  return Increase;
}
