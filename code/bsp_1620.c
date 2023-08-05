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

#define dissetmode 			  0x02  //显示模式设置 6位8段显示
#define writedatamode_z   0x40  //自动加一方式显存 写数据到显示寄存器
#define writeledmode_z 	  0x41  //自动加一方式写LED显存命令
#define readkeymode 		  0x42  //读按键命令
#define readswmode			  0x43  //读SW命令
#define writedatamode_g   0x44  //固定模式
//
#define startaddress 		  0xc0  //自动加1显示的起始地址设置，为00H
#define disconmode 			  0x8a  //0x8F //显示控制，显示开
#define discommode_off	  0x87  //显示控制，显示关闭

xdata u8 TM1620_temp[6] = {0};

//*************************************
// 函数名称：delay_ms
// 函数功能：延迟
// 入口参数：要操作的数据
// 出口参数：无
//***************************************/

// 100 us
uint8_t xdata ub_100us_delay_size = 0;
uint16_t xdata ub_100us_timeout_cnt = 0;

void delay_ms(void)
{
  ub_100us_delay_size = 4;

  while(ub_100us_delay_size != 0)
  {
      ub_100us_timeout_cnt++;

      if (ub_100us_timeout_cnt > 2000)
      {
          ub_100us_timeout_cnt = 0;
          return;
      }
  }

  ub_100us_timeout_cnt = 0;
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
      delay_ms();
      CLK = 1;
      delay_ms();
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
  delay_ms();
  STB = 0;
  TM1620_Write(cmd);
}
//*************************************
// 函数名称：init_TM1620
// 函数功能：向TM1620初始化
// 入口参数：无
// 出口参数：无
//***************************************/
void bsp_tm1620_init(void)
{
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
  CLK = 1;
  STB = 1;
  DIO = 1;
}

//*************************************
// 函数名称：led_show
// 函数功能：TM1620显示数据
// 入口参数：无
// 出口参数：无
//***************************************/
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
      // TM1620_Write(LED_tab[TM1620_temp[i]][0]);//传数据
      // TM1620_Write(LED_tab[TM1620_temp[i]][1]);//传数据
      TM1620_Write(LED_tab[TM1620_temp[i]]);//传数据
      TM1620_Write(LED_tab1[TM1620_temp[i]]);//传数据
    }
    Write_COM(disconmode);      	//显示控制命令
    STB=1;												//数据传输完毕，STB被拉高
}

/*
*********************************************************************************************************
*	函 数 名:display_refurbish
*	功能说明: 显示数据更新  有数据更改才进入这，数据刷新
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_1620_update_display(void)
{
  //数据存入缓存，进行刷新显示
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
    extern uint8_t g_ubir_data;

    uint8_t temp = g_ubir_data;

    TM1620_temp[3] = temp / 100;
    TM1620_temp[4] = temp % 100 / 10;
    TM1620_temp[5] = temp % 10;
    TM1620_temp[0] = 13;
    TM1620_temp[1] = 13;
    TM1620_temp[2] = 13;

    display_1620();

  #else
    if(BitDisplayData_chang)			//数据有更改时，进行显示刷新
    {


        //音频等级
        if(BitVoiceMute)							//禁用音量
        {
            TM1620_temp[0] = 13;			//-			显示“-”
            TM1620_temp[1] = 13;			//-
            TM1620_temp[2] = 13;			//-
            TM1620_temp[3] = 13;			//-			显示“-”
            TM1620_temp[4] = 13;			//-
            TM1620_temp[5] = 13;			//-
        }
        else
        {
            //输入通道
            TM1620_temp[0] = eb_voice_input_channel;			//

            TM1620_temp[1] = 13;						// -
            //输出通道
            TM1620_temp[2] = eb_voice_output_channel;			//
            TM1620_temp[3] = eb_voice_level/100;			//百位
            TM1620_temp[4] = eb_voice_level%100/10;	//十位
            TM1620_temp[5] = eb_voice_level%10;			//个位
        }

        BitDisplayData_chang = 0;

        display_1620();						//刷新数据
    }
  #endif
}