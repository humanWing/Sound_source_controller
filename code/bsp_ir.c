/*********************************************************************************************************
*
*   模块名称 : IR驱动模块
*   说    明 : 100US 定时器中调用。
*********************************************************************************************************/
#include "all.h"

//定义的变量
u8  VarIRRxData[ConIRRxDataCout];       			//接收数据BUF

bit BitIRRXPress;
bit BitIRRX_9MS;                              //9MS低电平接收完成标志
bit BitIRRX_4D5MS;                            //4.5MS高电平接收完成标志
bit BitIRRX_DUP;                                        //重复码标志.
bit BitIRRX_FREE;                                       //IR接收进行标志  1:正在接收中
bit BitIRRX_FINISH;                                			//接收完成标志
bit BitIRRX_LEVEL;
u8 VarIRRX_DATA;
u8 VarIRRX_CNT;                                         //;IR接收数据计数,总共4字节,32bit.
u8 VarIRRX_TIME;                                        //;IR接收时间计数.
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
  if (!BitIRRX_FREE)                                  //是否是释放状态
    {
      if (!P_IR_RX)                                   //低电平启动
        {
          //
          VarIRRX_DATA = 0;                           //保存的数据
          //
          BitIRRX_LEVEL = 0;
          //
          VarIRRX_TIME = 0;                           //计数时间
          BitIRRX_9MS = 0;
          BitIRRX_4D5MS = 0;
          BitIRRX_DUP = 0;
          BitIRRX_FINISH = 0;
          VarIRRX_CNT  = 0;                           //32bit -4个字节
          VarIRRxBitCnt = 0;
          //
          BitIRRX_FREE  = 1;                          //置1--;接收到低电平,启动接收

        }
    }
  else
    {
      if(BitIRRX_DUP)										//重复码标志位
        {
          //
          if (P_IR_RX == 0)																		//电平一直为低
            {
              if (++VarIRRX_TIME >= ConIRRXLOW_OVER)						//且大于OVER时间，释放
                {
                  BitIRRX_FREE = 0;
                  return ;                                    //退出
                }
            }
        }
      else if (BitIRRX_4D5MS)                                 //数据
        {
          if (BitIRRX_FINISH)                                 //接收成功后,如果接收到电平.强制释放接收状态.
            {
              if (!P_IR_RX)
                {
                  if (VarIRRX_TIME >= ConIRRXLOW_OVER) 				//强制释放接收状态.
                    {
                      BitIRRX_FREE = 0;
                      return ;                            		//退出
                    }
                }
              else
                {
                  BitIRRX_FREE = 0;
                  return ;                           				 //退出
                }
            }
          else                                               //32个bit数据 接收处理
            {
              if (P_IR_RX)
                {
                  if (!BitIRRX_LEVEL)                       //电平状态标志位
                    {
                      BitIRRX_LEVEL = 1;                    //低电平变高电平的时候,清除时间计数,重新开始计高电平时间
                      VarIRRX_TIME = 0;
                    }

                  VarIRRX_TIME ++;

                  if (VarIRRX_TIME >= ConIRRXHIGH_OVER)       //如果高电平时间超过,   说明接收数据错误,强制释放接收状态.
                    {
                      BitIRRX_FREE = 0;                       //释放
                      //
                      return ;                                //退出
                    }
                }
              else                                            //状态转成L电平时，再判断数据是 1还是0
                {
                  if (BitIRRX_LEVEL)                          //
                    {
                      BitIRRX_LEVEL = 0;
                      //
                      //判断数据
                      VarIRRX_DATA = VarIRRX_DATA >> 1;       //从低位开始接收，右移一位

                      if (VarIRRX_TIME  > ConIR_RX_01)        //时间>1ms,说明接收到高电平,
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

                          if (++VarIRRX_CNT >= 4)         //32个字节接收完毕
                            {
                              VarIRRX_CNT = 0;

                              if ((VarIRRxData[0] == 0xB2) && (VarIRRxData[1] == 0x4D))   //判断数据
                                {
                                  BitIRRX_FINISH = 1;                         //接收完成
                                }

                              return ;                                                    //退出循环

                            }
                        }

                      VarIRRX_TIME    =   0;
                    }

                  //
                  VarIRRX_TIME ++;

                  if (VarIRRX_TIME >= ConIRRXLOW_OVER)    //低电平时间计数+1
                    {
                      BitIRRX_FREE = 0;                   //>0.5ms--强制释放
                      return ;                            //退出
                    }
                }


            }
        }
      else if (BitIRRX_9MS)
        {
          //4.5MS
          if (P_IR_RX)                                 //高电平进入
            {
              VarIRRX_TIME ++;                         //接收到高电平,低电平计数时间+1
              //
              if (VarIRRX_TIME >= ConIRRxOVER_4D5ms)  //强制释放接收状态.
                {
                  BitIRRX_FREE = 0;                               //
                  return ;                            //退出
                }
            }
          else                                                                    //
            {
              if (VarIRRX_TIME < ConIR_RX_DUP)        //接收时间小于2.6ms,说明接收数据是重复码
                {
                  BitIRRX_DUP = 1;                    //如果接收到高电平时强制释放,9ms和4.5ms接收正常,并且没有接收到数据,认为接收到重复码.
                  VarIRRX_TIME = 0;
                }
              else if (VarIRRX_TIME > ConIRRx_4D5ms)
                {

                  BitIRRX_4D5MS  = 1;                 //接收时间>4.5ms,说明4.5MS时间接收正确.
                  VarIRRX_TIME = 0;

                }
              else
                {
                  BitIRRX_FREE = 0;                               //强制释放
                  return ;                            //退出
                }
            }
        }
      else
        {
          //;接收9ms低电平.
          if (!P_IR_RX)                                                   //低电平进入
            {
              VarIRRX_TIME ++;                                        //接收到低电平,低电平计数时间+1
              //
              if (VarIRRX_TIME >= ConIRRxOVER_9ms) //强制释放接收状态.
                {
                  BitIRRX_FREE = 0;                               //
                  return ;                            //退出
                }
            }
          else                                                                  //电平状态为高--此时判断VarIRRX_TIME
            {
              if (VarIRRX_TIME >   ConIRRx_9ms)       //接收时间>8.5ms,说明9MS时间接收正确.
                {
                  VarIRRX_TIME = 0;
                  BitIRRX_9MS = 1;
                }
              else                                                            //接收时间小于8.5ms,说明接收数据错误,强制释放接收状态
                {
                  BitIRRX_FREE = 0;
                  return ;                            //退出
                }
            }
        }
    }
}

