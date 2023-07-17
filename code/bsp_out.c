/*
*********************************************************************************************************
*
*	模块名称 :
*	文件名称 : bsp_out.c
*	版    本 : V1.0
*	说    明 :
*
*	修改记录 :
*		版本号  日期         作者     说明
*
*********************************************************************************************************
*/
#include "all.h"

//控制ULN2003 达林顿管   H-打开   L-关闭
//定义IO口  2进4出
sbit 	 IN_1 = P2^3;						//定义
sbit 	 IN_2 = P2^2;						//定义
//
sbit 	 OUT_1 = P1^6;						//定义
sbit 	 OUT_2 = P1^5;						//定义
sbit 	 OUT_3 = P1^4;						//定义
sbit 	 OUT_4 = P1^3;						//定义

sbit   MUTE =  P1^7;						//静音控制  H 静音  L无效


/*
*********************************************************************************************************
*	函 数 名: out_init
*	功能说明: 配置IO
*	形    参: 无
*	返 回 值: 无
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
*	函 数 名: out_ctrl
*	功能说明: 根据设定值，来进行打开关闭输入 输出
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void out_ctrl()
{
  if(!BitVoiceMute)		//开启状态下进入
    {
      //输入通道控制
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
      //输出通道控制
				//add :  选择那组通道，其他组关闭
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
      OUT_1 = 0;					//关闭
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
*	函 数 名: Mute_ctrl
*	功能说明: 根据设定值，打开/关闭静音
*	形    参: 无
*	返 回 值: 无
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

  if(BitVoiceMute)		//静音标志
  {
      MUTE = 1;				//禁用
  }
  else
  {
      MUTE = 0;				//打开
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
    //控制禁用音频
    BitVoiceMute = ~BitVoiceMute;
}
