/*
 * @Author: Lkw 1332861164@qq.com
 * @Date: 2023-06-09 23:35:03
 * @LastEditors: Lkw 1332861164@qq.com
 * @LastEditTime: 2023-06-24 21:00:15
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

#define IR_LONG_PRESS_TIME  2000      // 300ms----->one time: 100us


#define IR_REC_INTERVAL_TIME1  60000      // 300ms----->one time: 1us

uint16_t xdata gb_ir_rec_interval_time = 0;
uint8_t irdata_num      = 0;
uint8_t gb_ir_start_flag  = 0;                  //红外接收标志位 
volatile uint16_t ir_data_rec_table[10] = {0}; //脉冲时长数组

uint16_t *bsp_ir_point = 0; //脉冲时长数组

// sbit IR_input = P0^4;
// sbit test_input = P1^7;
uint16_t bsp_ir_data_raw(void);

/********************* extern port *********************/ 
void IR_Init()
{
    bsp_ir_point = ir_data_rec_table;
    GPIO_SET_MUX_MODE(P04CFG, GPIO_MUX_GPIO);
    GPIO_ENABLE_INPUT(P0TRIS, GPIO_PIN_4);
    PS_INT0 = 0x04;
    EXTINT_ConfigInt(EXTINT0, EXTINT_TRIG_FALLING);
    // EXTINT_EnableInt(EXTINT0);
}

void bsp_ir_rec_time_interval_count(void)
{
    if (gb_ir_rec_interval_time < IR_LONG_PRESS_TIME)
    {
        gb_ir_rec_interval_time++;
    }
}


void bsp_ir_rec_handle(void)
{
    uint16_t rec_time1_count;

    if (gb_ir_rec_interval_time == IR_LONG_PRESS_TIME)
    {
        TMR_Stop(TMR1);
        rec_time1_count = TMR_GetCountValue(TMR1);

        if (irdata_num == 0)
        {
            if (rec_time1_count >= 28000)
            {
                TMR_ConfigTimerPeriod(TMR1, 0, 0);
                gb_ir_rec_interval_time = 0;
            }
            else if ((rec_time1_count > 19000)
                && (rec_time1_count < 28000))
            {
                gb_ir_start_flag = 1;
            }
        }

        if (gb_ir_start_flag == 1)
        {
            ir_data_rec_table[irdata_num] = rec_time1_count;
            irdata_num++;

            if(irdata_num == 10)
            {
                TMR_ConfigTimerPeriod(TMR1, 0, 0);
                irdata_num=0;
                gb_ir_start_flag = 0;
                gb_ir_rec_interval_time = 0;
                bsp_ir_event_pro(bsp_ir_data_raw());
                return;
            }
        }

        TMR_ConfigTimerPeriod(TMR1, 0, 0);
        TMR_Start(TMR1);
    }
}

uint16_t bsp_ir_data_raw(void)
{
    uint8_t  loop;
    uint16_t ir_data = 0;

    for(loop=2; loop<10; loop++)
    {
        if (ir_data_rec_table[loop] < 2600)
        {
            ir_data<<=2;
            ir_data+=0x00;                    
        }
        else if (ir_data_rec_table[loop] < 3200)
        {
            ir_data<<=2;
            ir_data+=0x01;                    
        }
        else if (ir_data_rec_table[loop] < 3800)
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
#if (DEBUG_IR_FUNCTION == THIS_FUNCTION_ENABLE)

#else  
        // mute
        case IR_KEY_POWER:
        // case IR_KEY_ENTER:
            bsp_voice_mute_toggle();
          break;

        case IR_KEY_UP   :
            bsp_voice_select_inchannel_plus();
            break;
        
        case IR_KEY_DOWN :
            bsp_voice_select_inchannel_down();
            break;


        case IR_KEY_LEFT :
            bsp_voice_select_outchannel_down();
            break;

        case IR_KEY_RIGHT:
            bsp_voice_select_outchannel_plus();
            break;

        case IR_KEY_BACK :
            bsp_voice_reset_customer_setting();
          break;

        case IR_KEY_HOME:
            bsp_voice_fast_minus();
          break;

        case IR_KEY_MENU:
            bsp_voice_fast_plus();
          break;

        // volume plus
        case IR_KEY_PLUS:
            bsp_voice_plus();
          break;

        // volume minus
        case IR_KEY_MINUS:
            bsp_voice_minus();
          break;
#endif     
        default:
          break;
    }
}
