/*********************************************************************************************************
*
*   ģ������ : IR����ģ��
*   ˵    �� : 100US ��ʱ���е��á�
*********************************************************************************************************/
#include "all.h"

//����ı���
u8  VarIRRxData[ConIRRxDataCout];       			//��������BUF

bit BitIRRXPress;
bit BitIRRX_9MS;                              //9MS�͵�ƽ������ɱ�־
bit BitIRRX_4D5MS;                            //4.5MS�ߵ�ƽ������ɱ�־
bit BitIRRX_DUP;                                        //�ظ����־.
bit BitIRRX_FREE;                                       //IR���ս��б�־  1:���ڽ�����
bit BitIRRX_FINISH;                                			//������ɱ�־
bit BitIRRX_LEVEL;
u8 VarIRRX_DATA;
u8 VarIRRX_CNT;                                         //;IR�������ݼ���,�ܹ�4�ֽ�,32bit.
u8 VarIRRX_TIME;                                        //;IR����ʱ�����.
u8  VarIRRxBitCnt;
//
u8 Var_IR_RX_PRESS_TIME;
u8 Var_IR_RX_RELEASE_TIME;
bit Bit_IR_RX_LONG_PRESS;
bit BitIRRXPress;
u8 vattestcnt;
xdata u8 BitVoiceLevelBack;
//
void IR_Init()
{
  GPIO_SET_MUX_MODE(P04CFG, GPIO_MUX_GPIO);
  GPIO_ENABLE_INPUT(P0TRIS, GPIO_PIN_4);
}

