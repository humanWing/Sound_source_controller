/*
*********************************************************************************************************
*
*	ģ������ :
*	�ļ����� : bsp_display.c
*	��    �� : V1.0
*	˵    �� : ���������ʾ
*
*	�޸ļ�¼ :
*		�汾��  ����         ����     ˵��
*
*********************************************************************************************************
*/

#include "all.h"

extern u16 xdata 		VarTimer100ms;
u8 bai_num,shi_num,ge_num;

#if 0
static u8 code LED_tab[14][2]=   // ���������0~9�������
{
  {0x3f, 0x00},    //0
  {0x06, 0x00},
  {0x5b, 0x08},
  {0x4f, 0x08},
  {0x66, 0x08},
  {0x6d, 0x08},
  {0x7d, 0x08},
  {0x07, 0x00},
  {0x7f, 0x08},
  {0x6f, 0x08},   //9
  {0x5e, 0x08},		//d 10
  {0x30, 0x00},		//l 11
  {0x6e, 0x08},		//y 12
  {0x40, 0x08}		//- 13
};

#else
static u8 code LED_tab[14]=   // ���������0~9�������
{
  0x3f,    //0
  0x06,
  0x5b,
  0x4f,
  0x66,
  0x6d,
  0x7d,
  0x07,
  0x7f,
  0x6f,   //9
  0x5e,		//d 10
  0x30,		//l 11
  0x6e,		//y 12
  0x40,	//- 13
};

static u8 code LED_tab1[14]=   // ���������0~9�������
{
  0x00,    //0
  0x00,
  0x08,
  0x08,
  0x08,
  0x08,
  0x08,
  0x00,
  0x08,
  0x08,   //9
  0x08,		//d 10
  0x00,		//l 11
  0x08,		//y 12
  0x08		//- 13
};
#endif

#define dissetmode 			0x02 //��ʾģʽ���� 6λ8����ʾ
#define writedatamode_z 0x40 //�Զ���һ��ʽ�Դ� д���ݵ���ʾ�Ĵ���
#define writeledmode_z 	0x41 //�Զ���һ��ʽдLED�Դ�����
#define readkeymode 		0x42 //����������
#define readswmode			0x43 //��SW����
#define writedatamode_g   0x44    //�̶�ģʽ
//
#define startaddress 		0xc0 //�Զ���1��ʾ����ʼ��ַ���ã�Ϊ00H
#define disconmode 			0x8a//0x8F //��ʾ���ƣ���ʾ��
#define discommode_off	0x87 //��ʾ���ƣ���ʾ�ر�
bit 	BitDisplayOn;								//��ʾ����
u8 TmData;
xdata u8 TM1620_temp[6] = {0};

xdata u16 VarAutoTimeCnt;

extern u8 xdata 	BitTime100us;
u8 VarDisplayUpdateCnt = 0;
//*************************************
// �������ƣ�delay_ms
// �������ܣ��ӳ�
// ��ڲ�����Ҫ����������
// ���ڲ�������
//***************************************/
void delay_ms(u8 cyc)
{
  u8 ii,jj;

  for(jj=1; jj<=cyc; jj++)
    {
      for(ii=0; ii<150; ii++)
        {
          _nop_();
        }
    }
}
void TimeDelay_us(u8 Time)
{
  u8 i;
  i = Time;
  //
  while(i)
    {
      if (BitTime100us)
        {

          BitTime100us = 0;
          i--;
        }
    }
}

