#include "Log.h"

static int RecordIndex=0;

int8_t CompareRecordDate(EnergyRecord record,HRF_date_TypeDef date) //return 1 if record time greater
{
//HRF_date_TypeDef result;
if(record.Year>date.Year)return 1;
if(record.Year<date.Year)return -1;
//now years are same
if(record.Mon>date.Month)return 1;
if(record.Mon<date.Month)return -1;
//now months are same
if(record.Day>date.Day)return 1;
if(record.Day<date.Day)return -1;
//now days are same

if(record.Day>date.Hours)return 1;
if(record.Day<date.Hours)return -1;

if(record.Day>date.Minutes)return 1;
if(record.Day<date.Minutes)return -1;


return 0;

//return result;
}
//=======================================================================================================

void LogInit(void)
{
char temp[50];
int i;
int lastUpdateRecord=0;
EnergyRecord record;
bool result=false;
HRF_date_TypeDef maxDate;


maxDate.Year=0;
maxDate.Month=1;
maxDate.Day=1;
maxDate.Hours=0;
maxDate.Minutes=0;



delay_ms(700); 
for(i=0;i<ENERGY_RECORD_MAX_ADDRESS/sizeof(EnergyRecord);i++)	//in normal 4032/12=336
 {
 
 result=ReadEnergyRecord(i,&record);	//read record
  if(!result)continue; 								//skip nothing with invalid records
  if( CompareRecordDate(record,maxDate)<0)continue; //record date smaller
 
  	
	RecordIndex=i;
	maxDate.Year=record.Year;
	maxDate.Month=record.Mon;
	maxDate.Day=record.Day;
	maxDate.Hours=record.Hour;
	maxDate.Minutes=record.Min;

 }

  sprintf(temp,"\n%d %d %d %d %d index=%d\n",	maxDate.Year,maxDate.Month,maxDate.Day,maxDate.Hours,maxDate.Minutes,RecordIndex);
  send_string(temp);

}
//=======================================================================================================
bool ReadEnergyRecord(int recordNum ,EnergyRecord *record)
{

 char temp[30];
uint8_t *data=0;
//uint8_t data[15];

uint16_t  crc=0xffff;
int i=0;

recordNum*=sizeof(EnergyRecord);
data=(uint8_t*)record;

///////////////

//if(!I2C_EE_BufferRead(data,recordNum,12))
//{send_string("eeprom read \n");return false; }
//for(i=0;i<12;i++){
//if(!I2C_EE_ByteRead(recordNum+i,&data[i]))
//{
// send_string("eeprom read \n");return false;
//}
//}
//
//for(i=0;i<sizeof(EnergyRecord)-2;i++)
//{
//sprintf(temp,"%x ",data[i]);
//send_string(temp);
//
//	 crc=update_crc_16(crc,data[i]);
//
//} 	 
//

 for(i=0;i<sizeof(EnergyRecord);i++)
 {
 	 
  if(I2C_EE_ByteRead(recordNum+i,data)==0)  
  {
  send_string("ReadError\n");
  	return false;

  }
//sprintf(temp,"%x ",data[i]);
//send_string(temp);
	if((sizeof(EnergyRecord)-2)>i){
	crc=update_crc_16(crc,*data);
	
	}
	data++;
  

 }


////////////////
//sprintf(temp,"calc crc=%x crc=%x\n",crc,record->Crc);
//send_string(temp);
//sprintf(temp,"crc =%x rcrc=%x\n",crc,record->Crc);
//send_string(temp);

if(crc==record->Crc)
 return true;

return false;
}

//=======================================================================================================

bool WriteEnergyRecord(int recordNum ,EnergyRecord record)
{
uint8_t *data;
//char temp[20];
int i;
uint16_t  crc=0xffff;

recordNum*=sizeof(EnergyRecord);
if(recordNum >ENERGY_RECORD_MAX_ADDRESS)return 0;
data=(uint8_t*)(&record);

 ///////////////  write data
for(i=0;i<(sizeof(EnergyRecord)-2);i++)
{
 
if(I2C_EE_ByteWrite(*data,recordNum+i)==0)  
{
send_string("write Error\n");
return false;

}
crc=update_crc_16(crc,*data);
data++;
  

 }
//sprintf(temp,"crc =%x\n",crc);
//send_string(temp);
//////////////// write crc
if(!I2C_EE_ByteWrite((crc&0xff),sizeof(EnergyRecord)-2+recordNum))
{
send_string("write Error2\n");
return false;
}
crc>>=8;//get high byte
if( !I2C_EE_ByteWrite((crc&0xff),sizeof(EnergyRecord)-1+recordNum))
{
send_string("write Error3\n");
return false;
}

/////////////////
return true;



}
//=======================================================================================================
bool AppendEnergyRecord(EnergyRecord record)
{
bool result=false;
 
RecordIndex++;
result=WriteEnergyRecord( RecordIndex ,record );
if(result)return result;

RecordIndex--; 
return result;

}
//=============================================================================================
