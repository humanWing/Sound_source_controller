#include "all.h"

//IO电平轮询法驱动编码器


sbit 	 EC1_A = P3^0;						//定义
sbit 	 EC1_B = P3^1;						//定义
sbit 	 EC1_SW = P3^2;						//定义
//
sbit 	 EC2_A = P3^6;						//定义
sbit 	 EC2_B = P0^5;						//定义
sbit 	 EC2_SW = P2^6;						//定义

bit BitDisplayData_chang;

//https://whycan.com/t_6692.html
sbit EC_B1 = P0 ^ 0;   // 编码器 P0.0 端口
sbit EC_A1 = P1 ^ 3;   // 编码器 P1.3 端口
static uint8_t EC_A1_Val = 0, EC_B1_Val = 0;
static uint8_t EC_A1_old = 0, EC_B1_old = 0;
u8 KEY_state_A ;
//
sbit EC_B2 = P3 ^ 6;   // 编码器 P3.6 端口
sbit EC_A2 = P0 ^ 5;   // 编码器 P0.5 端口
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
  //获取初始状态，保存起来。
  EC_B2_old = EC2_A;
  EC_A2_old = EC2_B;

  EC_B1_old = EC1_A;
  EC_A1_old = EC1_B;

}
#if 1




/*********************************************
函数名：EncoderReading
功  能：读取编码器数据
形  参：
返回值：0--无动作 1--正转  2--反转
备  注：
作  者：薛建强
时  间：2019/06/11
**********************************************/
u8 EncoderReading_A(void)
{
  /*捕获一次*/
  EC_A1_Val = EC1_A;
  EC_B1_Val = EC1_B;
  /*************EC_B上升沿*******************/
  if (EC_A1_Val == EC_A1_old && EC_B1_Val > EC_B1_old)
    {
      /*重新给旧值赋值*/
      EC_B1_old = EC_B1_Val;
      EC_A1_old = EC_A1_Val;
      if (EC_A1_Val == 0) //反转
        {
          return 2;
        }
    }
  /*************EC_B下升沿*******************/
  if (EC_A1_Val == EC_A1_old && EC_B1_Val < EC_B1_old)
    {
      /*重新给旧值赋值*/
      EC_B1_old = EC_B1_Val;
      EC_A1_old = EC_A1_Val;
      if (EC_A1_Val == 1) //反转
        {
          return 2;
        }
    }
  /*************EC_A上升沿*******************/
  if (EC_B1_Val == EC_B1_old && EC_A1_Val > EC_A1_old)
    {
      /*重新给旧值赋值*/
      EC_B1_old = EC_B1_Val;
      EC_A1_old = EC_A1_Val;
      if (EC_B1_Val == 0) //正转
        {
          return 1;
        }
    }
  /*************EC_A下升沿*******************/
  if (EC_B1_Val == EC_B1_old && EC_A1_Val < EC_A1_old)
    {
      /*重新给旧值赋值*/
      EC_B1_old = EC_B1_Val;
      EC_A1_old = EC_A1_Val;
      if (EC_B1_Val == 1) //正转
        {
          return 1;
        }
    }
  return 0;
}
////////////////////////////
u8 EncoderReading_B(void)
{
  /*捕获一次*/
  EC_A2_Val = EC2_A;
  EC_B2_Val = EC2_B;
  /*************EC_B上升沿*******************/
  if (EC_A2_Val == EC_A2_old && EC_B2_Val > EC_B2_old)
    {
      /*重新给旧值赋值*/
      EC_B2_old = EC_B2_Val;
      EC_A2_old = EC_A2_Val;
      if (EC_A2_Val == 0) //反转
        {
          return 2;
        }
    }
  /*************EC_B下升沿*******************/
  if (EC_A2_Val == EC_A2_old && EC_B2_Val < EC_B2_old)
    {
      /*重新给旧值赋值*/
      EC_B2_old = EC_B2_Val;
      EC_A2_old = EC_A2_Val;
      if (EC_A2_Val == 1) //反转
        {
          return 2;
        }
    }
  /*************EC_A上升沿*******************/
  if (EC_B2_Val == EC_B2_old && EC_A2_Val > EC_A2_old)
    {
      /*重新给旧值赋值*/
      EC_B2_old = EC_B2_Val;
      EC_A2_old = EC_A2_Val;
      if (EC_B2_Val == 0) //正转
        {
          return 1;
        }
    }
  /*************EC_A下升沿*******************/
  if (EC_B2_Val == EC_B2_old && EC_A2_Val < EC_A2_old)
    {
      /*重新给旧值赋值*/
      EC_B2_old = EC_B2_Val;
      EC_A2_old = EC_A2_Val;
      if (EC_B2_Val == 1) //正转
        {
          return 1;
        }
    }
  return 0;
}


//////////////////
void encoder_a(void)
{
  //使用方式：
  KEY_state_A = EncoderReading_A(); //编码器调光度
  switch (KEY_state_A)
    {
    case 1://正转
      BitDisplayData_chang = 1;

      //输出通道等级
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
    case 2://反转
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
      /* 可选的 */
      break;

    }
}
void encoder_b(void)
{

  //使用方式：
  KEY_state_B = EncoderReading_B(); //编码器
	if(BitVoiceMute)  return ;				//静音时退出。
	//
  switch (KEY_state_B)
    {
    case 1://正转
      BitDisplayData_chang = 1;			//数据有更改标志位置1
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
    case 2://反转
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
      /* 可选的 */
      break;
    }
}


/*
*********************************************************************************************************
*	函 数 名: Scan_encodeer_a
*	功能说明: 编码器独立按键
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void Scan_encodeer_a(void)
{
  static u8		VarK1PressCnt;
  static u8	 	VarK1ReleaseCnt;
  if(EC1_SW == 0)
    {
      //按下
      VarK1ReleaseCnt = 0;
      //
      VarK1PressCnt ++;
      if(VarK1PressCnt > 3)
        {
          VarK1PressCnt--;
          if(BitK1LastStatus == 0)
            {
              BitK1LastStatus = 1;
              BitDisplayData_chang = 1;		//显示更新标志
              //
              BitDataCharg = 1;						//数据保存更新标志
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
      //如果按键释放
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
*	函 数 名: Scan_encodeer_b
*	功能说明: 编码器独立按键
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void Scan_encodeer_b(void)
{
  static u8		VarK2PressCnt;
  static u8	 	VarK2ReleaseCnt;
  if(EC2_SW == 0)
    {
      //按下
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
              //控制禁用音频
              BitVoiceMute = ~BitVoiceMute;
            }
        }
    }
  else
    {
      //如果按键释放
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


