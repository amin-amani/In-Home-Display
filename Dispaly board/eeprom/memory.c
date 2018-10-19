#include "memory.h"
#include "..//CRC//lib_crc.h"
#include <stdio.h>


u8 temp[40];
 u16 VirtAddVarTab[NumbOfVar] = { 0xd150,0xd250,0xd350};
FlashMemory *Memory;
u16 buffer[sizeof(FlashMemory)/2];
//=========================================================================================================
// unsigned int CreateMemCRC(FlashMemory dt)
//{
 //unsigned int crc_result= 0xffffffff;
 //unsigned int i=0;
 
//char*ptr=(char*)(&dt);
 //for(i=0; i<sizeof(FlashMemory)-4; i++)
// 	{
	// crc_result = update_crc_32(crc_result ,ptr[i]);	 
//	}
 
 //return (crc_result^0xffffffffL);
//}
//======================================================================================================
unsigned int CreateMemCRC2()
{
 unsigned int crc_result= 0xffffffff;
 unsigned int i=0;
 
char*ptr=(char*)(Memory);

 for(i=0; i<sizeof(FlashMemory)-4; i++)
 	{
 	crc_result = update_crc_32(crc_result ,ptr[i]);
 
	}

 return (crc_result^0xffffffffL);
}

//======================================================================================================
u16 SaveMemory(){
u16 status=0;
int i;
u16 *ptr;

Memory=(FlashMemory*)(buffer);
Memory->crc=CreateMemCRC2();


ptr=(u16*)Memory;
   FLASH_Unlock();
  /* EEPROM Init */
status= EE_Init();
for(i=0;i<sizeof(FlashMemory)/2;i++){
 EE_WriteVariable(VirtAddVarTab[0]+i, ptr[i]);
}
FLASH_Lock();
return status;
}
//=========================================================================================================
bool LoadMemory(){

 int i;
 unsigned int calc_crc=0;
 if(EE_Init()!=FLASH_COMPLETE){send_string("err\n");return 0;}


for(i=0;i<sizeof(FlashMemory)/2;i++){
 EE_ReadVariable(VirtAddVarTab[0]+i,&buffer[i]);
}
Memory=(FlashMemory*)(buffer);
calc_crc=CreateMemCRC2();

if(calc_crc!=Memory->crc){return 0;}
return 1;

}
//=========================================================================================================
