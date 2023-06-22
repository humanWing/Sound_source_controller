/*
*********************************************************************************************************
*
*	模块名称 :
*	文件名称 : bsp_display.c
*	版    本 : V1.0
*	说    明 : 控制输出显示
*
*	修改记录 :
*		版本号  日期         作者     说明
*
*********************************************************************************************************
*/

#include "all.h"

extern u16 xdata 		VarTimer100ms;
u8 bai_num,shi_num,ge_num;

#if 0
static u8 code LED_tab[14][2]=   // 定义数码管0~9数字码表
{
  {0x3f, 0x00},    //0
  {0x06, 0x00},
  {0x5b, 0x08},
  {0x4f, 0x08},
  {0x66, 0x08},
  {0x6d, 0x08},
  {0x7d, 0x08},
  {0x07, 0x00},
  {0x7f, 0x08},
  {0x6f, 0x08},   //9
  {0x5e, 0x08},		//d 10
  {0x30, 0x00},		//l 11
  {0x6e, 0x08},		//y 12
  {0x40, 0x08}		//- 13
};

#else
static u8 code LED_tab[14]=   // 定义数码管0~9数字码表
{
  0x3f,    //0
  0x06,
  0x5b,
  0x4f,
  0x66,
  0x6d,
  0x7d,
  0x07,
  0x7f,
  0x6f,   //9
  0x5e,		//d 10
  0x30,		//l 11
  0x6e,		//y 12
  0x40,	//- 13
};

static u8 code LED_tab1[14]=   // 定义数码管0~9数字码表
{
  0x00,    //0
  0x00,
  0x08,
  0x08,
  0x08,
  0x08,
  0x08,
  0x00,
  0x08,
  0x08,   //9
  0x08,		//d 10
  0x00,		//l 11
  0x08,		//y 12
  0x08		//- 13
};
#endif

#define dissetmode 			0x02 //显示模式设置 6位8段显示
#define writedatamode_z 0x40 //自动加一方式显存 写数据到显示寄存器
#define writeledmode_z 	0x41 //自动加一方式写LED显存命令
#define readkeymode 		0x42 //读按键命令
#define readswmode			0x43 //读SW命令
#define writedatamode_g   0x44    //固定模式
//
#define startaddress 		0xc0 //自动加1显示的起始地址设置，为00H
#define disconmode 			0x8a//0x8F //显示控制，显示开
#define discommode_off	0x87 //显示控制，显示关闭
bit 	BitDisplayOn;								//显示变量
u8 TmData;
xdata u8 TM1620_temp[6] = {0};

xdata u16 VarAutoTimeCnt;

extern u8 xdata 	BitTime100us;
u8 VarDisplayUpdateCnt = 0;
//*************************************
// 函数名称：delay_ms
// 函数功能：延迟
// 入口参数：要操作的数据
// 出口参数：无
//***************************************/
void delay_ms(u8 cyc)
{
  u8 ii,jj;

  for(jj=1; jj<=cyc; jj++)
    {
      for(ii=0; ii<150; ii++)
        {
          _nop_();
        }
    }
}
void TimeDelay_us(u8 Time)
{
  u8 i;
  i = Time;
  //
  while(i)
    {
      if (BitTime100us)
        {

          BitTime100us = 0;
          i--;
        }
    }
}

//*************************************
// 函数名称：TM1620_Write
// 函数功能：向TM1620发数据 8位数据-1个字节的数据
// 入口参数：要操作的数据
// 出口参数：无
// 技术简要：上升沿操作一位 从低位开始操作数据
//***************************************/
void TM1620_Write(u8 wr_data)
{
  u8 i;
  for(i=0; i<8; i++)				//开始传送8位数据，每循环一次传送一位数据
    {
      CLK = 0;
      if(wr_data&0x01)
        {
          DIO = 1;
        }
      else
        {
          DIO = 0;
        }
      //TimeDelay_us(10);						//主频速度24MHZ  速度太快了，要延迟下
      delay_ms(10);
      CLK = 1;
      //TimeDelay_us(10);
      delay_ms(10);
      wr_data>>=1;						//移位数据  低位在前
    }
}
//*************************************
// 函数名称：Write_COM
// 函数功能：向TM1620发送命令字
// 入口参数：要操作的数据
// 出口参数：无
// 技术简要：上升沿操作一位 从低位开始操作数据
//***************************************/
void Write_COM(unsigned char cmd)//
{
  STB = 1;
  //TimeDelay_us(10);
  delay_ms(10);
  STB = 0;
  TM1620_Write(cmd);
}
//*************************************
// 函数名称：init_TM1620
// 函数功能：向TM1620初始化
// 入口参数：无
// 出口参数：无
//***************************************/
void init_TM1620(void)
{
  //u8 i;
  //
  GPIO_SET_MUX_MODE(P01CFG, GPIO_MUX_GPIO);
  GPIO_ENABLE_OUTPUT(P0TRIS, GPIO_PIN_1);				//输出
  GPIO_ENABLE_OD(P0OD, GPIO_PIN_1);
  //
  GPIO_SET_MUX_MODE(P02CFG, GPIO_MUX_GPIO);
  GPIO_ENABLE_OUTPUT(P0TRIS, GPIO_PIN_2);
  GPIO_ENABLE_OD(P0OD, GPIO_PIN_2);							//设置开漏输出模式
  //
  GPIO_SET_MUX_MODE(P03CFG, GPIO_MUX_GPIO);
  GPIO_ENABLE_OUTPUT(P0TRIS, GPIO_PIN_3);
  GPIO_ENABLE_OD(P0OD, GPIO_PIN_3);
  //
  CLK = 1;					//初始电平高			---外部有上拉电阻
  STB = 1;
  DIO = 1;
  //
  /* Write_COM(dissetmode);       //设置显示模式
   Write_COM(writedatamode_z);  //自动模式 自动加1模式
   Write_COM(startaddress);
   //
   for(i=0; i<6; i++)    	 			 			//此处i变量为地址，相当于打开6个位选
   {
       Write_COM(startaddress+i*2); 		//传地址
       TM1620_Write(0x00);      //传数据
   }
   Write_COM(disconmode);       //显示控制命令
   STB=1;											 // 数据传输完毕，STB被拉高
  */
}


