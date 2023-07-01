#include "all.h"


//IO电平轮询法驱动编码器


sbit 	 EC1_A = P3^0;						//定义
sbit 	 EC1_B = P3^1;						//定义
sbit 	 EC1_SW = P3^2;						//定义
//
sbit 	 EC2_A = P3^6;						//定义
sbit 	 EC2_B = P0^5;						//定义
sbit 	 EC2_SW = P2^6;						//定义

bit  BitDisplayData_chang;

sbit EC_B1 = P0 ^ 0;   // 编码器 P0.0 端口
sbit EC_A1 = P1 ^ 3;   // 编码器 P1.3 端口
static uint8_t EC_A1_Val = 0, EC_B1_Val = 0;
static uint8_t EC_A1_old = 0, EC_B1_old = 0;
u8 KEY_state_A ;


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

  BitDisplayData_chang = 1;
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
      eb_voice_output_channel = eb_voice_output_channel < 4 ? (eb_voice_output_channel+1) : 1;
      BitDisplayData_chang = 1;
      VarWirtFlashCnt = 0;
      eBit_DataCharg = 1;
    break;

    case 2://反转
      eb_voice_output_channel = eb_voice_output_channel > 1 ? (eb_voice_output_channel-1) : 4;
      BitDisplayData_chang = 1;
      VarWirtFlashCnt = 0;
      eBit_DataCharg = 1;
    break;

    default :
      /* 可选的 */
      break;

    }
}

void encoder_b(void)
{
	if(BitVoiceMute)
  {
      return;
  }

    //使用方式：
    KEY_state_B = EncoderReading_B(); //编码器
    //
    switch (KEY_state_B)
    {
    case 1://正转
      
      if(eb_voice_level > VOLUME_MIN_CLASS)
      {
          eb_voice_level--;
      }

      BitDisplayData_chang = 1;			//数据有更改标志位置1
      eb_button_change_motor_sta = 1;
      VarWirtFlashCnt = 0;
      eBit_DataCharg = 1;
    break;

    case 2://反转
      if(eb_voice_level < VOLUME_MAX_CLASS)
      {
          eb_voice_level++;
      }
  
      BitDisplayData_chang = 1;
      eb_button_change_motor_sta = 1;
      VarWirtFlashCnt = 0;
      eBit_DataCharg = 1;
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
              if ((BitVoiceMute == 0)
              {
                  BitDisplayData_chang = 1;		//显示更新标志
                  VarWirtFlashCnt = 0;
                  eBit_DataCharg = 1;						//数据保存更新标志
                  if(eb_voice_input_channel >= 2)
                    {
                      eb_voice_input_channel = 1;
                    }
                  else
                    {
                      eb_voice_input_channel ++;
                    }
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


