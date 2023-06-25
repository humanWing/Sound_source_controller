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
** ����	2023��01��01�� 21:59:53
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
ȫ�ֱ���
*****************************************************************************/
static u8  xdata 	VarTimer10ms;
static u16 xdata  VarWirtFlashCnt;


u8 xdata eb_voice_input_channel;				//����ͨ��
u8 xdata eb_voice_output_channel;				//���ͨ��
u8 xdata eb_voice_level;		            //��Ƶ�ȼ�

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
*�������ƣ�void TMR0_Config(void)
*�������ܣ���ʱ��TMR0��ʼ��
*��ڲ�����void
*���ڲ�����void
**************************************************/
void TMR0_Config(void)
{

  /*
  (1)����Timer������ģʽ
  */
  TMR_ConfigRunMode(TMR0, TMR_MODE_TIMING,TMR_TIM_AUTO_8BIT);
  /*
  (2)����Timer ����ʱ��
  */
  //	/*Fsys = 12Mhz��Ftimer = 1Mhz,Ttmr= 1us*/
  TMR_ConfigTimerClk(TMR0, TMR_CLK_DIV_12);						/*Fsys = 24Mhz��Ftimer = 2Mhz,Ttmr=0.5us*/
  /*
  (3)����Timer����
  */
  TMR_ConfigTimerPeriod(TMR0, 256-200, 256-200); 				// 200*0.5us = 100us,��������
  //TMR_ConfigTimerPeriod(TMR0, 256-100, 256-100); 					//100*1us = 100us
  /*
  (4)�����ж�
  */
  TMR_EnableOverflowInt(TMR0);

  /*
  (5)����Timer�ж����ȼ�
  */
  IRQ_SET_PRIORITY(IRQ_TMR0,IRQ_PRIORITY_LOW);
  //IRQ_ALL_ENABLE();

  /*
  (6)����Timer
  */
  TMR_Start(TMR0);
}

void TMR1_Config(void)
{

  /*
  (1)����Timer������ģʽ
  */
  TMR_ConfigRunMode(TMR1, TMR_MODE_TIMING, TMR_TIM_16BIT);
  /*
  (2)����Timer ����ʱ��
  */
  //	/*Fsys = 12Mhz��Ftimer = 1Mhz,Ttmr= 1us*/
  TMR_ConfigTimerClk(TMR1, TMR_CLK_DIV_12);						/*Fsys = 24Mhz��Ftimer = 2Mhz,Ttmr=0.5us*/
  /*
  (3)����Timer����
  */
  TMR_ConfigTimerPeriod(TMR1, 0, 0); 				// 200*0.5us = 100us,��������
  //TMR_ConfigTimerPeriod(TMR0, 256-100, 256-100); 					//100*1us = 100us
  /*
  (4)�����ж�
  */
  // TMR_EnableOverflowInt(TMR1);

  /*
  (5)����Timer�ж����ȼ�
  */
  // IRQ_SET_PRIORITY(IRQ_TMR1, IRQ_PRIORITY_HIGH);
  //IRQ_ALL_ENABLE();

  /*
  (6)����Timer
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
  (1)ι��������WDTʱ��̫�̣�����WDT��λ
  */
  WDT_ClearWDT();
  /*
  (2)����WDT���ʱ��
  */
  WDT_ConfigOverflowTime(WDT_CLK_4194304);	// Fsys=24Mhz -> Twdt = 4194304/24 = 174.76ms
  /*
  (3)����WDT����ж�
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

    BitVoiceMute = 0;			//����������־

}

int main(void)
{
  EA = 0;
  bsp_tm1620_init();		//��ʾ
  encoder_init();				//��������ʼ��
  IR_Init();						//IR��ʼ��
  mt_init();					  //�������
  out_init();						//�������ͨ������
  First_ReadData();			//��ȡ��������
  adc_config();					//adc��ʼ��
  TMR0_Config();				//��ʱ����ʼ��
  TMR1_Config();
// WDT_Config();

  reset_parameter();
  
  EA = 1;								//����ȫ���ж�

  while(1)
  {
      // WDT_ClearWDT();								  //���Ź�ι��
      encoder_a();										//��ת������
      encoder_b();

      if(BitTimer1ms)											//1ms--��ʱʱ��
      {
          BitTimer1ms = 0;
          
          bsp_1620_update_display();											//��ʾˢ��

          if(++VarTimer10ms >= 10)		//10ms
          {
              VarTimer10ms = 0;

              Scan_encodeer_a();			//��������
              Scan_encodeer_b();
              out_ctrl();							//�����������
              Mute_ctrl();						//��������--Ĭ�Ϲر�
              mt_ctrl();							//������
          }

          if ((eBit_DataCharg == 1)
            && (++VarWirtFlashCnt >= 500))
          {
              VarWirtFlashCnt = 0;
              eBit_DataCharg = 0;
              //
              FLASH_UnLock();						//����
              FLASH_Erase(FLASH_DATA,0);//������Ӧ������һҳ
              FLASH_Write(FLASH_DATA,0x0, 0x55);			//д���־
              FLASH_Write(FLASH_DATA,0x1, 0x2a);
              FLASH_Write(FLASH_DATA,0x2, eb_voice_input_channel);	//д���ʼ����
              FLASH_Write(FLASH_DATA,0x3, eb_voice_output_channel);
              FLASH_Write(FLASH_DATA,0x4, eb_voice_level);
              FLASH_Lock();						//��
          }
      }
    }
}






















