#ifndef _BSP_1620_H_
#define _BSP_1620_H_

sbit STB =P0^1;        // ����ƬѡͨѶ��
sbit CLK =P0^2;        // ����ʱ��ͨѶ��
sbit DIO =P0^3;        // ������������/���ͨѶ��

extern void bsp_tm1620_init(void);

void bsp_1620_update_display(void);
#endif


