/*
 * @Author: Lkw 1332861164@qq.com
 * @Date: 2023-06-09 23:35:03
 * @LastEditors: Lkw 1332861164@qq.com
 * @LastEditTime: 2023-06-18 22:19:58
 * @FilePath: \CMS8S6990_6_10\code\bsp_ir.h
 * @Description: ����Ĭ������,������`customMade`, ��koroFileHeader�鿴���� ��������: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef _BSP_IR_H_
#define _BSP_IR_H_


sbit 	P_IR_RX = P0^4;						//��������������ݽӿ�
//

#define ConIRRxDataCout     4
//


//������ 9ms
#define ConIRRxOVER_9ms    				 95         //��ֵ100us
#define ConIRRx_9ms          			 75         //��ֵ100us
//
//ͬ���� 4.5ms
#define ConIRRxOVER_4D5ms    				55
#define ConIRRx_4D5ms            		32
//
#define ConIRRXHIGH_OVER            20      //���ݸߵ�ƽ���ʱ��,��ֵ100us.  �ߵ�ƽ�������:1.69ms  
#define ConIRRXLOW_OVER             10      //���ݵ͵�ƽ���ʱ��,��ֵ100us. �͵�ƽ�������:0.56ms
//
#define ConIR_RX_01                 10      //0/1�����ֽ���.  �ߵ�ƽʱ��  0:0.56ms  1:1.69ms
#define ConIR_RX_DUP                26      //�ظ��������͵�ƽֻ��2.25ms.  �ɴ��ж����ظ��뻹�ǳ�ʼ������.
//
extern xdata u8 BitVoiceLevelBack;

//�ⲿ���ú�������
void IR_Init();		//��ʼ��
void IR_RX(void);
void bsp_ir_event_pro(uint16_t ir_key_num);
#endif