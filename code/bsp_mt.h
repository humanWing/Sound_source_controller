/*
 * @Author: Lkw 1332861164@qq.com
 * @Date: 2023-06-09 23:35:03
 * @LastEditors: Lkw 1332861164@qq.com
 * @LastEditTime: 2023-06-17 18:34:57
 * @FilePath: \CMS8S6990_6_10\code\bsp_mt.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef _BSP_MT_H_
#define _BSP_MT_H_


void mt_init(void);
void MotorForward(void)	;
void MotorReverse(void)	;
//
void mt_ctrl(void);
//
extern uint8_t eb_button_change_motor_sta;
//extern bit BitData_Astrict_F;		//正转限制
//extern bit BitData_Astrict_R;		//反转限制
#endif

