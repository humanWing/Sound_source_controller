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
u16 xdata 		VarTimer100ms;
u8 xdata 		VarTimer500ms ;
u8 xdata    VarTimer1s;
u8 xdata 		BitTimer10ms;
u8 xdata 		VarTimer10ms;
u16 xdata VarWirtFlashCnt;
//
u8 xdata VarINCnt;				//����ͨ��
u8 xdata VarOutCnt;				//���ͨ��
u8 xdata VarVoiceLevel;		//��Ƶ�ȼ�
u8 xdata BitVoice_En;			//ʹ�ܱ�־

u8 xdata SysStatus;				//ϵͳ״̬
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
*�������ƣ�TimeMain
*�������ܣ�ʱ���ۼ�  ����.
**************************************************************/
void TimeMain(void)
{
  VarTimer100ms++;

  if (VarTimer100ms >= 10)
    {
      VarTimer100ms = 0;                                          //100ms��ʱ.
      //
    }

  VarTimer500ms++;

  if (VarTimer500ms >= 50)                                         //500ms��ʱ.
    {
      VarTimer500ms = 0;
    }
  //
  VarTimer1s ++;
  if (VarTimer1s >= 200)                                          //1s��ʱ.
    {
      VarTimer1s = 0;
      //
    }

}
/**************************************************************
���ö�ʱ��-��ʱ�ӳ���.
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

int main(void)
{
//	u8 i,Vartemp;
  //
  TMR0_Config();				//��ʱ����ʼ��
  //
  encoder_init();				//��������ʼ��
  init_TM1620();				//��ʾ
  adc_config();					//adc��ʼ��
  IR_Init();						//IR��ʼ��
  //
  mt_init();					  //�������
  out_init();						//�������ͨ������
  //
  //FLASH_Erase(FLASH_DATA,0);//��������
  //FLASH_Erase(FLASH_DATA,1);
  //FLASH_Erase(FLASH_DATA,2);
  //FLASH_Erase(FLASH_DATA,3);
  //FLASH_Erase(FLASH_DATA,4);
  //First_ReadData();			//��ȡ��������
  //
// WDT_Config();
  //
  EA =1;								//����ȫ���ж�
  //led_show();				 	//��ʾ����
  SysStatus = 0;				//��ʾdly  --3 ����ʱ
  VarAutoSet = 3;				//
  BitDisplayOn = 1;			//
  BitVoiceMute = 0;			//����������־
  //
  //VarVoiceLevel = 14;
  //save_VoiceLevel();
  //
  while(1)
    {
#if 1
      // WDT_ClearWDT();								  //���Ź�ι��
      //
      if(SysStatus)												//ϵͳ�������ɨ�������
        {
          encoder_a();										//��ת������
          encoder_b();
        }
      //
      if(BitTimer1ms)											//1ms--��ʱʱ��
        {
          BitTimer1ms = 0;
          //
          display();											//��ʾˢ��
          //
          switch(SysStatus)	 							//ϵͳ״̬
            {
            case 0:
              auto_countdown();						//����ʱ
              break;
            case 1:
              if(++VarTimer10ms >= 10)		//10ms
                {
                  BitTimer10ms = 1;
                  VarTimer10ms = 0;
                  Scan_encodeer_a();			//��������
                  Scan_encodeer_b();
                  //
                  out_ctrl();							//�����������
                  Mute_ctrl();						//��������--Ĭ�Ϲر�
                  //
                  mt_ctrl();							//������
                  //
                  VoiceLevelBack_Ctrl();	//���Ʒ��س�ʼֵ
                  //
                  IRReceiveCTRL();				//������մ���
                }
              if((++VarWirtFlashCnt >= 500 ) && BitDataCharg)		//10ms*1000  �������б仯�Ž��и���
                {
                  VarWirtFlashCnt = 0;
                  BitDataCharg = 0;
                  //
                  FLASH_UnLock();						//����
                  FLASH_Erase(FLASH_DATA,0);//������Ӧ������һҳ
                  //for(i = 0 ;i< 3 ;i++)		//����256 bytes��д�ȴ�Flashִ�����
                  //{
                  //	FLASH_Write(FLASH_DATA,0x4, 0xFF); //д��ַʹ�����ĵ�ַ(�����ַ�����ԣ�������ʹ�ý��ٵĵ�ַ)
                  //}
                  FLASH_Write(FLASH_DATA,0x0, 0x55);			//д���־
                  //Vartemp = FLASH_Read(FLASH_DATA,0x0);
                  FLASH_Write(FLASH_DATA,0x1, 0x2a);
                  //Vartemp = FLASH_Read(FLASH_DATA,0x1);
                  //
                  FLASH_Write(FLASH_DATA,0x2, VarINCnt);	//д���ʼ����
                  //Vartemp = FLASH_Read(FLASH_DATA,0x2);
                  FLASH_Write(FLASH_DATA,0x3, VarOutCnt);
                  //Vartemp = FLASH_Read(FLASH_DATA,0x3);
                  FLASH_Write(FLASH_DATA,0x4, VarVoiceLevel);
                  //Vartemp = FLASH_Read(FLASH_DATA,0x4);
                  //
                  FLASH_Lock();						//��
                }
              break;
            default:
              break;
            }
        }
#endif
    }
}






















