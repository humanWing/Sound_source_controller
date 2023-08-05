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

#define dissetmode 			  0x02  //��ʾģʽ���� 6λ8����ʾ
#define writedatamode_z   0x40  //�Զ���һ��ʽ�Դ� д���ݵ���ʾ�Ĵ���
#define writeledmode_z 	  0x41  //�Զ���һ��ʽдLED�Դ�����
#define readkeymode 		  0x42  //����������
#define readswmode			  0x43  //��SW����
#define writedatamode_g   0x44  //�̶�ģʽ
//
#define startaddress 		  0xc0  //�Զ���1��ʾ����ʼ��ַ���ã�Ϊ00H
#define disconmode 			  0x8a  //0x8F //��ʾ���ƣ���ʾ��
#define discommode_off	  0x87  //��ʾ���ƣ���ʾ�ر�

xdata u8 TM1620_temp[6] = {0};

//*************************************
// �������ƣ�delay_ms
// �������ܣ��ӳ�
// ��ڲ�����Ҫ����������
// ���ڲ�������
//***************************************/

// 100 us
uint8_t xdata ub_100us_delay_size = 0;
uint16_t xdata ub_100us_timeout_cnt = 0;

void delay_ms(void)
{
  ub_100us_delay_size = 4;

  while(ub_100us_delay_size != 0)
  {
      ub_100us_timeout_cnt++;

      if (ub_100us_timeout_cnt > 2000)
      {
          ub_100us_timeout_cnt = 0;
          return;
      }
  }

  ub_100us_timeout_cnt = 0;
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
      delay_ms();
      CLK = 1;
      delay_ms();
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
  delay_ms();
  STB = 0;
  TM1620_Write(cmd);
}
//*************************************
// �������ƣ�init_TM1620
// �������ܣ���TM1620��ʼ��
// ��ڲ�������
// ���ڲ�������
//***************************************/
void bsp_tm1620_init(void)
{
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
  CLK = 1;
  STB = 1;
  DIO = 1;
}

//*************************************
// �������ƣ�led_show
// �������ܣ�TM1620��ʾ����
// ��ڲ�������
// ���ڲ�������
//***************************************/
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
      // TM1620_Write(LED_tab[TM1620_temp[i]][0]);//������
      // TM1620_Write(LED_tab[TM1620_temp[i]][1]);//������
      TM1620_Write(LED_tab[TM1620_temp[i]]);//������
      TM1620_Write(LED_tab1[TM1620_temp[i]]);//������
    }
    Write_COM(disconmode);      	//��ʾ��������
    STB=1;												//���ݴ�����ϣ�STB������
}

/*
*********************************************************************************************************
*	�� �� ��:display_refurbish
*	����˵��: ��ʾ���ݸ���  �����ݸ��ĲŽ����⣬����ˢ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_1620_update_display(void)
{
  //���ݴ��뻺�棬����ˢ����ʾ
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
    extern uint8_t g_ubir_data;

    uint8_t temp = g_ubir_data;

    TM1620_temp[3] = temp / 100;
    TM1620_temp[4] = temp % 100 / 10;
    TM1620_temp[5] = temp % 10;
    TM1620_temp[0] = 13;
    TM1620_temp[1] = 13;
    TM1620_temp[2] = 13;

    display_1620();

  #else
    if(BitDisplayData_chang)			//�����и���ʱ��������ʾˢ��
    {


        //��Ƶ�ȼ�
        if(BitVoiceMute)							//��������
        {
            TM1620_temp[0] = 13;			//-			��ʾ��-��
            TM1620_temp[1] = 13;			//-
            TM1620_temp[2] = 13;			//-
            TM1620_temp[3] = 13;			//-			��ʾ��-��
            TM1620_temp[4] = 13;			//-
            TM1620_temp[5] = 13;			//-
        }
        else
        {
            //����ͨ��
            TM1620_temp[0] = eb_voice_input_channel;			//

            TM1620_temp[1] = 13;						// -
            //���ͨ��
            TM1620_temp[2] = eb_voice_output_channel;			//
            TM1620_temp[3] = eb_voice_level/100;			//��λ
            TM1620_temp[4] = eb_voice_level%100/10;	//ʮλ
            TM1620_temp[5] = eb_voice_level%10;			//��λ
        }

        BitDisplayData_chang = 0;

        display_1620();						//ˢ������
    }
  #endif
}