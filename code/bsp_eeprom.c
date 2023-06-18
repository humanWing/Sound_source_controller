/*
*********************************************************************************************************
*
*	ģ������ :
*	�ļ����� : bsp_eeprom.c
*	��    �� : V1.0
*	˵    �� : ���ڱ����û����ã����籣��
*
*	�޸ļ�¼ :
*		�汾��  ����         ����     ˵��
*
*********************************************************************************************************
*/
#include "all.h"
//
//demo ���Դ���
#if 0
void data_write()
{
  u8 i;
  FLASH_UnLock();						//����
  addr= 0;
  Dtemp =0;
  FLASH_Erase(FLASH_DATA,addr);
  for(i = 0 ; i< 256 ; i++)		//����256 bytes��д�ȴ�Flashִ�����
    {
      FLASH_Write(FLASH_DATA,0x3ff, 0xFF); //д��ַʹ�����ĵ�ַ(�����ַ�����ԣ�������ʹ�ý��ٵĵ�ַ)
    }
  for(addr=0; addr<0x10; addr++)
    {
      FLASH_Write(FLASH_DATA,addr, Dtemp++);
      temp = FLASH_Read(FLASH_DATA,addr);
    }
  FLASH_Lock();						//��
}
#endif

/**********************************************************
*������:First_ReadData
*����:�ϵ��ȡ���ݣ��ж��Ƿ��һ���ϵ磬�ǵ�һ�����ݸ���ʼֵ��
*�������:��
*�������:��
**********************************************************/
static void bsp_eer_delay(int time)
{
  int loop;
  for(loop=0; loop<time; loop++)
  {
    _nop_();		
    _nop_();
  }

}

void First_ReadData()
{
  u8 temp1,temp2;						//��ʱ���ݱ���

  FLASH_UnLock();						//����
firstread:
  temp1 = FLASH_Read(FLASH_DATA,0);
  temp2 = FLASH_Read(FLASH_DATA,1);
  //
  if((temp1 == 0x55 ) && (temp2 == 0x2a))	//����0x55 0xaa˵����һ���ϵ硣
    {
      eb_voice_input_channel =FLASH_Read(FLASH_DATA,0x2);
      eb_voice_output_channel = FLASH_Read(FLASH_DATA,0x3);
      eb_voice_level = FLASH_Read(FLASH_DATA,0x4);
    }
  else
    {
      eb_voice_input_channel =1;
      eb_voice_output_channel = 1;
      eb_voice_level = VOLUME_MAX_CLASS / 2;
      //
      FLASH_Erase(FLASH_DATA,0);
      bsp_eer_delay(100);
      FLASH_Write(FLASH_DATA,0x0, 0x55);			//д���־
      FLASH_Write(FLASH_DATA,0x1, 0x2a);
      FLASH_Write(FLASH_DATA,0x2, eb_voice_input_channel);	//д���ʼ����
      FLASH_Write(FLASH_DATA,0x3, eb_voice_output_channel);
      FLASH_Write(FLASH_DATA,0x4, eb_voice_level);
      bsp_eer_delay(100);
      goto firstread;			//�ٴζ�ȡһ���ж�
    }
  FLASH_Lock();						//��
}
/**********************************************************
*������:First_ReadData
*����:�ϵ��ȡ���ݣ��ж��Ƿ��һ���ϵ磬�ǵ�һ�����ݸ���ʼֵ��
*�������:��
*�������:��
**********************************************************/
void  save_INCnt(void)
{
  FLASH_UnLock();						//����
  FLASH_Write(FLASH_DATA,0x2,eb_voice_input_channel);	//д������
  FLASH_Lock();						//��
}

/**********************************************************
*������:First_ReadData
*����:�ϵ��ȡ���ݣ��ж��Ƿ��һ���ϵ磬�ǵ�һ�����ݸ���ʼֵ��
*�������:��
*�������:��
**********************************************************/
void  save_OutCnt(void)
{

  FLASH_UnLock();						//����
  //
  //	FLASH_Erase(FLASH_DATA,0x3);
  FLASH_Write(FLASH_DATA,0x3, eb_voice_output_channel);	//д������
  //
  FLASH_Lock();						//��

}


/**********************************************************
*������:First_ReadData
*����:�ϵ��ȡ���ݣ��ж��Ƿ��һ���ϵ磬�ǵ�һ�����ݸ���ʼֵ��
*�������:��
*�������:��
**********************************************************/
void  save_VoiceLevel(void)
{
  //uint8_t temp=0;
  FLASH_UnLock();						//����
  FLASH_Write(FLASH_DATA,0x4,eb_voice_level);	//д������
  FLASH_Lock();						//��
}