/*
 * @Author: Lkw 1332861164@qq.com
 * @Date: 2023-06-09 23:35:03
 * @LastEditors: Lkw 1332861164@qq.com
 * @LastEditTime: 2023-06-18 22:50:46
 * @FilePath: \CMS8S6990_6_10\code\bsp_ir.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/*********************************************************************************************************
 *
 *   模块名称 : IR驱动模块
 *   说    明 : 100US 定时器中调用。
 *********************************************************************************************************/
#include "all.h"

uint16_t gb_ir_judge_time = 0;
uint8_t irdata_num      = 0;
uint8_t self_lock_flag  = 0;                  //红外接收标志位 
uint8_t ir_rec_busy     = 0;
uint16_t ir_data_rec_table[10] = {0}; //脉冲时长数组

// sbit IR_input = P0^4;
// sbit test_input = P1^7;
uint16_t bsp_ir_data_raw(void);

/********************* extern port *********************/ 
void IR_Init()
{
  GPIO_SET_MUX_MODE(P04CFG, GPIO_MUX_GPIO);
  GPIO_ENABLE_INPUT(P0TRIS, GPIO_PIN_4);
  // P04CFG = 0x00;
  // P0TRIS &= 0xEF;
  // P04EICFG = 0x02;
  // P0EXTIE = 0x00;
    PS_INT0 = 0x04;
  // P0EXTIE |= 0x10;
    EXTINT_ConfigInt(EXTINT0, EXTINT_TRIG_FALLING);
    EXTINT_EnableInt(EXTINT0);
  // P0EXTIE |= 0x10;
  // GPIO_ENABLE_OUTPUT(P0TRIS, GPIO_PIN_4);
  // GPIO_ENABLE_OD(P0TRIS, GPIO_PIN_4);
}

void IR_RX(void)
{
    if (gb_ir_judge_time < 1800)
    {
        gb_ir_judge_time++;
    }
    else if (self_lock_flag == 0)
    {
        self_lock_flag = 1;
        irdata_num = 0;
    }
}

void bsp_ir_rec_handle(void)
{
    if (self_lock_flag == 1)
    {
      if (gb_ir_judge_time < 1800)
      {
          ir_rec_busy = 1;
          ir_data_rec_table[irdata_num] = gb_ir_judge_time;
          irdata_num++;

          if(irdata_num == 10)
          {
            ir_rec_busy = 0;
            irdata_num=0;
            self_lock_flag = 0;
            bsp_ir_event_pro(bsp_ir_data_raw());
          }
      }
    }
    gb_ir_judge_time=0;
}

uint16_t bsp_ir_data_raw(void)
{
    uint8_t  loop;
    uint16_t ir_data = 0;

    if (ir_rec_busy == 1)
    {
        return 0;
    }

    for(loop=1; loop<9; loop++)
    {
        if (ir_data_rec_table[loop] <= 13)
        {
            ir_data<<=2;
            ir_data+=0x00;                    
        }
        else if (ir_data_rec_table[loop] <= 16)
        {
            ir_data<<=2;
            ir_data+=0x01;                    
        }
        else if (ir_data_rec_table[loop] <= 19)
        {
            ir_data<<=2;
            ir_data+=0x02;                    
        }
        else
        {
            ir_data<<=2;
            ir_data+=0x03;                    
        }
    }
    return ir_data;
}
#if (DEBUG_IR_FUNCTION == THIS_FUNCTION_ENABLE)

#else

///////////xiaomi TV ir remote control explain
/* 
xiaomi            customer
power:  15564     (mute)
up:     34309     (input channel switch)
down:   34310     (input channel switch)
left:   34315     (input channel switch)
right:  34316     (input channel switch)
enter:  34317     (mute)
home:   34312     (output channel switch)
back:   34311     (input channel switch)
menu:   34308     (output channel switch)
plus:   34318     (volume plus)
minus:  34319     (volume minus)

 */

#define IR_KEY_POWER  15564
#define IR_KEY_UP     34309
#define IR_KEY_DOWN   34310
#define IR_KEY_LEFT   34315
#define IR_KEY_RIGHT  34316
#define IR_KEY_ENTER  34317
#define IR_KEY_HOME   34312
#define IR_KEY_BACK   34311
#define IR_KEY_MENU   34308
#define IR_KEY_PLUS   34318
#define IR_KEY_MINUS  34319

void bsp_ir_event_pro(uint16_t ir_key_num)
{
    switch (ir_key_num)
    {
    // mute
    case IR_KEY_POWER:
    case IR_KEY_ENTER:
        BitDisplayData_chang = 1;
        //控制禁用音频
        BitVoiceMute = ~BitVoiceMute;
      break;

    // input channel switch
    case IR_KEY_UP   :
    case IR_KEY_DOWN :
    case IR_KEY_LEFT :
    case IR_KEY_RIGHT:
    case IR_KEY_BACK :
        BitDisplayData_chang = 1;		//显示更新标志
        //
        BitDataCharg = 1;						//数据保存更新标志
        if(eb_voice_input_channel >= 2)
        {
            eb_voice_input_channel = 1;
        }
        else
        {
            eb_voice_input_channel ++;
        }
      break;

    // output channel switch
    case IR_KEY_HOME:
        eb_voice_output_channel = eb_voice_output_channel > 1 ? (eb_voice_output_channel-1) : 4;
        BitDisplayData_chang = 1;
        BitDataCharg = 1;
      break;

    case IR_KEY_MENU:
        eb_voice_output_channel = eb_voice_output_channel < 4 ? (eb_voice_output_channel+1) : 1;
        BitDisplayData_chang = 1;
        BitDataCharg = 1;
      break;

    // volume plus
    case IR_KEY_PLUS:
      if (BitVoiceMute == 0)
      {
        if(eb_voice_level < VOLUME_MAX_CLASS)
        {
            eb_voice_level++;
        }
    
        BitDisplayData_chang = 1;
        eb_button_change_motor_sta = 1;
        BitDataCharg = 1;
      }
      break;

    // volume minus
    case IR_KEY_MINUS:
      if (BitVoiceMute == 0)
      {
        if(eb_voice_level > VOLUME_MIN_CLASS)
        {
            eb_voice_level--;
        }

        BitDisplayData_chang = 1;			//数据有更改标志位置1
        eb_button_change_motor_sta = 1;
        BitDataCharg = 1;
      }
      break;
    
    default:
      break;
    }
}
#endif
