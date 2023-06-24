/*
 * @Author: Lkw 1332861164@qq.com
 * @Date: 2023-06-09 23:35:03
 * @LastEditors: Lkw 1332861164@qq.com
 * @LastEditTime: 2023-06-24 21:12:08
 * @FilePath: \CMS8S6990_6_10\code\all.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef _ALL_H_
#define _ALL_H_

#include "cms8s6990.h"
//
#include "bsp_ir.h"
#include "bsp_1620.h"
#include "bsp_encoder.h"
#include "bsp_adc.h"
#include "bsp_mt.h"
#include "bsp_out.h"
#include "bsp_eeprom.h"


#define VERSION_YEAR        2023
#define VERSION_MONTH       6
#define VERSION_DAY         18
#define SOFTWARE_VERSION    1.00.01
#define HARDWARE_VERSION    2.00.01


#define ture    1
#define false   0

#define THIS_FUNCTION_ENABLE    1
#define THIS_FUNCTION_DISABLE   0

typedef enum
{
    input_channel_1=1,
    input_channel_2,
    input_channel_max,
} voice_input_channel_e;

typedef enum
{
    output_channel_1=1,
    output_channel_2,
    output_channel_3,
    output_channel_4,
} voice_output_channel_e;



#define VOLUME_MAX_CLASS 254
#define VOLUME_MIN_CLASS 1


/* debug function switch */

// ad sample
#define DEBUG_AD_SAMPLE_FUNCTION    THIS_FUNCTION_DISABLE

//ir data receive debuf
#define DEBUG_IR_FUNCTION           THIS_FUNCTION_DISABLE


#if (DEBUG_AD_SAMPLE_FUNCTION && DEBUG_IR_FUNCTION)
#error "can't not all open"
#endif

//全局变量
extern u8 xdata eb_voice_input_channel;					//输入通道
extern u8 xdata eb_voice_output_channel;				//输出通道
extern u8 xdata eb_voice_level;		//音频等级
extern u8 xdata SysStatus;				//系统状态
extern u8 xdata VarAutoSet ;

extern u8 xdata BitDataCharg ;
//

extern void bsp_voice_select_inchannel_down(void);
extern void bsp_voice_select_inchannel_plus(void);
extern void bsp_voice_select_outchannel_down(void);
extern void bsp_voice_select_outchannel_plus(void);
extern void bsp_voice_plus(void);
extern void bsp_voice_fast_plus(void);
extern void bsp_voice_minus(void);
extern void bsp_voice_fast_minus(void);
extern void bsp_voice_reset_customer_setting(void);
extern void bsp_voice_mute_toggle(void);

#endif
