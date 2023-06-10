#include "all.h"

//IO��ƽ��ѯ������������


sbit 	 EC1_A = P3^0;						//����
sbit 	 EC1_B = P3^1;						//����
sbit 	 EC1_SW = P3^2;						//����
//
sbit 	 EC2_A = P3^6;						//����
sbit 	 EC2_B = P0^5;						//����
sbit 	 EC2_SW = P2^6;						//����

bit BitDisplayData_chang;

//https://whycan.com/t_6692.html
sbit EC_B1 = P0 ^ 0;   // ������ P0.0 �˿�
sbit EC_A1 = P1 ^ 3;   // ������ P1.3 �˿�
static uint8_t EC_A1_Val = 0, EC_B1_Val = 0;
static uint8_t EC_A1_old = 0, EC_B1_old = 0;
u8 KEY_state_A ;
//
sbit EC_B2 = P3 ^ 6;   // ������ P3.6 �˿�
sbit EC_A2 = P0 ^ 5;   // ������ P0.5 �˿�
static uint8_t EC_A2_Val = 0, EC_B2_Val = 0;
static uint8_t EC_A2_old = 0, EC_B2_old = 0;
u8 KEY_state_B;

u8 BitK1LastStatus,BitK2LastStatus;
bit BitVoiceMute;
//
void encoder_init()
{
  //A
  GPIO_SET_MUX_MODE(P30CFG, GPIO_MUX_GPIO);
  GPIO_ENABLE_INPUT(P3TRIS, GPIO_PIN_0);
  GPIO_SET_MUX_MODE(P31CFG, GPIO_MUX_GPIO);
  GPIO_ENABLE_INPUT(P3TRIS, GPIO_PIN_1);
  //
  GPIO_ENABLE_UP(P3UP,GPIO_PIN_2);
  GPIO_SET_MUX_MODE(P32CFG, GPIO_MUX_GPIO);
  GPIO_ENABLE_INPUT(P3TRIS, GPIO_PIN_2);
  //B
  GPIO_SET_MUX_MODE(P36CFG, GPIO_MUX_GPIO);
  GPIO_ENABLE_INPUT(P3TRIS, GPIO_PIN_6);
  GPIO_SET_MUX_MODE(P05CFG, GPIO_MUX_GPIO);
  GPIO_ENABLE_INPUT(P0TRIS, GPIO_PIN_5);
  //
  GPIO_ENABLE_UP(P2UP,GPIO_PIN_6);
  GPIO_SET_MUX_MODE(P26CFG, GPIO_MUX_GPIO);
  GPIO_ENABLE_INPUT(P2TRIS, GPIO_PIN_6);
  //
  //��ȡ��ʼ״̬������������
  EC_B2_old = EC2_A;
  EC_A2_old = EC2_B;

  EC_B1_old = EC1_A;
  EC_A1_old = EC1_B;

}
#if 1