void IR_RX(void)
{
  //
  if (!BitIRRX_FREE)                                  //�Ƿ����ͷ�״̬
    {
      if (!P_IR_RX)                                   //�͵�ƽ����
        {
          //
          VarIRRX_DATA = 0;                           //���������
          //
          BitIRRX_LEVEL = 0;
          //
          VarIRRX_TIME = 0;                           //����ʱ��
          BitIRRX_9MS = 0;
          BitIRRX_4D5MS = 0;
          BitIRRX_DUP = 0;
          BitIRRX_FINISH = 0;
          VarIRRX_CNT  = 0;                           //32bit -4���ֽ�
          VarIRRxBitCnt = 0;
          //
          BitIRRX_FREE  = 1;                          //��1--;���յ��͵�ƽ,��������

        }
    }
  else
    {
      if(BitIRRX_DUP)										//�ظ����־λ
        {
          //
          if (P_IR_RX == 0)																		//��ƽһֱΪ��
            {
              if (++VarIRRX_TIME >= ConIRRXLOW_OVER)						//�Ҵ���OVERʱ�䣬�ͷ�
                {
                  BitIRRX_FREE = 0;
                  return ;                                    //�˳�
                }
            }
        }
      else if (BitIRRX_4D5MS)                                 //����
        {
          if (BitIRRX_FINISH)                                 //���ճɹ���,������յ���ƽ.ǿ���ͷŽ���״̬.
            {
              if (!P_IR_RX)
                {
                  if (VarIRRX_TIME >= ConIRRXLOW_OVER) 				//ǿ���ͷŽ���״̬.
                    {
                      BitIRRX_FREE = 0;
                      return ;                            		//�˳�
                    }
                }
              else
                {
                  BitIRRX_FREE = 0;
                  return ;                           				 //�˳�
                }
            }
          else                                               //32��bit���� ���մ���
            {
              if (P_IR_RX)
                {
                  if (!BitIRRX_LEVEL)                       //��ƽ״̬��־λ
                    {
                      BitIRRX_LEVEL = 1;                    //�͵�ƽ��ߵ�ƽ��ʱ��,���ʱ�����,���¿�ʼ�Ƹߵ�ƽʱ��
                      VarIRRX_TIME = 0;
                    }

                  VarIRRX_TIME ++;

                  if (VarIRRX_TIME >= ConIRRXHIGH_OVER)       //����ߵ�ƽʱ�䳬��,   ˵���������ݴ���,ǿ���ͷŽ���״̬.
                    {
                      BitIRRX_FREE = 0;                       //�ͷ�
                      //
                      return ;                                //�˳�
                    }
                }
              else                                            //״̬ת��L��ƽʱ�����ж������� 1����0
                {
                  if (BitIRRX_LEVEL)                          //
                    {
                      BitIRRX_LEVEL = 0;
                      //
                      //�ж�����
                      VarIRRX_DATA = VarIRRX_DATA >> 1;       //�ӵ�λ��ʼ���գ�����һλ

                      if (VarIRRX_TIME  > ConIR_RX_01)        //ʱ��>1ms,˵�����յ��ߵ�ƽ,
                        {
                          VarIRRX_DATA  |= 0x80;
                        }

                      //
                      VarIRRxBitCnt++;                                        //

                      if (VarIRRxBitCnt >= 8)
                        {
                          VarIRRxBitCnt = 0;
                          //
                          VarIRRxData[VarIRRX_CNT] = VarIRRX_DATA;

                          if (++VarIRRX_CNT >= 4)         //32���ֽڽ������
                            {
                              VarIRRX_CNT = 0;

                              if ((VarIRRxData[0] == 0xB2) && (VarIRRxData[1] == 0x4D))   //�ж�����
                                {
                                  BitIRRX_FINISH = 1;                         //�������
                                }

                              return ;                                                    //�˳�ѭ��

                            }
                        }

                      VarIRRX_TIME    =   0;
                    }

                  //
                  VarIRRX_TIME ++;

                  if (VarIRRX_TIME >= ConIRRXLOW_OVER)    //�͵�ƽʱ�����+1
                    {
                      BitIRRX_FREE = 0;                   //>0.5ms--ǿ���ͷ�
                      return ;                            //�˳�
                    }
                }


            }
        }
      else if (BitIRRX_9MS)
        {
          //4.5MS
          if (P_IR_RX)                                 //�ߵ�ƽ����
            {
              VarIRRX_TIME ++;                         //���յ��ߵ�ƽ,�͵�ƽ����ʱ��+1
              //
              if (VarIRRX_TIME >= ConIRRxOVER_4D5ms)  //ǿ���ͷŽ���״̬.
                {
                  BitIRRX_FREE = 0;                               //
                  return ;                            //�˳�
                }
            }
          else                                                                    //
            {
              if (VarIRRX_TIME < ConIR_RX_DUP)        //����ʱ��С��2.6ms,˵�������������ظ���
                {
                  BitIRRX_DUP = 1;                    //������յ��ߵ�ƽʱǿ���ͷ�,9ms��4.5ms��������,����û�н��յ�����,��Ϊ���յ��ظ���.
                  VarIRRX_TIME = 0;
                }
              else if (VarIRRX_TIME > ConIRRx_4D5ms)
                {

                  BitIRRX_4D5MS  = 1;                 //����ʱ��>4.5ms,˵��4.5MSʱ�������ȷ.
                  VarIRRX_TIME = 0;

                }
              else
                {
                  BitIRRX_FREE = 0;                               //ǿ���ͷ�
                  return ;                            //�˳�
                }
            }
        }
      else
        {
          //;����9ms�͵�ƽ.
          if (!P_IR_RX)                                                   //�͵�ƽ����
            {
              VarIRRX_TIME ++;                                        //���յ��͵�ƽ,�͵�ƽ����ʱ��+1
              //
              if (VarIRRX_TIME >= ConIRRxOVER_9ms) //ǿ���ͷŽ���״̬.
                {
                  BitIRRX_FREE = 0;                               //
                  return ;                            //�˳�
                }
            }
          else                                                                  //��ƽ״̬Ϊ��--��ʱ�ж�VarIRRX_TIME
            {
              if (VarIRRX_TIME >   ConIRRx_9ms)       //����ʱ��>8.5ms,˵��9MSʱ�������ȷ.
                {
                  VarIRRX_TIME = 0;
                  BitIRRX_9MS = 1;
                }
              else                                                            //����ʱ��С��8.5ms,˵���������ݴ���,ǿ���ͷŽ���״̬
                {
                  BitIRRX_FREE = 0;
                  return ;                            //�˳�
                }
            }
        }
    }
}

/*============================================================================
*������մ���
�ɺ�����ճ��򴫵ݵı�־ F_IR_RX_FINISH, F_IR_RX_DUP �жϰ���״̬.
;�ɺ�����ճ��򴫵ݵ����� R_IR_RX_DATA1( �û��� )   R_IR_RX_DATA3( ���� )�жϰ���λ��.
============================================================================*/

