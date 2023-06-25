#ifndef _BSP_1620_H_
#define _BSP_1620_H_

sbit STB =P0^1;        // 定义片选通讯口
sbit CLK =P0^2;        // 定义时钟通讯口
sbit DIO =P0^3;        // 定义数据输入/输出通讯口

extern void bsp_tm1620_init(void);

void bsp_1620_update_display(void);
#endif