/*********************************************
��������EncoderReading
��  �ܣ���ȡ����������
��  �Σ�
����ֵ��0--�޶��� 1--��ת  2--��ת
��  ע��
��  �ߣ�Ѧ��ǿ
ʱ  �䣺2019/06/11
**********************************************/
u8 EncoderReading_A(void)
{
  /*����һ��*/
  EC_A1_Val = EC1_A;
  EC_B1_Val = EC1_B;
  /*************EC_B������*******************/
  if (EC_A1_Val == EC_A1_old && EC_B1_Val > EC_B1_old)
    {
      /*���¸���ֵ��ֵ*/
      EC_B1_old = EC_B1_Val;
      EC_A1_old = EC_A1_Val;
      if (EC_A1_Val == 0) //��ת
        {
          return 2;
        }
    }
  /*************EC_B������*******************/
  if (EC_A1_Val == EC_A1_old && EC_B1_Val < EC_B1_old)
    {
      /*���¸���ֵ��ֵ*/
      EC_B1_old = EC_B1_Val;
      EC_A1_old = EC_A1_Val;
      if (EC_A1_Val == 1) //��ת
        {
          return 2;
        }
    }
  /*************EC_A������*******************/
  if (EC_B1_Val == EC_B1_old && EC_A1_Val > EC_A1_old)
    {
      /*���¸���ֵ��ֵ*/
      EC_B1_old = EC_B1_Val;
      EC_A1_old = EC_A1_Val;
      if (EC_B1_Val == 0) //��ת
        {
          return 1;
        }
    }
  /*************EC_A������*******************/
  if (EC_B1_Val == EC_B1_old && EC_A1_Val < EC_A1_old)
    {
      /*���¸���ֵ��ֵ*/
      EC_B1_old = EC_B1_Val;
      EC_A1_old = EC_A1_Val;
      if (EC_B1_Val == 1) //��ת
        {
          return 1;
        }
    }
  return 0;
}
////////////////////////////
u8 EncoderReading_B(void)
{
  /*����һ��*/
  EC_A2_Val = EC2_A;
  EC_B2_Val = EC2_B;
  /*************EC_B������*******************/
  if (EC_A2_Val == EC_A2_old && EC_B2_Val > EC_B2_old)
    {
      /*���¸���ֵ��ֵ*/
      EC_B2_old = EC_B2_Val;
      EC_A2_old = EC_A2_Val;
      if (EC_A2_Val == 0) //��ת
        {
          return 2;
        }
    }
  /*************EC_B������*******************/
  if (EC_A2_Val == EC_A2_old && EC_B2_Val < EC_B2_old)
    {
      /*���¸���ֵ��ֵ*/
      EC_B2_old = EC_B2_Val;
      EC_A2_old = EC_A2_Val;
      if (EC_A2_Val == 1) //��ת
        {
          return 2;
        }
    }
  /*************EC_A������*******************/
  if (EC_B2_Val == EC_B2_old && EC_A2_Val > EC_A2_old)
    {
      /*���¸���ֵ��ֵ*/
      EC_B2_old = EC_B2_Val;
      EC_A2_old = EC_A2_Val;
      if (EC_B2_Val == 0) //��ת
        {
          return 1;
        }
    }
  /*************EC_A������*******************/
  if (EC_B2_Val == EC_B2_old && EC_A2_Val < EC_A2_old)
    {
      /*���¸���ֵ��ֵ*/
      EC_B2_old = EC_B2_Val;
      EC_A2_old = EC_A2_Val;
      if (EC_B2_Val == 1) //��ת
        {
          return 1;
        }
    }
  return 0;
}


//////////////////
void encoder_a(void)
{
  //ʹ�÷�ʽ��
  KEY_state_A = EncoderReading_A(); //�����������
  switch (KEY_state_A)
    {
    case 1://��ת
      BitDisplayData_chang = 1;

      //���ͨ���ȼ�
      if(VarOutCnt>4)
        {
          VarOutCnt = 1;
        }
      else
        {
          VarOutCnt ++;
        }
      //
      BitDataCharg = 1;
      //
      break;
    case 2://��ת
      BitDisplayData_chang = 1;
      if(VarOutCnt <= 1)
        {
          VarOutCnt = 4;
        }
      else
        {
          VarOutCnt --;
        }
      //
      BitDataCharg = 1;
      //
      break;
    default :
      /* ��ѡ�� */
      break;

    }
}
void encoder_b(void)
{

  //ʹ�÷�ʽ��
  KEY_state_B = EncoderReading_B(); //������
	if(BitVoiceMute)  return ;				//����ʱ�˳���
	//
  switch (KEY_state_B)
    {
    case 1://��ת
      BitDisplayData_chang = 1;			//�����и��ı�־λ��1
      BitUpData_chang = 1;
      BitData_Astrict_R = 0;
      if(VarVoiceLevel == 1)
        {
          VarVoiceLevel =1;
        }
      else
        {
          VarVoiceLevel--;
        }
      //save_VoiceLevel();
      BitDataCharg = 1;
      break;
    case 2://��ת
      BitDisplayData_chang = 1;
      BitUpData_chang = 1;
      //
      BitData_Astrict_F =0;

      if(VarVoiceLevel >= 254)
        {
          VarVoiceLevel =254;
        }
      else
        {
          VarVoiceLevel++;
        }
      //
      BitDataCharg = 1;
      //save_VoiceLevel();
      break;
    default :
      /* ��ѡ�� */
      break;
    }
}


