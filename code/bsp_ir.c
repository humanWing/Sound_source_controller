/*
 * @Author: Lkw 1332861164@qq.com
 * @Date: 2023-06-09 23:35:03
 * @LastEditors: Lkw 1332861164@qq.com
 * @LastEditTime: 2023-08-06 22:08:39
 * @FilePath: \CMS8S6990_6_10\code\bsp_ir.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/*********************************************************************************************************
 *
 *   模块名称 : IR驱动模块
 *   说    明 : 100US 定时器中调用。
 *********************************************************************************************************/
#include "all.h"

#define IR_COUNT_INTERRUPT_TIME_BASE 100 //us

#define IR_LONG_PRESS_TIME  3000      // 300ms----->one time: 100us


#define IR_REC_INTERVAL_TIME1  60000      // 300ms----->one time: 1us

uint16_t xdata gb_ir_rec_interval_time = 0;
uint8_t irdata_num          = 0;
uint8_t irdata_long_num     = 0;
uint8_t gb_ir_start_flag    = 0;                  //红外接收标志位 
uint8_t gb_ir_stop_flag     = 0;                  //红外接收标志位

uint8_t g_ubir_data = 0;


#define NEC_DATA_SIZE           32
#define NEC_ADDR_DATA_SIZE      8
#define NEC_ADDR_AGAINST_SIZE   8
#define NEC_CTR_CMD_SIZE        8
#define NEC_CTR_AGAINST_SIZE    8

//NEC_DATA_SIZE = NEC_ADDR_DATA_SIZE + NEC_ADDR_DATA_SIZE + NEC_CTR_CMD_SIZE + NEC_CTR_AGAINST_SIZE (Byte)
volatile uint16_t ir_data_rec_table[8] = {0}; //脉冲时长数组


// sbit IR_input = P0^4;
// sbit test_input = P1^7;
void bsp_ir_data_raw(void);

/********************* extern port *********************/ 
void IR_Init()
{
    GPIO_SET_MUX_MODE(P04CFG, GPIO_MUX_GPIO);
    GPIO_ENABLE_INPUT(P0TRIS, GPIO_PIN_4);
    PS_INT0 = 0x04;
    EXTINT_ConfigInt(EXTINT0, EXTINT_TRIG_FALLING);
    EXTINT_EnableInt(EXTINT0);
}

void bsp_ir_rec_time_interval_count(void)
{

    if (gb_ir_rec_interval_time == IR_LONG_PRESS_TIME - 1)
    {
        gb_ir_start_flag = 0;
        gb_ir_stop_flag = 0;
    }

    if (gb_ir_rec_interval_time < IR_LONG_PRESS_TIME)
    {
        gb_ir_rec_interval_time++;
    }
}


void bsp_ir_rec_handle(void)
{
    uint16_t rec_time1_count;

    TMR_Stop(TMR1);
    rec_time1_count = TMR_GetCountValue(TMR1);

    if (gb_ir_start_flag == 0)
    {
        if ((rec_time1_count > 25000)
        && (rec_time1_count < 29000))
        {
            irdata_num = 0;
            irdata_long_num = 0;
            gb_ir_start_flag = 1;

        }
        else if ((rec_time1_count > 21000)
        && (rec_time1_count <= 25000))
        {
            if (gb_ir_stop_flag == 1)
            {

                if (irdata_long_num > 1)
                {
                    bsp_ir_event_pro(1);
                }
                irdata_long_num++;
            }
        }
        else
        {

        }
    }
    else if (gb_ir_start_flag == 1)
    {
        if ((irdata_num >= 16)
        && (irdata_num < 24))
        {
            ir_data_rec_table[irdata_num-16] = rec_time1_count;
        }

        irdata_num++;

        if(irdata_num == 32)
        {
            TMR_ConfigTimerPeriod(TMR1, 0, 0);
            irdata_num=0;
            gb_ir_start_flag = 0;
            gb_ir_stop_flag = 1;
            gb_ir_rec_interval_time = 0;
            bsp_ir_data_raw();
            bsp_ir_event_pro(0);
            return;
        }
    }

    gb_ir_rec_interval_time = 0;
    TMR_ConfigTimerPeriod(TMR1, 0, 0);
    TMR_Start(TMR1);
}

void bsp_ir_data_raw(void)
{
    int8_t loop;
    g_ubir_data = 0;

    for(loop=NEC_CTR_CMD_SIZE-1; loop>=0; loop--)
    {
        if (ir_data_rec_table[loop] < 2800)
        {
            g_ubir_data<<=1;
        }
        else
        {
            g_ubir_data<<=1;
            g_ubir_data += 1;
        }

    }
}


///////////xiaomi TV ir remote control explain
/* 
xiaomi            customer
first version
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

second version
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

#define IR_KEY_POWER        0xf0
#define IR_KEY_CH_PLUS      0xf1
#define IR_KEY_CH_MINUS     0xf2
#define IR_KEY_VOL_PLUS     0xf3
#define IR_KEY_VOL_MINUS    0xf4
#define IR_KEY_MUTE         0xf5


void bsp_ir_event_pro(uint8_t long_process)
{

    if ((long_process == 1)
    && (g_ubir_data != IR_KEY_VOL_PLUS) 
    && (g_ubir_data != IR_KEY_VOL_MINUS))
    {
        return;
    }


    switch (g_ubir_data)
    {
#if (DEBUG_IR_FUNCTION == THIS_FUNCTION_ENABLE)

#else  
        // mute
        case IR_KEY_POWER:
        // case IR_KEY_ENTER:
            bsp_voice_reset_customer_setting();
          break;

        case IR_KEY_CH_PLUS   :
            bsp_voice_select_inchannel_plus();
            break;
        
        case IR_KEY_CH_MINUS :
            bsp_voice_select_outchannel_plus();
            break;

        case IR_KEY_VOL_PLUS :
            bsp_voice_plus();
            break;

        case IR_KEY_VOL_MINUS:
            bsp_voice_minus();
            break;

        case IR_KEY_MUTE :
            bsp_voice_mute_toggle();
          break;
#endif     
        default:
          break;
    }
}
