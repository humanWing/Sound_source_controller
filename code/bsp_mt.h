#ifndef _BSP_MT_H_
#define _BSP_MT_H_


void mt_init(void);
void MotorForward(void)	;
void MotorReverse(void)	;
//
void mt_ctrl();
//
extern bit BitUpData_chang;
extern bit BitData_Astrict_F;		//正转限制
extern bit BitData_Astrict_R;		//反转限制
#endif

