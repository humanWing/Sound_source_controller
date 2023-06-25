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
  22,
  38,
  54,
  70,
  86,
  102,
  118,
  134,
  150,
  166,
  182,
  198,
  214,
  230,
  246,
  262,
  278,
  294,
  310,
  326,
  342,
  358,
  374,
  390,
  406,
  422,
  438,
  454,
  470,
  486,
  502,
  518,
  534,
  550,
  566,
  582,
  598,
  614,
  630,
  646,
  662,
  678,
  694,
  710,
  726,
  742,
  758,
  774,
  790,
  806,
  822,
  838,
  854,
  870,
  886,
  902,
  918,
  934,
  950,
  966,
  982,
  998,
  1014,
  1030,
  1046,
  1062,
  1078,
  1094,
  1110,
  1126,
  1142,
  1158,
  1174,
  1190,
  1206,
  1222,
  1238,
  1254,
  1270,
  1286,
  1302,
  1318,
  1334,
  1350,
  1366,
  1382,
  1398,
  1414,
  1430,
  1446,
  1462,
  1478,
  1494,
  1510,
  1526,
  1542,
  1558,
  1574,
  1590,
  1606,
  1622,
  1638,
  1654,
  1670,
  1686,
  1702,
  1718,
  1734,
  1750,
  1766,
  1782,
  1798,
  1814,
  1830,
  1846,
  1862,
  1878,
  1894,
  1910,
  1926,
  1942,
  1958,
  1974,
  1990,
  2006,
  2022,
  2038,
  2054,
  2070,
  2086,
  2102,
  2118,
  2134,
  2150,
  2166,
  2182,
  2198,
  2214,
  2230,
  2246,
  2262,
  2278,
  2294,
  2310,
  2326,
  2342,
  2358,
  2374,
  2390,
  2406,
  2422,
  2438,
  2454,
  2470,
  2486,
  2502,
  2518,
  2534,
  2550,
  2566,
  2582,
  2598,
  2614,
  2630,
  2646,
  2662,
  2678,
  2694,
  2710,
  2726,
  2742,
  2758,
  2774,
  2790,
  2806,
  2822,
  2838,
  2854,
  2870,
  2886,
  2902,
  2918,
  2934,
  2950,
  2966,
  2982,
  2998,
  3014,
  3030,
  3046,
  3062,
  3078,
  3094,
  3110,
  3126,
  3142,
  3158,
  3174,
  3190,
  3206,
  3222,
  3238,
  3254,
  3270,
  3286,
  3302,
  3318,
  3334,
  3350,
  3366,
  3382,
  3398,
  3414,
  3430,
  3446,
  3462,
  3478,
  3494,
  3510,
  3526,
  3542,
  3558,
  3574,
  3590,
  3606,
  3622,
  3638,
  3654,
  3670,
  3686,
  3702,
  3718,
  3734,
  3750,
  3766,
  3782,
  3798,
  3814,
  3830,
  3846,
  3862,
  3878,
  3894,
  3910,
  3926,
  3942,
  3958,
  3974,
  3990,
  4006,
  4022,
  4038,
  4054,
  4070,
  4086
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
      
      if (VarADData_Vlaue > Voice_Adc_Table[VOLUME_MAX_CLASS - eb_voice_level] + 10)
      {
          BitMTDirection_Forward = MOTOR_ANTICLOCKWISE_TURN;
      }
      else if (VarADData_Vlaue < Voice_Adc_Table[VOLUME_MAX_CLASS - eb_voice_level] - 10)
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
              BitMTDirection_Forward = MOTOR_ANTICLOCKWISE_TURN;
          }
          else if (eb_voice_level < VarVoiceLevel_Back)
          {
              BitMTDirection_Forward = MOTOR_CLOCKWISE_TURN;
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

      if(VarADData_Vlaue < (Voice_Adc_Table[VOLUME_MAX_CLASS - VarVoiceLevel_Back]))
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
      
      if(VarADData_Vlaue > (Voice_Adc_Table[VOLUME_MAX_CLASS - VarVoiceLevel_Back]) )
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