//*************************************
// �������ƣ�TM1620_Write
// �������ܣ���TM1620������ 8λ����-1���ֽڵ�����
// ��ڲ�����Ҫ����������
// ���ڲ�������
// ������Ҫ�������ز���һλ �ӵ�λ��ʼ��������
//***************************************/
void TM1620_Write(u8 wr_data)
{
  u8 i;
  for(i=0; i<8; i++)				//��ʼ����8λ���ݣ�ÿѭ��һ�δ���һλ����
    {
      CLK = 0;
      if(wr_data&0x01)
        {
          DIO = 1;
        }
      else
        {
          DIO = 0;
        }
      //TimeDelay_us(10);						//��Ƶ�ٶ�24MHZ  �ٶ�̫���ˣ�Ҫ�ӳ���
      delay_ms(10);
      CLK = 1;
      //TimeDelay_us(10);
      delay_ms(10);
      wr_data>>=1;						//��λ����  ��λ��ǰ
    }
}
//*************************************
// �������ƣ�Write_COM
// �������ܣ���TM1620����������
// ��ڲ�����Ҫ����������
// ���ڲ�������
// ������Ҫ�������ز���һλ �ӵ�λ��ʼ��������
//***************************************/
void Write_COM(unsigned char cmd)//
{
  STB = 1;
  //TimeDelay_us(10);
  delay_ms(10);
  STB = 0;
  TM1620_Write(cmd);
}
//*************************************
// �������ƣ�init_TM1620
// �������ܣ���TM1620��ʼ��
// ��ڲ�������
// ���ڲ�������
//***************************************/
void init_TM1620(void)
{
  //u8 i;
  //
  GPIO_SET_MUX_MODE(P01CFG, GPIO_MUX_GPIO);
  GPIO_ENABLE_OUTPUT(P0TRIS, GPIO_PIN_1);				//���
  GPIO_ENABLE_OD(P0OD, GPIO_PIN_1);
  //
  GPIO_SET_MUX_MODE(P02CFG, GPIO_MUX_GPIO);
  GPIO_ENABLE_OUTPUT(P0TRIS, GPIO_PIN_2);
  GPIO_ENABLE_OD(P0OD, GPIO_PIN_2);							//���ÿ�©���ģʽ
  //
  GPIO_SET_MUX_MODE(P03CFG, GPIO_MUX_GPIO);
  GPIO_ENABLE_OUTPUT(P0TRIS, GPIO_PIN_3);
  GPIO_ENABLE_OD(P0OD, GPIO_PIN_3);
  //
  CLK = 1;					//��ʼ��ƽ��			---�ⲿ����������
  STB = 1;
  DIO = 1;
  //
  /* Write_COM(dissetmode);       //������ʾģʽ
   Write_COM(writedatamode_z);  //�Զ�ģʽ �Զ���1ģʽ
   Write_COM(startaddress);
   //
   for(i=0; i<6; i++)    	 			 			//�˴�i����Ϊ��ַ���൱�ڴ�6��λѡ
   {
       Write_COM(startaddress+i*2); 		//����ַ
       TM1620_Write(0x00);      //������
   }
   Write_COM(disconmode);       //��ʾ��������
   STB=1;											 // ���ݴ�����ϣ�STB������
  */
}


//*************************************
// �������ƣ�led_show
// �������ܣ�TM1620��ʾ����
// ��ڲ�������
// ���ڲ�������
//***************************************/
void TM1620_Dsplay(unsigned char ch,unsigned char dat)
{
  CLK = 1;					//��
  STB = 1;
  DIO = 1;
  //
  Write_COM(0x02);       			//��ʾģʽ���ã�����Ϊ6��GRID��8��SEG
  Write_COM(writedatamode_z);  //��ַ����ģʽ �̶���ַ
  //
  Write_COM(startaddress+(ch-1)*2);//����ʼ��ַ
  TM1620_Write(1<<(dat-1));			//������
  //
  Write_COM(disconmode);      	//��ʾ��������
  STB=1;												// ���ݴ�����ϣ�STB������
}


