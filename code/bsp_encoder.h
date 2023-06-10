#ifndef _BSP_ENCODER_H_
#define _BSP_ENCODER_H_

extern bit BitVoiceMute;
extern bit BitDisplayData_chang;
//外部调用函数声明
void encoder_init();
//
void encoder_a(void);
void encoder_b();
u8 EncoderReading_A(void);
u8 EncoderReading_B(void);

void Scan_encodeer_a(void);
void Scan_encodeer_b(void);

void encoder_a_ctrl();
void encoder_b_ctrl();
#endif