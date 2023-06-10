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
  MUTE = 0;
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
  if(BitDisplayOn  &&  (!BitVoiceMute))		//����״̬�½���
    {
      //����ͨ������
      if(VarINCnt == 1)
        {
          IN_1 = 1;
          IN_2 = 0;
        }
      else if(VarINCnt == 2)
        {
          IN_2 = 1;
          IN_1 = 0;
        }
      //���ͨ������
				//add :  ѡ������ͨ����������ر�
      switch(VarOutCnt)
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
  if(BitDisplayOn)
    {
      if(BitVoiceMute)		//������־
        {
          MUTE = 1;				//����
        }
      else
        {
          MUTE = 0;				//��
        }
    }
  else
    {
      MUTE = 1;						//����
    }
}
