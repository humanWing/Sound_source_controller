/*******************************************************************************
* Copyright (C) 2019 China Micro Semiconductor Limited Company. All Rights Reserved.
*
* This software is owned and published by:
* CMS LLC, No 2609-10, Taurus Plaza, TaoyuanRoad, NanshanDistrict, Shenzhen, China.
*
* BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND
* BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
*
* This software contains source code for use with CMS
* components. This software is licensed by CMS to be adapted only
* for use in systems utilizing CMS components. CMS shall not be
* responsible for misuse or illegal use of this software for devices not
* supported herein. CMS is providing this software "AS IS" and will
* not be responsible for issues arising from incorrect user implementation
* of the software.
*
* This software may be replicated in part or whole for the licensed use,
* with the restriction that this Disclaimer and Copyright notice must be
* included with each copy of this software, whether used in part or whole,
* at all times.
*/

/****************************************************************************/
/** \file main.c
**
**
** 初版	2023年01月01日 21:59:53
**	History:
**
*****************************************************************************/
/****************************************************************************/
/*	include files
*****************************************************************************/

#include "all.h"
/****************************************************************************/
/*	Local pre-processor symbols('#define')
*****************************************************************************/
extern u8 xdata 	BitTimer1ms;

/****************************************************************************/
/*	Global variable definitions(declared in header file with 'extern')
全局变量
*****************************************************************************/
static u8  xdata 	VarTimer10ms;
static u16 xdata  VarWirtFlashCnt;


u8 xdata eb_voice_input_channel;				//输入通道
u8 xdata eb_voice_output_channel;				//输出通道
u8 xdata eb_voice_level;		            //音频等级

u8 xdata eBit_DataCharg = 0;
/****************************************************************************/
/*	Local type definitions('typedef')
*****************************************************************************/

/****************************************************************************/
/*	Local variable  definitions('static')
*****************************************************************************/


/****************************************************************************/
/*	Local function prototypes('static')
*****************************************************************************/


/**************************************************
*函数名称：void TMR0_Config(void)
*函数功能：定时器TMR0初始化
*入口参数：void
*出口参数：void
**************************************************/
void TMR0_Config(void)
{

  /*
  (1)设置Timer的运行模式
  */
  TMR_ConfigRunMode(TMR0, TMR_MODE_TIMING,TMR_TIM_AUTO_8BIT);
  /*
  (2)设置Timer 运行时钟
  */
  //	/*Fsys = 12Mhz，Ftimer = 1Mhz,Ttmr= 1us*/
  TMR_ConfigTimerClk(TMR0, TMR_CLK_DIV_12);						/*Fsys = 24Mhz，Ftimer = 2Mhz,Ttmr=0.5us*/
  /*
  (3)设置Timer周期
  */
  TMR_ConfigTimerPeriod(TMR0, 256-200, 256-200); 				// 200*0.5us = 100us,递增计数
  //TMR_ConfigTimerPeriod(TMR0, 256-100, 256-100); 					//100*1us = 100us
  /*
  (4)开启中断
  */
  TMR_EnableOverflowInt(TMR0);

  /*
  (5)设置Timer中断优先级
  */
  IRQ_SET_PRIORITY(IRQ_TMR0,IRQ_PRIORITY_LOW);
  //IRQ_ALL_ENABLE();

  /*
  (6)开启Timer
  */
  TMR_Start(TMR0);
}