/*
*********************************************************************************************************
*	�� �� ��: Scan_encodeer_a
*	����˵��: ��������������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Scan_encodeer_a(void)
{
  static u8		VarK1PressCnt;
  static u8	 	VarK1ReleaseCnt;
  if(EC1_SW == 0)
    {
      //����
      VarK1ReleaseCnt = 0;
      //
      VarK1PressCnt ++;
      if(VarK1PressCnt > 3)
        {
          VarK1PressCnt--;
          if(BitK1LastStatus == 0)
            {
              BitK1LastStatus = 1;
              BitDisplayData_chang = 1;		//��ʾ���±�־
              //
              BitDataCharg = 1;						//���ݱ�����±�־
              if(VarINCnt >= 2)
                {
                  VarINCnt = 1;
                }
              else
                {
                  VarINCnt ++;
                }
            }
        }
    }
  else
    {
      //��������ͷ�
      VarK1PressCnt = 0 ;
      if(++VarK1ReleaseCnt >= 3)
        {
          --VarK1ReleaseCnt ;
          BitK1LastStatus = 0 ;
        }
    }
}
/*
*********************************************************************************************************
*	�� �� ��: Scan_encodeer_b
*	����˵��: ��������������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Scan_encodeer_b(void)
{
  static u8		VarK2PressCnt;
  static u8	 	VarK2ReleaseCnt;
  if(EC2_SW == 0)
    {
      //����
      VarK2ReleaseCnt = 0;
      //
      VarK2PressCnt ++;
      if(VarK2PressCnt >= 3)
        {
          VarK2PressCnt--;
          if(BitK2LastStatus == 0)
            {
              BitK2LastStatus = 1;
              BitDisplayData_chang = 1;
              //���ƽ�����Ƶ
              BitVoiceMute = ~BitVoiceMute;
            }
        }
    }
  else
    {
      //��������ͷ�
      VarK2PressCnt = 0 ;
      if(++VarK2ReleaseCnt >= 3)
        {
          --VarK2ReleaseCnt ;
          BitK2LastStatus = 0 ;
        }
    }

}



#endif
////////////////////////////////////////////////////////////////
#if 0
u8 xdata BitBmEn_b;
u8 xdata BitBmInfo_b ;
u8 xdata BitBmEn_a;
u8 xdata BitBmInfo_a ;
//
u8 xdata BitTEST;

void encoder_a()
{
  u8 i = 0;
  //
  static u8 CurrentCode_a,LastCode_a,Codecnt_a;
  //
  if(EC1_A) i|= 0x01;
  if(EC1_B) i|= 0x02;
  //
  if(CurrentCode_a != i)
    {
      CurrentCode_a = i;
      Codecnt_a = 30;
    }
  if(Codecnt_a)Codecnt_a--;
  else
    {
      //	if(BitBmEn_a == 0)
      {
        if(CurrentCode_a == 3)
          {
            if(LastCode_a == 1)
              {
                BitBmEn_a = 1;
                BitBmInfo_a = 0;
              }
            else if (LastCode_a == 2)
              {
                BitBmEn_a = 1;
                BitBmInfo_a = 1;
              }
          }
        LastCode_a = CurrentCode_a;
      }
    }

}

void encoder_b()
{
  u8 i = 0;
  //
  static u8 CurrentCode_b,LastCode_b,Codecnt_b;
  //
  if(EC2_A) i|= 0x01;
  if(EC2_B) i|= 0x02;
  //
  if(CurrentCode_b != i)
    {
      CurrentCode_b = i;
      Codecnt_b = 30;
    }
  if(Codecnt_b)Codecnt_b--;
  else
    {
      //if(BitBmEn_b == 0)
      {
        if(CurrentCode_b == 3)
          {
            if(LastCode_b == 1)
              {
                BitBmEn_b = 1;
                BitBmInfo_b = 0;
              }
            else if (LastCode_b == 2)
              {
                BitBmEn_b = 1;
                BitBmInfo_b = 1;
              }
          }
        LastCode_b = CurrentCode_b;
      }
    }

}
void encoder_a_ctrl()
{
  if(BitBmEn_a)
    {
      BitBmEn_a = 0;
      if(BitBmInfo_a)
        {
          BitTEST = 1;
        }
      else
        {
          BitTEST =2;
        }
    }
}
u8 BitTESTb;

void encoder_b_ctrl()
{
  if(BitBmEn_b)
    {
      BitBmEn_b = 0;
      if(BitBmInfo_b)
        {
          BitTESTb = 1;
        }
      else
        {
          BitTESTb =2;
        }
    }
}
#endif


