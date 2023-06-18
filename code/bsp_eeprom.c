/*
*********************************************************************************************************
*
*	模块名称 :
*	文件名称 : bsp_eeprom.c
*	版    本 : V1.0
*	说    明 : 用于保存用户配置，掉电保存
*
*	修改记录 :
*		版本号  日期         作者     说明
*
*********************************************************************************************************
*/
#include "all.h"
//
//demo 测试代码
#if 0
void data_write()
{
  u8 i;
  FLASH_UnLock();						//解锁
  addr= 0;
  Dtemp =0;
  FLASH_Erase(FLASH_DATA,addr);
  for(i = 0 ; i< 256 ; i++)		//连续256 bytes的写等待Flash执行完成
    {
      FLASH_Write(FLASH_DATA,0x3ff, 0xFF); //写地址使用最后的地址(任意地址都可以，建议用使用较少的地址)
    }
  for(addr=0; addr<0x10; addr++)
    {
      FLASH_Write(FLASH_DATA,addr, Dtemp++);
      temp = FLASH_Read(FLASH_DATA,addr);
    }
  FLASH_Lock();						//锁
}
#endif

/**********************************************************
*函数名:First_ReadData
*描述:上电读取数据，判断是否第一次上电，是第一次数据赋初始值。
*输入参数:无
*输出参数:无
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
  u8 temp1,temp2;						//临时数据保存

  FLASH_UnLock();						//解锁
firstread:
  temp1 = FLASH_Read(FLASH_DATA,0);
  temp2 = FLASH_Read(FLASH_DATA,1);
  //
  if((temp1 == 0x55 ) && (temp2 == 0x2a))	//不是0x55 0xaa说明是一次上电。
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
      FLASH_Write(FLASH_DATA,0x0, 0x55);			//写入标志
      FLASH_Write(FLASH_DATA,0x1, 0x2a);
      FLASH_Write(FLASH_DATA,0x2, eb_voice_input_channel);	//写入初始数据
      FLASH_Write(FLASH_DATA,0x3, eb_voice_output_channel);
      FLASH_Write(FLASH_DATA,0x4, eb_voice_level);
      bsp_eer_delay(100);
      goto firstread;			//再次读取一次判断
    }
  FLASH_Lock();						//锁
}
/**********************************************************
*函数名:First_ReadData
*描述:上电读取数据，判断是否第一次上电，是第一次数据赋初始值。
*输入参数:无
*输出参数:无
**********************************************************/
void  save_INCnt(void)
{
  FLASH_UnLock();						//解锁
  FLASH_Write(FLASH_DATA,0x2,eb_voice_input_channel);	//写入数据
  FLASH_Lock();						//锁
}

/**********************************************************
*函数名:First_ReadData
*描述:上电读取数据，判断是否第一次上电，是第一次数据赋初始值。
*输入参数:无
*输出参数:无
**********************************************************/
void  save_OutCnt(void)
{

  FLASH_UnLock();						//解锁
  //
  //	FLASH_Erase(FLASH_DATA,0x3);
  FLASH_Write(FLASH_DATA,0x3, eb_voice_output_channel);	//写入数据
  //
  FLASH_Lock();						//锁

}


/**********************************************************
*函数名:First_ReadData
*描述:上电读取数据，判断是否第一次上电，是第一次数据赋初始值。
*输入参数:无
*输出参数:无
**********************************************************/
void  save_VoiceLevel(void)
{
  //uint8_t temp=0;
  FLASH_UnLock();						//解锁
  FLASH_Write(FLASH_DATA,0x4,eb_voice_level);	//写入数据
  FLASH_Lock();						//锁
}