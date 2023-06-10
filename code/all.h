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
//全局变量
extern u8 xdata VarINCnt;					//输入通道
extern u8 xdata VarOutCnt;				//输出通道
extern u8 xdata VarVoiceLevel;		//音频等级
extern u8 xdata SysStatus;				//系统状态
extern u8 xdata VarAutoSet ;

extern u8 xdata BitDataCharg ;
//
#endif
