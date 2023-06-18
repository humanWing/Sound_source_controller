/* *****************************************************
// �ļ���   : bsp_adc.c
// ������   : cms8s6990  ssop24
// ���뻷�� : Keil5 C51
// ϵͳʱ�� :
// ��    ��	: V1.0
// ��������	:
// �޸�����	:
// ������ :
***************************************************** */
#include "all.h"


//*************************************
// �������ƣ�adc_config
// �������ܣ�ADC���ó�ʼ��
// ��ڲ�������
// ���ڲ�������
//***************************************/
void adc_config()
{
  //����ADC������ģʽ
  ADC_ConfigRunMode(ADC_CLK_DIV_16, ADC_RESULT_LEFT);	//����ADCʱ��Ϊϵͳʱ�ӵ�16��Ƶ��ADC���Ϊ����룬���ж�ADCʱ���в�����ο�"��΢8051оƬADCģ��Ӧ�ñʼ�"
  //����ADCת��ͨ��--�˿�����
  ADC_EnableChannel(ADC_CH_0);
  GPIO_SET_MUX_MODE(P00CFG, GPIO_P00_MUX_AN0);
  //����ADC�ο���ѹ  VDD
  // ADC_EnableLDO();
  // ADC_ConfigADCVref(ADC_VREF_3V);		//ADC_VREF_1P2V, ADC_VREF_2V, ADC_VREF_2P4V, ADC_VREF_3V
  //LDO��ֹ���ο���ѹΪоƬ��Դ��ѹ��
  ADC_DisableLDO();
  //����ADC�ж�
  //ADC_EnableInt();
  //IRQ_SET_PRIORITY(IRQ_ADC,IRQ_PRIORITY_HIGH);
  //IRQ_ALL_ENABLE();

  //����ADC--1= ʹ��ADC ADC1EN
  ADC_Start();
}

//*************************************************************************
//                             AD����
//�������ֵ,��Сֵ, ����ֵ��ƽ��
//������: 8��,  ������Ϊ1,2,4,8.
//*************************************************************************
unsigned int ADVaule[8];
unsigned char AD_index;
unsigned char AD_start_flag = 0;
#define AD_sample_num 8
unsigned int GetADValue(unsigned char ADChannel,unsigned char ADFilterNum)
{
  unsigned int ADVaule1=0,Temp=0,ADValueMax=0x0000,ADValueMin=0xffff;
  unsigned char SampleNum=0;
  // int loop;

  //
  if(ADFilterNum > 8)
    ADFilterNum = 8;                										//��������������16��.
  //
  ADCON1 |= ADChannel;
  //
  ADCON0 |= 0X02;   																	//��ʼADCת��
  while(!(ADCON0&0X02));	         		 								//�ȴ� ADCת�����;  ��ѯ ADGO λ
  //																									//��һ�δβ�������.
  
  if (AD_start_flag == 0)
  {
    AD_start_flag = 1;
    for(SampleNum=0; SampleNum<AD_sample_num; SampleNum++)
    {
        ADCON0 |= 0X02;   																//��ʼADCת��
        while(!(ADCON0&0X02));	         		 							//�ȴ� ADCת�����;
        //
        Temp = ADC_GetADCResult();

        // if(Temp > ADValueMax)
        //   ADValueMax = Temp;														//�������ֵ
        // else if(Temp < ADValueMin)
        //   ADValueMin = Temp;														//������Сֵ.

        ADVaule[SampleNum] = Temp;
    }
  }

  for(SampleNum=0; SampleNum<3; SampleNum++)
    {
      ADCON0 |= 0X02;   																//��ʼADCת��
      while(!(ADCON0&0X02));	         		 							//�ȴ� ADCת�����;
      //
      Temp = ADC_GetADCResult();

      if(Temp > ADValueMax)
        ADValueMax = Temp;														//�������ֵ
      else if(Temp < ADValueMin)
        ADValueMin = Temp;														//������Сֵ.

      ADVaule1 += Temp;
    }

  ADVaule[AD_index] = ADVaule1 - ADValueMax - ADValueMin;
  AD_index++;

  if (AD_index >= AD_sample_num)
  {
    AD_index = 0;
  }

  ADVaule1 = 0;

  for(SampleNum=0; SampleNum<AD_sample_num; SampleNum++)
  {
    ADVaule1 += ADVaule[SampleNum];
  }
  ADVaule1 /= AD_sample_num;
  return ADVaule1;																		//����AD����ֵ
}