//*************************************
// �������ƣ�led_show
// �������ܣ�TM1620��ʾ����
// ��ڲ�������
// ���ڲ�������
//***************************************/
void led_show()
{
  u8 i;
  //
  u8 temp[4]; //�����ĸ���������Ҫ��ʾ���ĸ�����
  temp[0]=1; 	//�Ȱ�TmData�������������̣�����ڸ�ֵ��temp[0]����ȡ��TmData��ǧλ��
  temp[1]=2; 	//ͬ��
  temp[2]=3;
  temp[3]=4;
  //
  Write_COM(dissetmode);       			//������ʾģʽ
  Write_COM(writedatamode_z);  			//�Զ�ģʽ �Զ���1ģʽ
  //Write_COM(startaddress);
  //
  for(i=0; i<6; i++)    	 			 		//�˴�i����Ϊ��ַ���൱�ڴ�6��λѡ
    {
      Write_COM(startaddress+i*2); 	//����ַ
      TM1620_Write(LED_tab[i]);     //������
    }
  Write_COM(disconmode);      	//��ʾ��������
  STB=1;												// ���ݴ�����ϣ�STB������


}

void display_1620(void)
{
  u8 i;
  //
  Write_COM(dissetmode);       			//������ʾģʽ
  Write_COM(writedatamode_z);  			//�Զ�ģʽ �Զ���1ģʽ
  Write_COM(startaddress);
  //
  for(i=0; i<6; i++)    	 			 		//�˴�i����Ϊ��ַ���൱�ڴ�6��λѡ
    {
      // Write_COM(startaddress+i*2); 	//����ַ
      // TM1620_Write(LED_tab[TM1620_temp[i]][0]);//������
      // TM1620_Write(LED_tab[TM1620_temp[i]][1]);//������
      TM1620_Write(LED_tab[TM1620_temp[i]]);//������
      TM1620_Write(LED_tab1[TM1620_temp[i]]);//������
    }
  Write_COM(disconmode);      	//��ʾ��������
  STB=1;												//���ݴ�����ϣ�STB������
}
//
void display_off(void)
{
  u8 i;
  //
  Write_COM(dissetmode);       			//������ʾģʽ
  Write_COM(writedatamode_z);  			//�Զ�ģʽ �Զ���1ģʽ
  //Write_COM(startaddress);
  //
  for(i=0; i<6; i++)    	 			 		//�˴�i����Ϊ��ַ���൱�ڴ�6��λѡ
    {
      Write_COM(startaddress+i*2); 	//����ַ
      TM1620_Write(0x00);						//������
    }
  Write_COM(discommode_off);      			//��ʾ��������
  STB=1;														//���ݴ�����ϣ�STB������
}

