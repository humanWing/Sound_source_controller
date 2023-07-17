/*
*********************************************************************************************************
*
*	ģ������ :
*	�ļ����� : bsp_out.c
*	��    �� : V1.0
*	˵    �� :
*
*	�޸ļ�¼ :
*		�汾��  ����         ����     ˵��
*
*********************************************************************************************************
*/
#include "all.h"

//����ULN2003 ���ֶٹ�   H-��   L-�ر�
//����IO��  2��4��
sbit 	 IN_1 = P2^3;						//����
sbit 	 IN_2 = P2^2;						//����
//
sbit 	 OUT_1 = P1^6;						//����
sbit 	 OUT_2 = P1^5;						//����
sbit 	 OUT_3 = P1^4;						//����
sbit 	 OUT_4 = P1^3;						//����

sbit   MUTE =  P1^7;						//��������  H ����  L��Ч


/*
*********************************************************************************************************
*	�� �� ��: out_init
*	����˵��: ����IO
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void out_init(void)
{
  //
  IN_1 = 0;
  IN_2 = 0;
  OUT_1 = 0;
  OUT_2 = 0;
  OUT_3 = 0;
  OUT_4 = 0;
  //
  MUTE = 1;
  //
  GPIO_SET_MUX_MODE(P23CFG, GPIO_MUX_GPIO);
  GPIO_ENABLE_OUTPUT(P2TRIS, GPIO_PIN_3);

  GPIO_SET_MUX_MODE(P22CFG, GPIO_MUX_GPIO);
  GPIO_ENABLE_OUTPUT(P2TRIS, GPIO_PIN_2);
  //
  GPIO_SET_MUX_MODE(P16CFG, GPIO_MUX_GPIO);
  GPIO_ENABLE_OUTPUT(P1TRIS, GPIO_PIN_6);

  GPIO_SET_MUX_MODE(P15CFG, GPIO_MUX_GPIO);
  GPIO_ENABLE_OUTPUT(P1TRIS, GPIO_PIN_5);

  GPIO_SET_MUX_MODE(P14CFG, GPIO_MUX_GPIO);
  GPIO_ENABLE_OUTPUT(P1TRIS, GPIO_PIN_4);

  GPIO_SET_MUX_MODE(P13CFG, GPIO_MUX_GPIO);
  GPIO_ENABLE_OUTPUT(P1TRIS, GPIO_PIN_3);
  //

  GPIO_SET_MUX_MODE(P17CFG, GPIO_MUX_GPIO);
  GPIO_ENABLE_OUTPUT(P1TRIS, GPIO_PIN_7);
//
}


/*
*********************************************************************************************************
*	�� �� ��: out_ctrl
*	����˵��: �����趨ֵ�������д򿪹ر����� ���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void out_ctrl()
{
  if(!BitVoiceMute)		//����״̬�½���
    {
      //����ͨ������
      if(eb_voice_input_channel == 1)
        {
          IN_1 = 1;
          IN_2 = 0;
        }
      else if(eb_voice_input_channel == 2)
        {
          IN_2 = 1;
          IN_1 = 0;
        }
      //���ͨ������
				//add :  ѡ������ͨ����������ر�
      switch(eb_voice_output_channel)
        {
        case 1:
          OUT_1 = 1;
          OUT_2 = 0;
          OUT_3 = 0;
          OUT_4 = 0;
          break;

        case 2:
          OUT_1 = 0;
          OUT_2 = 1;
          OUT_3 = 0;
          OUT_4 = 0;
          break;

        case 3:
          OUT_1 = 0;
          OUT_2 = 0;
          OUT_3 = 1;
          OUT_4 = 0;
          break;
        case 4:
          OUT_1 = 0;
          OUT_2 = 0;
          OUT_3 = 0;
          OUT_4 = 1;
          break;
        }
      //
    }
  else
    {
      OUT_1 = 0;					//�ر�
      OUT_2 = 0;
      OUT_3 = 0;
      OUT_4 = 0;
      //
      IN_1 = 0;
      IN_2 = 0;
    }
}

/*
*********************************************************************************************************
*	�� �� ��: Mute_ctrl
*	����˵��: �����趨ֵ����/�رվ���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Mute_ctrl(void)
{
  static uint8_t wait_pro_time = 0;

  if (wait_pro_time < 200)   // 20ms:one time Mute_ctrl()
  {
    wait_pro_time++;
    return;
  }

  if(BitVoiceMute)		//������־
  {
      MUTE = 1;				//����
  }
  else
  {
      MUTE = 0;				//��
  }
}



void bsp_voice_select_inchannel_down(void)
{
    if (BitVoiceMute == 0)
    {
        eb_voice_input_channel = eb_voice_input_channel > output_channel_1 ?
        (eb_voice_input_channel - 1) : 2;
        BitDisplayData_chang = 1;
        VarWirtFlashCnt = 0;
        eBit_DataCharg = 1;
    }
}

void bsp_voice_select_inchannel_plus(void)
{
    if (BitVoiceMute == 0)
    {
        eb_voice_input_channel = eb_voice_input_channel < input_channel_2 ?\
        (eb_voice_input_channel + 1) : 1;
        BitDisplayData_chang = 1;
        VarWirtFlashCnt = 0;
        eBit_DataCharg = 1;
    }
}


void bsp_voice_select_outchannel_down(void)
{
    if (BitVoiceMute == 0)
    {
        eb_voice_output_channel = eb_voice_output_channel > output_channel_1 ?\
        (eb_voice_output_channel - 1) : 4;
        BitDisplayData_chang = 1;
        VarWirtFlashCnt = 0;
        eBit_DataCharg = 1;
    }
}

void bsp_voice_select_outchannel_plus(void)
{
    if (BitVoiceMute == 0)
    {
        eb_voice_output_channel = eb_voice_output_channel < output_channel_4 ?\
        (eb_voice_output_channel + 1) : 1;
        BitDisplayData_chang = 1;
        VarWirtFlashCnt = 0;
        eBit_DataCharg = 1;
    }
}

void bsp_voice_plus(void)
{
    if ((BitVoiceMute == 0)
     && (eb_voice_level < VOLUME_MAX_CLASS))
    {
        eb_voice_level++;
        BitDisplayData_chang = 1;
        eb_button_change_motor_sta = 1;
        VarWirtFlashCnt = 0;
        eBit_DataCharg = 1;
    }
}

void bsp_voice_fast_plus(void)
{
    uint8_t temp_buff;

    if ((BitVoiceMute == 0)
     && (eb_voice_level < VOLUME_MAX_CLASS))
    {
        temp_buff = VOLUME_MAX_CLASS - eb_voice_level;

        if (temp_buff >= 10)
        {
            eb_voice_level = eb_voice_level + 10;
        }
        else
        {
            eb_voice_level = eb_voice_level + temp_buff;
        }

        BitDisplayData_chang = 1;
        eb_button_change_motor_sta = 1;
        VarWirtFlashCnt = 0;
        eBit_DataCharg = 1;
    }
}

void bsp_voice_minus(void)
{
    if ((BitVoiceMute == 0)
     && (eb_voice_level > VOLUME_MIN_CLASS))
    {
        eb_voice_level--;
        BitDisplayData_chang = 1;
        eb_button_change_motor_sta = 1;
        VarWirtFlashCnt = 0;
        eBit_DataCharg = 1;
    }
}

void bsp_voice_fast_minus(void)
{
    uint8_t temp_buff;
    if ((BitVoiceMute == 0)
     && (eb_voice_level > VOLUME_MIN_CLASS))
    {
        temp_buff = eb_voice_level - VOLUME_MIN_CLASS;

        if (temp_buff >= 10)
        {
            eb_voice_level = eb_voice_level - 10;
        }
        else
        {
            eb_voice_level = eb_voice_level - temp_buff;
        }

        BitDisplayData_chang = 1;
        eb_button_change_motor_sta = 1;
        VarWirtFlashCnt = 0;
        eBit_DataCharg = 1;
    }
}

void bsp_voice_reset_customer_setting(void)
{
    if ((BitVoiceMute == 0)
     && (eb_voice_level != 30))
    {
        eb_voice_level = 30;
        BitDisplayData_chang = 1;
        eb_button_change_motor_sta = 1;
        VarWirtFlashCnt = 0;
        eBit_DataCharg = 1;
    }
}


void bsp_voice_mute_toggle(void)
{
    BitDisplayData_chang = 1;
    //���ƽ�����Ƶ
    BitVoiceMute = ~BitVoiceMute;
}