//*************************************
// 函数名称：led_show
// 函数功能：TM1620显示数据
// 入口参数：无
// 出口参数：无
//***************************************/
void TM1620_Dsplay(unsigned char ch,unsigned char dat)
{
  CLK = 1;					//高
  STB = 1;
  DIO = 1;
  //
  Write_COM(0x02);       			//显示模式设置，设置为6个GRID，8个SEG
  Write_COM(writedatamode_z);  //地址增加模式 固定地址
  //
  Write_COM(startaddress+(ch-1)*2);//传起始地址
  TM1620_Write(1<<(dat-1));			//传数据
  //
  Write_COM(disconmode);      	//显示控制命令
  STB=1;												// 数据传输完毕，STB被拉高
}


//*************************************
// 函数名称：led_show
// 函数功能：TM1620显示数据
// 入口参数：无
// 出口参数：无
//***************************************/
void led_show()
{
  u8 i;
  //
  u8 temp[4]; //定义四个变量，即要显示的四个数字
  temp[0]=1; 	//先把TmData求余数后再求商，最后在赋值给temp[0]，即取出TmData的千位数
  temp[1]=2; 	//同上
  temp[2]=3;
  temp[3]=4;
  //
  Write_COM(dissetmode);       			//设置显示模式
  Write_COM(writedatamode_z);  			//自动模式 自动加1模式
  //Write_COM(startaddress);
  //
  for(i=0; i<6; i++)    	 			 		//此处i变量为地址，相当于打开6个位选
    {
      Write_COM(startaddress+i*2); 	//传地址
      TM1620_Write(LED_tab[i]);     //传数据
    }
  Write_COM(disconmode);      	//显示控制命令
  STB=1;												// 数据传输完毕，STB被拉高


}

void display_1620(void)
{
  u8 i;
  //
  Write_COM(dissetmode);       			//设置显示模式
  Write_COM(writedatamode_z);  			//自动模式 自动加1模式
  Write_COM(startaddress);
  //
  for(i=0; i<6; i++)    	 			 		//此处i变量为地址，相当于打开6个位选
    {
      // Write_COM(startaddress+i*2); 	//传地址
      // TM1620_Write(LED_tab[TM1620_temp[i]][0]);//传数据
      // TM1620_Write(LED_tab[TM1620_temp[i]][1]);//传数据
      TM1620_Write(LED_tab[TM1620_temp[i]]);//传数据
      TM1620_Write(LED_tab1[TM1620_temp[i]]);//传数据
    }
  Write_COM(disconmode);      	//显示控制命令
  STB=1;												//数据传输完毕，STB被拉高
}
//
void display_off(void)
{
  u8 i;
  //
  Write_COM(dissetmode);       			//设置显示模式
  Write_COM(writedatamode_z);  			//自动模式 自动加1模式
  //Write_COM(startaddress);
  //
  for(i=0; i<6; i++)    	 			 		//此处i变量为地址，相当于打开6个位选
    {
      Write_COM(startaddress+i*2); 	//传地址
      TM1620_Write(0x00);						//传数据
    }
  Write_COM(discommode_off);      			//显示控制命令
  STB=1;														//数据传输完毕，STB被拉高
}

