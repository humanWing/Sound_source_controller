/*
 * @Author: Lkw 1332861164@qq.com
 * @Date: 2023-06-09 23:35:03
 * @LastEditors: Lkw 1332861164@qq.com
 * @LastEditTime: 2023-06-18 22:19:58
 * @FilePath: \CMS8S6990_6_10\code\bsp_ir.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef _BSP_IR_H_
#define _BSP_IR_H_


sbit 	P_IR_RX = P0^4;						//定义红外脉冲数据接口
//

#define ConIRRxDataCout     4
//


//引导码 9ms
#define ConIRRxOVER_9ms    				 95         //基值100us
#define ConIRRx_9ms          			 75         //基值100us
//
//同步码 4.5ms
#define ConIRRxOVER_4D5ms    				55
#define ConIRRx_4D5ms            		32
//
#define ConIRRXHIGH_OVER            20      //数据高电平最大时间,基值100us.  高电平理论最大:1.69ms  
#define ConIRRXLOW_OVER             10      //数据低电平最大时间,基值100us. 低电平理论最大:0.56ms
//
#define ConIR_RX_01                 10      //0/1的区分界限.  高电平时间  0:0.56ms  1:1.69ms
#define ConIR_RX_DUP                26      //重复码引导低电平只有2.25ms.  由此判断是重复码还是初始按键码.
//
extern xdata u8 BitVoiceLevelBack;

//外部调用函数声明
void IR_Init();		//初始化
void IR_RX(void);
void bsp_ir_event_pro(uint16_t ir_key_num);
#endif