/**********************************************************************************************************
*	�� �� ��:auto_countdown
*	����˵��: �ϵ���ʾ  DLY				003  ���ҵ���ʱ
*	��    ��: ��
*	�� �� ֵ: ��										1msʱ��   1S�л�һ�����ݣ�����0������һ��״̬
*********************************************************************************************************
*/
void auto_countdown(void)
{
  //
  //static u8 VarDisplayUpdateCnt = 0;
  //
  if(VarAutoSet >0)
    {
      VarAutoTimeCnt++;
      if(VarAutoTimeCnt >= 1000)	//1ms*1000 = 1s
        {
          VarAutoTimeCnt =0  ;
          VarAutoSet --;
          VarDisplayUpdateCnt = 0;	//ˢ������
        }      //
    }
  else
    {
      VarAutoSet = 0;
      VarAutoTimeCnt++;
      if(VarAutoTimeCnt >= 500)
        {
          VarAutoTimeCnt = 0;
          VarAutoSet = 0;
          //
          SysStatus = 1;						//��ʾ����ģʽ
          BitDisplayData_chang = 1;
          VarDisplayUpdateCnt = 0;
        }
    }
  TM1620_temp[0] = 10;			//
  TM1620_temp[1] = 11;			//
  TM1620_temp[2] = 12;			//
  //
  TM1620_temp[3] = 13;			//
  TM1620_temp[4] = 13;			//

  TM1620_temp[5] = VarAutoSet;//����ʱ 3 - 2- 1- 0
  //

  if(VarDisplayUpdateCnt == 0)
    {
      VarDisplayUpdateCnt = 1;
      //����һ������
      display_1620();						//ˢ������
      //
    }
  //
#if 0
  if(VarAutoSetCnt < 3)
    {
      TM1620_temp[0] = 10;			//
      TM1620_temp[1] = 11;			//
      TM1620_temp[2] = 12;			//
      //
      TM1620_temp[3] = 13;			//
      TM1620_temp[4] = 13;			//

      TM1620_temp[5] = VarAutoSet;//
      //
      if(VarDisplayUpdateCnt == 0)
        {
          VarDisplayUpdateCnt = 1;
          //����һ������
          display_1620();						//ˢ������
          //
        }
      else
        {
          VarAutoTimeCnt++;
          if(VarAutoTimeCnt >= 1000)	//1ms*1000 = 1s
            {
              VarAutoTimeCnt =0  ;
              VarAutoSet --;
              VarDisplayUpdateCnt = 0;	//ˢ������
              //
              if(++VarAutoSetCnt == 3)
                {
                  VarAutoSetCnt = 3;
                  VarAutoTimeCnt = 0;
                }
            }
        }
    }
  else											//������ʾ״̬
    {
      VarAutoSet = 0;

      SysStatus = 1;		//��ʾ����ģʽ
      BitDisplayData_chang = 1;
      VarDisplayUpdateCnt = 0;
    }
#endif
}
/*
*********************************************************************************************************
*	�� �� ��:display_refurbish
*	����˵��: ��ʾ���ݸ���  �����ݸ��ĲŽ����⣬����ˢ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/

void display(void)
{
  if(!SysStatus)  
  {
    return ;
  }

  //���ݴ��뻺�棬����ˢ����ʾ
  if(BitDisplayOn)						//
    {
      #if (DEBUG_AD_SAMPLE_FUNCTION == THIS_FUNCTION_ENABLE)
      extern u16 VarADData_Vlaue;
      TM1620_temp[3] = VarADData_Vlaue / 1000;
      TM1620_temp[4] = VarADData_Vlaue % 1000 / 100;
      TM1620_temp[5] = VarADData_Vlaue % 100 / 10;
      TM1620_temp[0] = VarADData_Vlaue % 10;
      TM1620_temp[1] = 13;
      TM1620_temp[2] = 13;

      display_1620();
      
    #elif (DEBUG_IR_FUNCTION == THIS_FUNCTION_ENABLE)
      extern uint16_t bsp_ir_data_raw(void);
      uint16_t temp = bsp_ir_data_raw();

      TM1620_temp[3] = temp / 10000;
      TM1620_temp[4] = temp % 10000 / 1000;
      TM1620_temp[5] = temp % 1000 / 100;
      TM1620_temp[0] = temp % 100 / 10;
      TM1620_temp[1] = temp % 10;
      TM1620_temp[2] = 13;

      display_1620();

    #else
      //����ͨ��
      TM1620_temp[0] = eb_voice_input_channel;			//

      TM1620_temp[1] = 13;						// -
      //���ͨ��
      TM1620_temp[2] = eb_voice_output_channel;			//
      //
      //��Ƶ�ȼ�
      if(BitVoiceMute)							//��������
      {
          TM1620_temp[3] = 13;			//-			��ʾ��-��
          TM1620_temp[4] = 13;			//-
          TM1620_temp[5] = 13;			//-
      }
      else
      {
          TM1620_temp[3] = eb_voice_level/100;			//��λ
          TM1620_temp[4] = eb_voice_level%100/10;	//ʮλ
          TM1620_temp[5] = eb_voice_level%10;			//��λ
      }
      if(BitDisplayData_chang)			//�����и���ʱ��������ʾˢ��
      {
          BitDisplayData_chang = 0;
          display_1620();						//ˢ������
          //
      }
    #endif
    }
  else
    {
      TM1620_temp[0] = 13;			//-
      TM1620_temp[1] = 13;			//-
      TM1620_temp[2] = 13;			//-
      TM1620_temp[3] = 13;			//-
      TM1620_temp[4] = 13;			//-
      TM1620_temp[5] = 13;			//-
      //
      if(BitDisplayData_chang)			//�����и���ʱ��������ʾˢ��
        {
          BitDisplayData_chang = 0;
          display_1620();							//ˢ����ʾ����
        }
    }

}