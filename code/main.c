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
u16 xdata 		VarTimer100ms;
u8 xdata 		VarTimer500ms ;
u8 xdata    VarTimer1s;
u8 xdata 		BitTimer10ms;
u8 xdata 		VarTimer10ms;
u16 xdata VarWirtFlashCnt;
//
u8 xdata VarINCnt;				//输入通道
u8 xdata VarOutCnt;				//输出通道
u8 xdata VarVoiceLevel;		//音频等级
u8 xdata BitVoice_En;			//使能标志

u8 xdata SysStatus;				//系统状态
u8 xdata VarAutoSet ;
//
u8 xdata BitDataCharg = 0;
/****************************************************************************/
/*	Local type definitions('typedef')
*****************************************************************************/

/****************************************************************************/
/*	Local variable  definitions('static')
*****************************************************************************/
uint32_t Systemclock = 24000000;

/****************************************************************************/
/*	Local function prototypes('static')
*****************************************************************************/
/**************************************************************
*函数名称：TimeMain
*函数功能：时间累计  控制.
**************************************************************/
void TimeMain(void)
{
  VarTimer100ms++;

  if (VarTimer100ms >= 10)
    {
      VarTimer100ms = 0;                                          //100ms定时.
      //
    }

  VarTimer500ms++;

  if (VarTimer500ms >= 50)                                         //500ms定时.
    {
      VarTimer500ms = 0;
    }
  //
  VarTimer1s ++;
  if (VarTimer1s >= 200)                                          //1s计时.
    {
      VarTimer1s = 0;
      //
    }

}
/**************************************************************
利用定时器-延时子程序.
***************************************************************/
void Delay10ms(u8 Time)
{
  u8 i;
  i = Time;

  while(i)
    {
      if (BitTimer10ms)
        {
          // WDTCON |= 0x10;
          BitTimer10ms = 0;
          i--;
        }
    }
}
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

int main(void)
{
//	u8 i,Vartemp;
  //
  TMR0_Config();				//定时器初始化
  //
  encoder_init();				//编码器初始化
  init_TM1620();				//显示
  adc_config();					//adc初始化
  IR_Init();						//IR初始化
  //
  mt_init();					  //马达驱动
  out_init();						//输入输出通道设置
  //
  //FLASH_Erase(FLASH_DATA,0);//擦除数据
  //FLASH_Erase(FLASH_DATA,1);
  //FLASH_Erase(FLASH_DATA,2);
  //FLASH_Erase(FLASH_DATA,3);
  //FLASH_Erase(FLASH_DATA,4);
  //First_ReadData();			//读取保存数据
  //
// WDT_Config();
  //
  EA =1;								//开启全局中断
  //led_show();				 	//显示测试
  SysStatus = 0;				//显示dly  --3 倒计时
  VarAutoSet = 3;				//
  BitDisplayOn = 1;			//
  BitVoiceMute = 0;			//音量静音标志
  //
  //VarVoiceLevel = 14;
  //save_VoiceLevel();
  //
  while(1)
    {
#if 1
      // WDT_ClearWDT();								  //看门狗喂狗
      //
      if(SysStatus)												//系统启动后才扫描编码器
        {
          encoder_a();										//旋转编码器
          encoder_b();
        }
      //
      if(BitTimer1ms)											//1ms--定时时基
        {
          BitTimer1ms = 0;
          //
          display();											//显示刷新
          //
          switch(SysStatus)	 							//系统状态
            {
            case 0:
              auto_countdown();						//倒计时
              break;
            case 1:
              if(++VarTimer10ms >= 10)		//10ms
                {
                  BitTimer10ms = 1;
                  VarTimer10ms = 0;
                  Scan_encodeer_a();			//独立按键
                  Scan_encodeer_b();
                  //
                  out_ctrl();							//输入输出控制
                  Mute_ctrl();						//静音控制--默认关闭
                  //
                  mt_ctrl();							//马达控制
                  //
                  VoiceLevelBack_Ctrl();	//控制返回初始值
                  //
                  IRReceiveCTRL();				//红外接收处理
                }
              if((++VarWirtFlashCnt >= 500 ) && BitDataCharg)		//10ms*1000  且数据有变化才进行更新
                {
                  VarWirtFlashCnt = 0;
                  BitDataCharg = 0;
                  //
                  FLASH_UnLock();						//解锁
                  FLASH_Erase(FLASH_DATA,0);//擦除对应所在这一页
                  //for(i = 0 ;i< 3 ;i++)		//连续256 bytes的写等待Flash执行完成
                  //{
                  //	FLASH_Write(FLASH_DATA,0x4, 0xFF); //写地址使用最后的地址(任意地址都可以，建议用使用较少的地址)
                  //}
                  FLASH_Write(FLASH_DATA,0x0, 0x55);			//写入标志
                  //Vartemp = FLASH_Read(FLASH_DATA,0x0);
                  FLASH_Write(FLASH_DATA,0x1, 0x2a);
                  //Vartemp = FLASH_Read(FLASH_DATA,0x1);
                  //
                  FLASH_Write(FLASH_DATA,0x2, VarINCnt);	//写入初始数据
                  //Vartemp = FLASH_Read(FLASH_DATA,0x2);
                  FLASH_Write(FLASH_DATA,0x3, VarOutCnt);
                  //Vartemp = FLASH_Read(FLASH_DATA,0x3);
                  FLASH_Write(FLASH_DATA,0x4, VarVoiceLevel);
                  //Vartemp = FLASH_Read(FLASH_DATA,0x4);
                  //
                  FLASH_Lock();						//锁
                }
              break;
            default:
              break;
            }
        }
#endif
    }
}






