void TMR1_Config(void)
{

  /*
  (1)设置Timer的运行模式
  */
  TMR_ConfigRunMode(TMR1, TMR_MODE_TIMING, TMR_TIM_16BIT);
  /*
  (2)设置Timer 运行时钟
  */
  //	/*Fsys = 12Mhz，Ftimer = 1Mhz,Ttmr= 1us*/
  TMR_ConfigTimerClk(TMR1, TMR_CLK_DIV_12);						/*Fsys = 24Mhz，Ftimer = 2Mhz,Ttmr=0.5us*/
  /*
  (3)设置Timer周期
  */
  TMR_ConfigTimerPeriod(TMR1, 0, 0); 				// 200*0.5us = 100us,递增计数
  //TMR_ConfigTimerPeriod(TMR0, 256-100, 256-100); 					//100*1us = 100us
  /*
  (4)开启中断
  */
  // TMR_EnableOverflowInt(TMR1);

  /*
  (5)设置Timer中断优先级
  */
  // IRQ_SET_PRIORITY(IRQ_TMR1, IRQ_PRIORITY_HIGH);
  //IRQ_ALL_ENABLE();

  /*
  (6)开启Timer
  */
  // TMR_Start(TMR0);
}

/******************************************************************************
 ** \brief	 WDT_Config
 ** \param [in]
 **
 ** \return  none
 ** \note
 ******************************************************************************/
void WDT_Config(void)
{
  /*
  (1)喂狗，避免WDT时间太短，进入WDT复位
  */
  WDT_ClearWDT();
  /*
  (2)开启WDT溢出时间
  */
  WDT_ConfigOverflowTime(WDT_CLK_4194304);	// Fsys=24Mhz -> Twdt = 4194304/24 = 174.76ms
  /*
  (3)设置WDT溢出中断
  */
  //WDT_EnableOverflowInt();

  //IRQ_SET_PRIORITY(IRQ_WDT, IRQ_PRIORITY_HIGH);
  //IRQ_ALL_ENABLE();

}
/****************************************************************************/
/*	Function implementation - global ('extern') and local('static')
*****************************************************************************/

/*****************************************************************************
 ** \brief	 main
 **
 ** \param [in]  none
 **
 ** \return 0
 *****************************************************************************/
void reset_parameter(void)
{
    VarTimer10ms    = 0;
    VarWirtFlashCnt = 0;
    eBit_DataCharg  = 0;

    BitVoiceMute = 0;			//音量静音标志

}

int main(void)
{
  EA = 0;
  bsp_tm1620_init();		//显示
  encoder_init();				//编码器初始化
  IR_Init();						//IR初始化
  mt_init();					  //马达驱动
  out_init();						//输入输出通道设置
  First_ReadData();			//读取保存数据
  adc_config();					//adc初始化
  TMR0_Config();				//定时器初始化
  TMR1_Config();
// WDT_Config();

  reset_parameter();
  
  EA = 1;								//开启全局中断

  while(1)
  {
      // WDT_ClearWDT();								  //看门狗喂狗
      encoder_a();										//旋转编码器
      encoder_b();

      if(BitTimer1ms)											//1ms--定时时基
      {
          BitTimer1ms = 0;
          
          bsp_1620_update_display();											//显示刷新

          if(++VarTimer10ms >= 10)		//10ms
          {
              VarTimer10ms = 0;

              Scan_encodeer_a();			//独立按键
              Scan_encodeer_b();
              out_ctrl();							//输入输出控制
              Mute_ctrl();						//静音控制--默认关闭
              mt_ctrl();							//马达控制
          }

          if ((eBit_DataCharg == 1)
            && (++VarWirtFlashCnt >= 500))
          {
              VarWirtFlashCnt = 0;
              eBit_DataCharg = 0;
              //
              FLASH_UnLock();						//解锁
              FLASH_Erase(FLASH_DATA,0);//擦除对应所在这一页
              FLASH_Write(FLASH_DATA,0x0, 0x55);			//写入标志
              FLASH_Write(FLASH_DATA,0x1, 0x2a);
              FLASH_Write(FLASH_DATA,0x2, eb_voice_input_channel);	//写入初始数据
              FLASH_Write(FLASH_DATA,0x3, eb_voice_output_channel);
              FLASH_Write(FLASH_DATA,0x4, eb_voice_level);
              FLASH_Lock();						//锁
          }
      }
    }
}






