/**********************************************************************************************************
*	函 数 名:auto_countdown
*	功能说明: 上电显示  DLY				003  并且倒计时
*	形    参: 无
*	返 回 值: 无										1ms时基   1S切换一个数据，减到0进入下一个状态
*********************************************************************************************************
*/
void auto_countdown(void)
{
  //
  //static u8 VarDisplayUpdateCnt = 0;
  //
  if(VarAutoSet >0)
    {
      VarAutoTimeCnt++;
      if(VarAutoTimeCnt >= 1000)	//1ms*1000 = 1s
        {
          VarAutoTimeCnt =0  ;
          VarAutoSet --;
          VarDisplayUpdateCnt = 0;	//刷新数据
        }      //
    }
  else
    {
      VarAutoSet = 0;
      VarAutoTimeCnt++;
      if(VarAutoTimeCnt >= 500)
        {
          VarAutoTimeCnt = 0;
          VarAutoSet = 0;
          //
          SysStatus = 1;						//显示正常模式
          BitDisplayData_chang = 1;
          VarDisplayUpdateCnt = 0;
        }
    }
  TM1620_temp[0] = 10;			//
  TM1620_temp[1] = 11;			//
  TM1620_temp[2] = 12;			//
  //
  TM1620_temp[3] = 13;			//
  TM1620_temp[4] = 13;			//

  TM1620_temp[5] = VarAutoSet;//倒计时 3 - 2- 1- 0
  //

  if(VarDisplayUpdateCnt == 0)
    {
      VarDisplayUpdateCnt = 1;
      //更新一次数据
      display_1620();						//刷新数据
      //
    }
  //
#if 0
  if(VarAutoSetCnt < 3)
    {
      TM1620_temp[0] = 10;			//
      TM1620_temp[1] = 11;			//
      TM1620_temp[2] = 12;			//
      //
      TM1620_temp[3] = 13;			//
      TM1620_temp[4] = 13;			//

      TM1620_temp[5] = VarAutoSet;//
      //
      if(VarDisplayUpdateCnt == 0)
        {
          VarDisplayUpdateCnt = 1;
          //更新一次数据
          display_1620();						//刷新数据
          //
        }
      else
        {
          VarAutoTimeCnt++;
          if(VarAutoTimeCnt >= 1000)	//1ms*1000 = 1s
            {
              VarAutoTimeCnt =0  ;
              VarAutoSet --;
              VarDisplayUpdateCnt = 0;	//刷新数据
              //
              if(++VarAutoSetCnt == 3)
                {
                  VarAutoSetCnt = 3;
                  VarAutoTimeCnt = 0;
                }
            }
        }
    }
  else											//进入显示状态
    {
      VarAutoSet = 0;

      SysStatus = 1;		//显示正常模式
      BitDisplayData_chang = 1;
      VarDisplayUpdateCnt = 0;
    }
#endif
}
/*
*********************************************************************************************************
*	函 数 名:display_refurbish
*	功能说明: 显示数据更新  有数据更改才进入这，数据刷新
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/

void display(void)
{
  if(!SysStatus)  
  {
    return ;
  }

  //数据存入缓存，进行刷新显示
  if(BitDisplayOn)						//
    {
      #if (DEBUG_AD_SAMPLE_FUNCTION == THIS_FUNCTION_ENABLE)
      extern u16 VarADData_Vlaue;
      TM1620_temp[3] = VarADData_Vlaue / 1000;
      TM1620_temp[4] = VarADData_Vlaue % 1000 / 100;
      TM1620_temp[5] = VarADData_Vlaue % 100 / 10;
      TM1620_temp[0] = VarADData_Vlaue % 10;
      TM1620_temp[1] = 13;
      TM1620_temp[2] = 13;

      display_1620();
      
    #elif (DEBUG_IR_FUNCTION == THIS_FUNCTION_ENABLE)
      extern uint16_t bsp_ir_data_raw(void);
      uint16_t temp = bsp_ir_data_raw();

      TM1620_temp[3] = temp / 10000;
      TM1620_temp[4] = temp % 10000 / 1000;
      TM1620_temp[5] = temp % 1000 / 100;
      TM1620_temp[0] = temp % 100 / 10;
      TM1620_temp[1] = temp % 10;
      TM1620_temp[2] = 13;

      display_1620();

    #else
      //输入通道
      TM1620_temp[0] = eb_voice_input_channel;			//

      TM1620_temp[1] = 13;						// -
      //输出通道
      TM1620_temp[2] = eb_voice_output_channel;			//
      //
      //音频等级
      if(BitVoiceMute)							//禁用音量
      {
          TM1620_temp[3] = 13;			//-			显示“-”
          TM1620_temp[4] = 13;			//-
          TM1620_temp[5] = 13;			//-
      }
      else
      {
          TM1620_temp[3] = eb_voice_level/100;			//百位
          TM1620_temp[4] = eb_voice_level%100/10;	//十位
          TM1620_temp[5] = eb_voice_level%10;			//个位
      }
      if(BitDisplayData_chang)			//数据有更改时，进行显示刷新
      {
          BitDisplayData_chang = 0;
          display_1620();						//刷新数据
          //
      }
    #endif
    }
  else
    {
      TM1620_temp[0] = 13;			//-
      TM1620_temp[1] = 13;			//-
      TM1620_temp[2] = 13;			//-
      TM1620_temp[3] = 13;			//-
      TM1620_temp[4] = 13;			//-
      TM1620_temp[5] = 13;			//-
      //
      if(BitDisplayData_chang)			//数据有更改时，进行显示刷新
        {
          BitDisplayData_chang = 0;
          display_1620();							//刷新显示数据
        }
    }

}