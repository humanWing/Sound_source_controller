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

void First_ReadData()
{
  u8 temp1,temp2;						//��ʱ���ݱ���
  //
  FLASH_UnLock();						//����
firstread:
  temp1 = FLASH_Read(FLASH_DATA,0);
  temp2 = FLASH_Read(FLASH_DATA,1);
  //
  if((temp1 == 0x55 ) && (temp2 == 0x2a))	//����0x55 0xaa˵����һ���ϵ硣
    {
      VarINCnt =FLASH_Read(FLASH_DATA,0x2);
      VarOutCnt = FLASH_Read(FLASH_DATA,0x3);
      VarVoiceLevel = FLASH_Read(FLASH_DATA,0x4);
    }
  else
    {
      VarINCnt =1;
      VarOutCnt = 1;
      VarVoiceLevel = 1;
      //
      FLASH_Erase(FLASH_DATA,0);
      //for(i = 0 ;i< 3 ;i++)		//����256 bytes��д�ȴ�Flashִ�����
      //{
      //	FLASH_Write(FLASH_DATA,0x4, 0xFF); //д��ַʹ�����ĵ�ַ(�����ַ�����ԣ�������ʹ�ý��ٵĵ�ַ)
      //}
      FLASH_Write(FLASH_DATA,0x0, 0x55);			//д���־
      //temp = FLASH_Read(FLASH_DATA,0x0);
      FLASH_Write(FLASH_DATA,0x1, 0x2a);
      //temp = FLASH_Read(FLASH_DATA,0x1);
      //
      FLASH_Write(FLASH_DATA,0x2, VarINCnt);	//д���ʼ����
      // temp = FLASH_Read(FLASH_DATA,0x2);
      FLASH_Write(FLASH_DATA,0x3, VarOutCnt);
      //temp = FLASH_Read(FLASH_DATA,0x3);
      FLASH_Write(FLASH_DATA,0x4, VarVoiceLevel);
      //temp = FLASH_Read(FLASH_DATA,0x4);
      //
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
  //
  //FLASH_Erase(FLASH_DATA,0x02);
  FLASH_Write(FLASH_DATA,0x2,VarINCnt);	//д������
  //
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
  FLASH_Write(FLASH_DATA,0x3, VarOutCnt);	//д������
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
  //
  //FLASH_Erase(FLASH_DATA,0x4);
  FLASH_Write(FLASH_DATA,0x4,VarVoiceLevel);	//д������
  //temp = FLASH_Read(FLASH_DATA,0x4);
  //
  FLASH_Lock();						//��
}