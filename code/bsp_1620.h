#ifndef _BSP_1620_H_
#define _BSP_1620_H_

sbit STB =P0^1;        // ����ƬѡͨѶ��
sbit CLK =P0^2;        // ����ʱ��ͨѶ��
sbit DIO =P0^3;        // ������������/���ͨѶ��

extern u8 TmData;
extern u8 bai_num,shi_num,ge_num;
extern bit 	BitDisplayOn;
extern xdata u16 VarAutoTimeCnt;
extern u8 VarDisplayUpdateCnt ;
//
extern void init_TM1620(void);
extern void led_show();
extern void delay_us(u8 cyc);
extern void TM1620_Write(u8 wr_data);

void display_1620(void);
void auto_countdown(void);
void display(void);
//
#endif