void IRReceiveCTRL(void)
{
  u8  tmp1;
  u8  IR_RX_VALUE;
  //
  if(++Var_IR_RX_PRESS_TIME >= 255)		//��������ʱ��+1
    Var_IR_RX_PRESS_TIME --;
  //
  if (BitIRRX_FINISH)								 //�����ⰴ��.�����һ�κ�����մ���.  ����ֻ�ǵ�һ�β�������.
    {
      BitIRRX_FINISH = 0;
      //
      //��֤�����Ƿ�ʱ��ȷ
      tmp1 = VarIRRxData[3] ^ 0xFF;

      if (VarIRRxData[2] == tmp1)
        {
          Var_IR_RX_RELEASE_TIME = 0;		//�����ͷ�ʱ����0.
          Var_IR_RX_PRESS_TIME = 0;			//��������ʱ����0.
          //
          BitIRRXPress = 1;							//�ú��ⰴ�±�־
          IR_RX_VALUE =VarIRRxData[2] ;//����һ��������������
          //
          BitDisplayData_chang = 1;
          //
          switch (IR_RX_VALUE)				//�ж�����
            {
            case 0xDC:								//IR���ػ�
              BitDisplayOn = ~BitDisplayOn;

              //
              if(BitDisplayOn)
                {
                  SysStatus = 0;					//IR������-- �л�������ʱ
                  VarAutoSet = 3;
                  VarAutoTimeCnt = 0;
                  VarDisplayUpdateCnt = 0;
                }
              else
                {
                  BitDisplayData_chang = 1;

                }//
              break;
            case 0x82:								//����
              if(BitDisplayOn)				//����״̬�²��ܽ��д˲���
                {
                  BitVoiceMute = ~BitVoiceMute;
                  BitDisplayData_chang = 1;
                }
              break;
            case 0xCA:								//�����ȼ���
              if(BitDisplayOn)				//����״̬�²��ܽ��д˲���
                {
                  BitUpData_chang = 1;
                  BitData_Astrict_R = 0;
                  //
                  if(VarVoiceLevel>= 254)
                    {
                      VarVoiceLevel = 254;
                    }
                  else
                    {
                      VarVoiceLevel++;
                    }
                  BitDataCharg = 1;
                }
              break;
            case 0x80:
              if(BitDisplayOn)				//����״̬�²��ܽ��д˲���
                {
                  BitUpData_chang = 1;
                  BitData_Astrict_R = 0;
                  //
                  if(VarVoiceLevel>= 254)
                    VarVoiceLevel = 254;
                  else
                    VarVoiceLevel++;
                  //
                  BitDataCharg = 1;

                }
              break;

            case 0xD2:								//�����ȼ���
              if(BitDisplayOn)				//����״̬�²��ܽ��д˲���
                {
                  BitUpData_chang = 1;
                  BitData_Astrict_F = 0;
                  if(VarVoiceLevel== 0)
                    {
                      VarVoiceLevel = 0;
                    }
                  else
                    {
                      VarVoiceLevel--;
                    }
                  BitDataCharg = 1;

                }
              break;
            case 0x81:
              if(BitDisplayOn)				//����״̬�²��ܽ��д˲���
                {
                  BitUpData_chang = 1;
                  BitData_Astrict_F = 0;
                  if(VarVoiceLevel== 0)
                    {
                      VarVoiceLevel = 0;
                    }
                  else
                    {
                      VarVoiceLevel--;
                    }
                  BitDataCharg = 1;
                }
              break;
            case 0x99:								//����
              if(BitDisplayOn)				//����״̬�²��ܽ��д˲���
                {
                  if(VarINCnt == 1)
                    {
                      VarINCnt = 2;
                    }
                  else
                    {
                      VarINCnt --;
                    }
                  BitDataCharg = 1;
                }
              break;

            case 0xC1:						//���ͨ���ȼ�
              if(BitDisplayOn)				//����״̬�²��ܽ��д˲���
                {
                  if(VarINCnt == 2)
                    {
                      VarINCnt = 1;
                    }
                  else
                    {
                      VarINCnt ++;
                    }
                  BitDataCharg = 1;


                }
              break;

            case 0xce:
              if(BitDisplayOn)				//����״̬�²��ܽ��д˲���
                {
                  if(VarOutCnt>=4)
                    {
                      VarOutCnt = 1;
                    }
                  else
                    {
                      VarOutCnt ++;
                    }
                  BitDataCharg = 1;
                }
              break;

            case 0xC5:								//�л��ص���ʼ�����ȼ�
              if(BitDisplayOn)				//����״̬�²��ܽ��д˲���
                {
                  BitVoiceLevelBack = 1;	//���˱�־λ
                }
              break;

            default:
              break;
            }

        }

    }
  else
    {
      //��⵽�ظ���,�����ظ��봦��,��Ҫ����������.
      if(BitIRRX_DUP)
        {
          BitIRRX_DUP = 0;
          //��⵽�ظ���֮ǰ�����ȼ�⵽������,����ǿ���ͷ�.
          if(BitIRRXPress)
            {
              Var_IR_RX_RELEASE_TIME = 0;			//�����ͷ�ʱ����0
              //
              if(Var_IR_RX_PRESS_TIME >= 120 )//ʱ���ֵ 10ms
                {
                  Var_IR_RX_PRESS_TIME --;
                  //
                  Bit_IR_RX_LONG_PRESS	 = 1;		//����������־��1
                  //
//dup:
                  BitDisplayData_chang = 1;
                  switch(VarIRRxData[2])				//�˴���ӳ�������.
                    {
                    case 0xCA:								//�����ȼ���
                      if(BitDisplayOn)				//����״̬�²��ܽ��д˲���
                        {
                          BitUpData_chang = 1;
                          BitData_Astrict_R = 0;
                          //
                          if(VarVoiceLevel>= 254)
                            VarVoiceLevel = 254;
                          else
                            VarVoiceLevel++;

                          //save_VoiceLevel();

                        }
                      break;
                    case 0x80:
                      if(BitDisplayOn)				//����״̬�²��ܽ��д˲���
                        {
                          BitUpData_chang = 1;
                          BitData_Astrict_R = 0;
                          //
                          if(VarVoiceLevel>= 254)
                            VarVoiceLevel = 254;
                          else
                            VarVoiceLevel++;

                          //save_VoiceLevel();
                        }
                      break;

                    case 0xD2:								//�����ȼ���
                      if(BitDisplayOn)				//����״̬�²��ܽ��д˲���
                        {
                          BitUpData_chang = 1;
                          BitData_Astrict_F = 0;
                          if(VarVoiceLevel== 0)
                            {
                              VarVoiceLevel = 0;
                            }
                          else
                            {
                              VarVoiceLevel--;
                            }
                          //save_VoiceLevel();

                        }
                      break;
                    case 0x81:
                      if(BitDisplayOn)				//����״̬�²��ܽ��д˲���
                        {
                          BitUpData_chang = 1;
                          BitData_Astrict_F = 0;
                          if(VarVoiceLevel== 0)
                            {
                              VarVoiceLevel = 0;
                            }
                          else
                            {
                              VarVoiceLevel--;
                            }
                          //	save_VoiceLevel();
                        }
                      break;

                    default:
                      break;
                    }

                  //
                }
            }
        }
      else
        {
          if(BitIRRXPress)	//����ް�������,˵����ʱ���ڿ���״̬.�жϰ����ͷ�.
            {
              if(++Var_IR_RX_RELEASE_TIME >= 21 )			//�����ͷ�ʱ��  ʱ���׼10ms
                {
                  Var_IR_RX_RELEASE_TIME --;

                  //
                  BitIRRXPress = 0;											//�ͷ�ʱ�䵽,�����־λ.

                }
              else
                {
                  // goto dup;
                }
              /*	else																		//���ں���100ms����һ���ź�,���Գ���ȷ�Ϻ�,��û���źŵ�ʱ��Ҳ��Ҫ���볤������.
              	{
              				if(Bit_IR_RX_LONG_PRESS)					//�г�������,ֱ��
              				{
              						switch(VarIRRxData[2])				//�˴���ӳ�������.
              						{
              							case 0x45:vattestcnt =4;break;

              								default:
              							break;
              						}
              				}
              	}*/
            }
          else
            {
              BitIRRXPress = 0;
              Bit_IR_RX_LONG_PRESS = 0;
              Var_IR_RX_RELEASE_TIME = 0;
              Var_IR_RX_PRESS_TIME = 0;

            }
        }
    }
}

/*
*********************************************************************************************************
*	�� �� ��:VoiceLevelBack_Ctrl
*	����˵��: IR  ���ƻ��������ȼ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
u8 VarBackTimeCnt;
//
void VoiceLevelBack_Ctrl(void)
{
  if(BitVoiceLevelBack == 1)
    {
      if(VarBackTimeCnt >= 1)
        {
          VarBackTimeCnt =0;
          //
          BitDisplayData_chang = 1;
          BitUpData_chang = 1;
          //
          if(VarVoiceLevel ==1)
            {
              VarVoiceLevel = 1;
              BitVoiceLevelBack = 0;
            }
          else
            {
              VarVoiceLevel--;
            }
          //
        }
      else
        {
          VarBackTimeCnt ++;
        }
    }
  else
    {
      VarBackTimeCnt = 0;
    }

}
