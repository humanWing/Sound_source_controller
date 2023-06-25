/*
*********************************************************************************************************
*
*	模块名称 :
*	文件名称 : bsp_mt.c
*	版    本 : V1.0
*	说    明 :
*
*	修改记录 :
*		版本号  日期         作者     说明
*
*********************************************************************************************************
*/
/*
INA     INB						状态
H				L							正转
L				H							反转
H				H							刹车
L				L							待机
*/


#include "all.h"

//定义IO口  马达控制

sbit 	 IN_A = P2^4;						//定义
sbit 	 IN_B = P2^5;						//定义
//音频等级对应的ADC的数据
//  VDD / 4096 = Vr /Vadc
code u16 Voice_Adc_Table[255]=
{
  0XFF6,
  0XFE6,
  0XFD8,
  0XFD0,
  0XFC0,
  0XFB1,
  0XFA1,
  0XF92,
  0XF82,
  0XF72,
  0XF63,
  0XF53,
  0XF44,
  0XF34,
  0XF25,
  0XF15,
  0XF06,
  0XEF6,
  0XEE6,
  0XED7,
  0XEC7,
  0XEB8,
  0XEA8,
  0XE99,
  0XE89,
  0XE79,
  0XE6A,
  0XE5A,
  0XE4B,
  0XE3B,
  0XE2C,
  0XE1C,
  0XE0D,
  0XDFD,
  0XDED,
  0XDDE,
  0XDCE,
  0XDBF,
  0XDAF,
  0XDA0,
  0XD90,
  0XD80,
  0XD71,
  0XD61,
  0XD52,
  0XD42,
  0XD33,
  0XD23,
  0XD14,
  0XD04,
  0XCF4,
  0XCE5,
  0XCD5,
  0XCC6,
  0XCB6,
  0XCA7,
  0XC97,
  0XC88,
  0XC78,
  0XC68,
  0XC59,
  0XC49,
  0XC3A,
  0XC2A,
  0XC1B,
  0XC0B,
  0XBFB,
  0XBEC,
  0XBDC,
  0XBCD,
  0XBBD,
  0XBAE,
  0XB9E,
  0XB8F,
  0XB7F,
  0XB6F,
  0XB60,
  0XB50,
  0XB41,
  0XB31,
  0XB22,
  0XB12,
  0XB02,
  0XAF3,
  0XAE3,
  0XAD4,
  0XAC4,
  0XAB5,
  0XAA5,
  0XA96,
  0XA86,
  0XA76,
  0XA67,
  0XA57,
  0XA48,
  0XA38,
  0XA29,
  0XA19,
  0XA0A,
  0X9FA,
  0X9EA,
  0X9DB,
  0X9CB,
  0X9BC,
  0X9AC,
  0X99D,
  0X98D,
  0X97D,
  0X96E,
  0X95E,
  0X94F,
  0X93F,
  0X930,
  0X920,
  0X911,
  0X901,
  0X8F1,
  0X8E2,
  0X8D2,
  0X8C3,
  0X8B3,
  0X8A4,
  0X894,
  0X884,
  0X875,
  0X865,
  0X856,
  0X846,
  0X837,
  0X827,
  0X818,
  0X808,
  0X7F8,
  0X7E9,
  0X7D9,
  0X7CA,
  0X7BA,
  0X7AB,
  0X79B,
  0X78C,
  0X77C,
  0X76C,
  0X75D,
  0X74D,
  0X73E,
  0X72E,
  0X71F,
  0X70F,
  0X6FF,
  0X6F0,
  0X6E0,
  0X6D1,
  0X6C1,
  0X6B2,
  0X6A2,
  0X693,
  0X683,
  0X673,
  0X664,
  0X654,
  0X645,
  0X635,
  0X626,
  0X616,
  0X606,
  0X5F7,
  0X5E7,
  0X5D8,
  0X5C8,
  0X5B9,
  0X5A9,
  0X59A,
  0X58A,
  0X57A,
  0X56B,
  0X55B,
  0X54C,
  0X53C,
  0X52D,
  0X51D,
  0X50E,
  0X4FE,
  0X4EE,
  0X4DF,
  0X4CF,
  0X4C0,
  0X4B0,
  0X4A1,
  0X491,
  0X481,
  0X472,
  0X462,
  0X453,
  0X443,
  0X434,
  0X424,
  0X415,
  0X405,
  0X3F5,
  0X3E6,
  0X3D6,
  0X3C7,
  0X3B7,
  0X3A8,
  0X398,
  0X388,
  0X379,
  0X369,
  0X35A,
  0X34A,
  0X33B,
  0X32B,
  0X31C,
  0X30C,
  0X2FC,
  0X2ED,
  0X2DD,
  0X2CE,
  0X2BE,
  0X2AF,
  0X29F,
  0X290,
  0X280,
  0X270,
  0X261,
  0X251,
  0X242,
  0X232,
  0X223,
  0X213,
  0X203,
  0X1F4,
  0X1E4,
  0X1D5,
  0X1C5,
  0X1B6,
  0X1A6,
  0X197,
  0X187,
  0X177,
  0X168,
  0X158,
  0X149,
  0X139,
  0X12A,
  0X11A,
  0X10A,
  0XFB,
  0XEB,
  0XDC,
  0XCC,
  0XBD,
  0XAD,
  0X9E,
  0X8E
};