/*============================================================================
*红外接收处理
由红外接收程序传递的标志 F_IR_RX_FINISH, F_IR_RX_DUP 判断按键状态.
;由红外接收程序传递的数据 R_IR_RX_DATA1( 用户码 )   R_IR_RX_DATA3( 数据 )判断按键位号.
============================================================================*/

void IRReceiveCTRL(void)
{
  u8  tmp1;
  u8  IR_RX_VALUE;
  //
  if(++Var_IR_RX_PRESS_TIME >= 255)		//长按计数时间+1
    Var_IR_RX_PRESS_TIME --;
  //
  if (BitIRRX_FINISH)								 //检测红外按下.进入第一次红外接收处理.  红外只是第一次才有数据.
    {
      BitIRRX_FINISH = 0;
      //
      //验证数据是否时正确
      tmp1 = VarIRRxData[3] ^ 0xFF;

      if (VarIRRxData[2] == tmp1)
        {
          Var_IR_RX_RELEASE_TIME = 0;		//接收释放时间清0.
          Var_IR_RX_PRESS_TIME = 0;			//按键计数时间清0.
          //
          BitIRRXPress = 1;							//置红外按下标志
          IR_RX_VALUE =VarIRRxData[2] ;//用另一个变量保存数据
          //
          BitDisplayData_chang = 1;
          //
          switch (IR_RX_VALUE)				//判断数据
            {
            case 0xDC:								//IR开关机
              BitDisplayOn = ~BitDisplayOn;

              //
              if(BitDisplayOn)
                {
                  SysStatus = 0;					//IR开机后-- 切换到倒计时
                  VarAutoSet = 3;
                  VarAutoTimeCnt = 0;
                  VarDisplayUpdateCnt = 0;
                }
              else
                {
                  BitDisplayData_chang = 1;

                }//
              break;
            case 0x82:								//静音
              if(BitDisplayOn)				//开启状态下才能进行此操作
                {
                  BitVoiceMute = ~BitVoiceMute;
                  BitDisplayData_chang = 1;
                }
              break;
            case 0xCA:								//音量等级加
              if(BitDisplayOn)				//开启状态下才能进行此操作
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
              if(BitDisplayOn)				//开启状态下才能进行此操作
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

            case 0xD2:								//音量等级减
              if(BitDisplayOn)				//开启状态下才能进行此操作
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
              if(BitDisplayOn)				//开启状态下才能进行此操作
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
            case 0x99:								//输入
              if(BitDisplayOn)				//开启状态下才能进行此操作
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

            case 0xC1:						//输出通道等级
              if(BitDisplayOn)				//开启状态下才能进行此操作
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
              if(BitDisplayOn)				//开启状态下才能进行此操作
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

            case 0xC5:								//切换回到初始音量等级
              if(BitDisplayOn)				//开启状态下才能进行此操作
                {
                  BitVoiceLevelBack = 1;	//回退标志位
                }
              break;

            default:
              break;
            }

        }

    }
  else
    {
      //检测到重复码,进行重复码处理,主要处理长按动作.
      if(BitIRRX_DUP)
        {
          BitIRRX_DUP = 0;
          //检测到重复码之前必须先检测到红外码,否则强制释放.
          if(BitIRRXPress)
            {
              Var_IR_RX_RELEASE_TIME = 0;			//接收释放时间清0
              //
              if(Var_IR_RX_PRESS_TIME >= 120 )//时间基值 10ms
                {
                  Var_IR_RX_PRESS_TIME --;
                  //
                  Bit_IR_RX_LONG_PRESS	 = 1;		//长按动作标志置1
                  //
//dup:
                  BitDisplayData_chang = 1;
                  switch(VarIRRxData[2])				//此处添加长按动作.
                    {
                    case 0xCA:								//音量等级加
                      if(BitDisplayOn)				//开启状态下才能进行此操作
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
                      if(BitDisplayOn)				//开启状态下才能进行此操作
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

                    case 0xD2:								//音量等级减
                      if(BitDisplayOn)				//开启状态下才能进行此操作
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
                      if(BitDisplayOn)				//开启状态下才能进行此操作
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
          if(BitIRRXPress)	//如果无按键按下,说明此时出于空闲状态.判断按键释放.
            {
              if(++Var_IR_RX_RELEASE_TIME >= 21 )			//红外释放时间  时间基准10ms
                {
                  Var_IR_RX_RELEASE_TIME --;

                  //
                  BitIRRXPress = 0;											//释放时间到,清除标志位.

                }
              else
                {
                  // goto dup;
                }
              /*	else																		//由于红外100ms接收一次信号,所以长按确认后,在没有信号的时候也需要进入长按处理.
              	{
              				if(Bit_IR_RX_LONG_PRESS)					//有长按动作,直接
              				{
              						switch(VarIRRxData[2])				//此处添加长按动作.
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
*	函 数 名:VoiceLevelBack_Ctrl
*	功能说明: IR  控制回退音量等级
*	形    参: 无
*	返 回 值: 无
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
