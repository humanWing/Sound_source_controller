/* *****************************************************
// 文件名   : bsp_adc.c
// 处理器   : cms8s6990  ssop24
// 编译环境 : Keil5 C51
// 系统时钟 :
// 版    本	: V1.0
// 生成日期	:
// 修改日期	:
// 简单描述 :
***************************************************** */
#include "all.h"


//*************************************
// 函数名称：adc_config
// 函数功能：ADC配置初始化
// 入口参数：无
// 出口参数：无
//***************************************/
void adc_config()
{
  //设置ADC的运行模式
  ADC_ConfigRunMode(ADC_CLK_DIV_16, ADC_RESULT_LEFT);	//设置ADC时钟为系统时钟的16分频，ADC结果为左对齐，如有对ADC时钟有操作请参考"中微8051芯片ADC模块应用笔记"
  //设置ADC转换通道--端口配置
  ADC_EnableChannel(ADC_CH_0);
  GPIO_SET_MUX_MODE(P00CFG, GPIO_P00_MUX_AN0);
  //设置ADC参考电压  VDD
  // ADC_EnableLDO();
  // ADC_ConfigADCVref(ADC_VREF_3V);		//ADC_VREF_1P2V, ADC_VREF_2V, ADC_VREF_2P4V, ADC_VREF_3V
  //LDO禁止，参考电压为芯片电源电压。
  ADC_DisableLDO();
  //设置ADC中断
  //ADC_EnableInt();
  //IRQ_SET_PRIORITY(IRQ_ADC,IRQ_PRIORITY_HIGH);
  //IRQ_ALL_ENABLE();

  //开启ADC--1= 使能ADC ADC1EN
  ADC_Start();
}

//*************************************************************************
//                             AD采样
//丢弃最大值,最小值, 其他值做平均
//最大次数: 8次,  建议设为1,2,4,8.
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
    ADFilterNum = 8;                										//最大采样次数限制16次.
  //
  ADCON1 |= ADChannel;
  //
  ADCON0 |= 0X02;   																	//开始ADC转换
  while(!(ADCON0&0X02));	         		 								//等待 ADC转换完成;  查询 ADGO 位
  //																									//第一次次采样丢弃.
  
  if (AD_start_flag == 0)
  {
    AD_start_flag = 1;
    for(SampleNum=0; SampleNum<AD_sample_num; SampleNum++)
    {
        ADCON0 |= 0X02;   																//开始ADC转换
        while(!(ADCON0&0X02));	         		 							//等待 ADC转换完成;
        //
        Temp = ADC_GetADCResult();

        // if(Temp > ADValueMax)
        //   ADValueMax = Temp;														//保存最大值
        // else if(Temp < ADValueMin)
        //   ADValueMin = Temp;														//保存最小值.

        ADVaule[SampleNum] = Temp;
    }
  }

  for(SampleNum=0; SampleNum<3; SampleNum++)
    {
      ADCON0 |= 0X02;   																//开始ADC转换
      while(!(ADCON0&0X02));	         		 							//等待 ADC转换完成;
      //
      Temp = ADC_GetADCResult();

      if(Temp > ADValueMax)
        ADValueMax = Temp;														//保存最大值
      else if(Temp < ADValueMin)
        ADValueMin = Temp;														//保存最小值.

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
  return ADVaule1;																		//返回AD采样值
}