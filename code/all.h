/*
 * @Author: Lkw 1332861164@qq.com
 * @Date: 2023-06-09 23:35:03
 * @LastEditors: Lkw 1332861164@qq.com
 * @LastEditTime: 2023-06-18 13:45:53
 * @FilePath: \CMS8S6990_6_10\code\all.h
 * @Description: ����Ĭ������,������`customMade`, ��koroFileHeader�鿴���� ��������: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
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
    input_channel_1,
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
#define DEBUG_AD_SAMPLE_FUNCTION    THIS_FUNCTION_DISABLE


//ȫ�ֱ���
extern u8 xdata eb_voice_input_channel;					//����ͨ��
extern u8 xdata eb_voice_output_channel;				//���ͨ��
extern u8 xdata eb_voice_level;		//��Ƶ�ȼ�
extern u8 xdata SysStatus;				//ϵͳ״̬
extern u8 xdata VarAutoSet ;

extern u8 xdata BitDataCharg ;
//
#endif