#define MOTOR_ANTICLOCKWISE_TURN  0
#define MOTOR_CLOCKWISE_TURN      1
#define MOTOR_STOP_TURN           0xff

u16 VarADData_Vlaue = 0;
uint8_t BitMTDirection_Forward = MOTOR_STOP_TURN;
uint8_t gb_motor_power_collation = 0;
uint8_t eb_button_change_motor_sta = 0;
/*
*********************************************************************************************************
*	函 数 名:
*	功能说明: 配置
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void mt_init(void)
{
  //待机
  IN_A = 0;
  IN_B = 0;
  //
  GPIO_SET_MUX_MODE(P24CFG, GPIO_MUX_GPIO);
  GPIO_ENABLE_OUTPUT(P2TRIS, GPIO_PIN_4);

  GPIO_SET_MUX_MODE(P25CFG, GPIO_MUX_GPIO);
  GPIO_ENABLE_OUTPUT(P2TRIS, GPIO_PIN_5);
}

/*
*********************************************************************************************************
*	函 数 名:
*	功能说明: 配置
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
//************************************************************* */
//函数名称：MotorForward()
//函数功能：设置电机正转的角度
//入口参数：无
//出口参数：无
//************************************************************** */
void MotorForward(void)
{
  IN_A = 1;
  IN_B = 0;
}
//************************************************************* */
//函数名称：MotorCcwDegre()
//函数功能：设置电机反转的角度
//入口参数：无
//出口参数：无
//************************************************************** */
void MotorReverse(void)
{
  IN_A = 0;
  IN_B = 1;
}

//************************************************************* */
//函数名称：MotorCcwDegre()
//函数功能：设置电机刹车
//入口参数：无
//出口参数：无
//************************************************************** */
void MotorStop(void)
{
  IN_A = 1;
  IN_B = 1;
}

/*
*********************************************************************************************************
*	函 数 名:mt_ctrl()
*	功能说明: 根据设置的Voice的等级，调整电机输出还是停止。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void mt_ctrl(void)
{
    #if 1
  static u8 VarVoiceLevel_Back = 0;
  //
  if (gb_motor_power_collation == 0)
  {
      VarADData_Vlaue = GetADValue(0, 8);
      
      if (VarADData_Vlaue > Voice_Adc_Table[VOLUME_MAX_CLASS - eb_voice_level] + 50)
      {
          BitMTDirection_Forward = MOTOR_ANTICLOCKWISE_TURN;
      }
      else if (VarADData_Vlaue < Voice_Adc_Table[VOLUME_MAX_CLASS - eb_voice_level] - 50)
      {
          BitMTDirection_Forward = MOTOR_CLOCKWISE_TURN;
      }
      else
      {
          BitMTDirection_Forward = MOTOR_STOP_TURN;
      }

      gb_motor_power_collation = 1;
      VarVoiceLevel_Back = eb_voice_level;
  }
  else
  {
      if(eb_button_change_motor_sta)			//数据有变化
      {
          eb_button_change_motor_sta = 0;
          
          MotorStop();
          VarADData_Vlaue = GetADValue(0, 8);

          if (eb_voice_level > VarVoiceLevel_Back )
          {
              BitMTDirection_Forward = MOTOR_CLOCKWISE_TURN;
          }
          else if (eb_voice_level < VarVoiceLevel_Back)
          {
              BitMTDirection_Forward = MOTOR_ANTICLOCKWISE_TURN;
          }
          else
          {
              BitMTDirection_Forward = MOTOR_STOP_TURN;
          }

          VarVoiceLevel_Back = eb_voice_level;				//保存上一次得等级值，防止出现等级增加，采集到AD值导致反转。
      }
  }
#if (DEBUG_AD_SAMPLE_FUNCTION == THIS_FUNCTION_DISABLE)
  if(BitMTDirection_Forward == MOTOR_CLOCKWISE_TURN)
  {
      MotorStop();
      VarADData_Vlaue = GetADValue(0, 8);

      if(VarADData_Vlaue < (Voice_Adc_Table[VOLUME_MAX_CLASS - VarVoiceLevel_Back] - 10))
      {
          MotorForward();
      }
      else
      {
          BitMTDirection_Forward = MOTOR_STOP_TURN;
      }
  }
  else if(BitMTDirection_Forward == MOTOR_ANTICLOCKWISE_TURN)
  {
      MotorStop();
      VarADData_Vlaue = GetADValue(0, 8);
      
      if(VarADData_Vlaue > (Voice_Adc_Table[VOLUME_MAX_CLASS - VarVoiceLevel_Back] + 10) )
      {
          MotorReverse();
      }
      else
      {
          BitMTDirection_Forward = MOTOR_STOP_TURN;
      }
  }
  else
  {
    
  }
#else
    VarADData_Vlaue = GetADValue(0, 8);
#endif
    #else
    static uint16_t test12=100;
    --test12;

    if (test12 < 100)
    {
        MotorReverse();
    }
    if (test12 < 50)
    {
        if (test12 == 0)
        {
            test12 = 100;
        }
        MotorForward();
    }
    VarADData_Vlaue = GetADValue(0, 8);
    #endif
}


