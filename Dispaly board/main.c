#include <stm32f10x_lib.h>
#include <stdio.h>
#include <string.h>
///#include <stdbool.h>
	//C:\Users\Amin\Downloads\freezed version of 2.01\freezed version of 2.01\MDK-ARM\autoclave usart test with timer+usart irq
#include "stm32_init.h"
#include "serial//serial.h"
#include "delay//delay.h"
#include "SystemInit/SystemInit.h"
#include "SSD1306//SSD1306.h" 
#include "SSD1306//font7X10.h"
#include "SSD1306//font5X7.h"
#include "eeprom//memory.h"
#include "commandline//commandline.h"
#include "Log//Log.h"
#include "CRC//lib_crc.h"
#include "ds3231//ds3231.h"
#define USART1_BUFFER_SIZE 40
#define USART3_BUFFER_SIZE 20
//--------------------------------------------------------------------------------------------------------------------
char Usart1Buff[USART1_BUFFER_SIZE];
int Usart1BuffIndex=0;
char Usart3Buff[USART3_BUFFER_SIZE];
int Usart3BuffIndex=0;
char temp_main[70];
float InputCurrent=0,InputVoltage=0;
bool DeviceInitialized=false;
float TotalEnergy=0;
float Voltage=0,Current=0;
int CurrentTemperature=23;
int i;
HRF_date_TypeDef CurrentDate;
u8 Rx1_Buffer[10];

// Tile bitmap
static const uint8_t bmp_tile[] = {
		0x38,0x44,0x82,0x29,0x11,0x29,0x82,0x44,0x38,0x44,0x82,0x11,0x29,0x11,
		0x82,0x44,0x38,0x44,0x82,0x11,0x29,0x11,0x82,0x44,0x38,0x44,0x82,0x29,
		0x11,0x29,0x82,0x44,0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x00,0x00,0x00,
		0x00,0x01,0x01,0x01,0x00,0x00
};
//=========================================================================================================================

int RadTemp()
{ 
return 0;
}
//=========================================================================================================================
void USART1_IRQHandler()
{
   char ch=getkey();
	   
Usart1Buff[Usart1BuffIndex]=ch;
Usart1BuffIndex++;
Usart1Buff[Usart1BuffIndex]=0;

if(ch==10)
{
//  send_string(Usart1Buff);
  CommandlineHandle(Usart1Buff);
  Usart1BuffIndex=0;
  Usart1Buff[0]=0;
}
if(Usart1BuffIndex>USART1_BUFFER_SIZE)Usart1BuffIndex=0;				

}
//=========================================================================================================================
void USART3_IRQHandler()
{
char ch=getkey3();

Usart3Buff[Usart3BuffIndex]=ch;
Usart3BuffIndex++;
if(Usart3BuffIndex>=USART3_BUFFER_SIZE)	 Usart3BuffIndex=0;
Usart3Buff[Usart3BuffIndex]=0;



if(ch==10)
{
//send_string(Usart3Buff) ;
sscanf(Usart3Buff,"%f %f",&InputCurrent,&InputVoltage) ;
if(InputCurrent<0.1)InputCurrent=0;
 Usart3Buff[0]=0;
     Usart3BuffIndex=0;
}
//sendchar(ch);
//if(ch=='I')iFlag=1;
//if(ch=='V')vFlag=1;
//if(vFlag==1 && ch==":")vFlag=2;
//if(vFlag==1 && ch==":")vFlag=2;



}
 //=================================================================================
 void DoLog()
 {
 
 EnergyRecord record;
 send_string("Start Log...\n");
 //if(!DS3231_ReadDate(&CurrentDate)){send_string("Log RTC Error...\n");return;	}
 record.Year=CurrentDate.Year;
 record.Mon=CurrentDate.Month;
 record.Day=CurrentDate.Day;
 record.Hour=CurrentDate.Hours;
 record.Min=0;//min is alwasy zero becase of sampling period
 record.Temp=25;
 record.Energy=1288;
 
 if(!AppendEnergyRecord(record))
 {
 send_string("Log error!\n");
 }
 



 
 }
//=========================================================================================================================

void DisplayValues(unsigned long int refereshTime)
{ 
float temp;
static unsigned long int roundRobinLcd=0;
 if(roundRobinLcd++>refereshTime)
 {

	SSD1306_Fill(0x00);
	//DS3231_ReadDate(&CurrentDate);
	sprintf(temp_main,"Time %2d:%2d:%2d",CurrentDate.Hours,CurrentDate.Minutes,CurrentDate.Seconds);
	LCD_PutStr(1,5,temp_main,fnt7x10,( uint8_t *)fdata);
	sprintf(temp_main,"I=%2.2f",InputCurrent);
	LCD_PutStr(1,20,temp_main,fnt7x10,( uint8_t *)fdata);
	sprintf(temp_main,"V=%3.2f",InputVoltage);
	LCD_PutStr(1,35,temp_main,fnt7x10,( uint8_t *)fdata);
	sprintf(temp_main,"P=%3.2f",InputVoltage*InputCurrent*0.9);
	LCD_PutStr(1,50,temp_main,fnt7x10,( uint8_t *)fdata);
	
	SSD1306_Flush();
	roundRobinLcd=0;
	send_string("test\n");

  
}

}
//----------------------------------------------------------------------------------

void OnStartButtonPresed()
{

  	  	 //send_string("start\n");
}
//=================================================================================

void OnStopButtonPresed()
{
 	 //send_string("stop\n");
}
//=================================================================================

void OnUpButtonPresed()
{
  	// send_string("up\n");

}
//=================================================================================

void OnDownButtonPresed()
{
     	// send_string("down\n");
}
//=================================================================================

void OnPlusButtonPresed()
{
  	// send_string("plus\n");

}
//=================================================================================

void OnMinusButtonPresed()
{
    	// send_string("min\n");
}
//=================================================================================
void CheckKeyPad()
{
  static bool lock=false;
  static int lockCount=0;
  if(!AnyKeyPressed)
  {
  	 lock=0;
	 lockCount=0;

 return;
  }
if(!lock)
{
lock=true;
 Beep(10);


if(DownButton)
OnDownButtonPresed();
if(PlusButton)
OnPlusButtonPresed();
if(MinusButton)
OnMinusButtonPresed();
if(UpButton)
OnUpButtonPresed();

 return;
}
lockCount++;
if(lockCount<KeyLockThreshold)
return;
lock=0;
lockCount=0;

}

 ////////////////////////////////////////////////////////////////////////Commandline Functions
 //=================================================================================
 void Ping(char*par)
 {
DoLog();
 }
 //=================================================================================
 void EraseMemory(char*par)
 {
    send_string("Erase Mem\n");
 }
 //=================================================================================
 void GetInfo(char*par)
 {
 send_string("GetInfo\n");

 }
 //=================================================================================
 void GetOnlineParameters(char*par)
 {
sprintf(temp_main,"%3.1f,%3.1f,%6.1f,%d\n",InputVoltage,InputCurrent,TotalEnergy,CurrentTemperature);
send_string(temp_main); 


 }
 //=================================================================================
 void GetDate(char*par)		  
 {
EnergyRecord rec;
int addr=0;


sscanf(par,"%d",&addr) ;
if(ReadEnergyRecord(addr,&rec))
{
sprintf(temp_main,"%d %d %d %d %d %d %d\n",rec.Year,rec.Mon,rec.Day,rec.Hour,rec.Min,rec.Energy,rec.Temp);
send_string(temp_main); 
return ;
}
sprintf(temp_main,"read bad crc @%d\n ",addr);
send_string(temp_main); 

 }
 //=================================================================================
 void SetDate(char*par)
 {
/*

uint8_t		Year;
uint8_t		Mon;
uint8_t		Day;
uint8_t		Hour;
uint8_t		Min;
uint32_t	Energy;
uint8_t 	Temp;
*/
EnergyRecord rec;
int addr=0;


 sscanf(par,"%d",&addr) ;
rec.Year=18;
rec.Mon=11;
rec.Day=16;
rec.Hour=1;
rec.Min=59;
rec.Energy=2345;
rec.Temp=25;
 if( WriteEnergyRecord(addr ,rec))
 {
send_string("write crc ok\n"); 
return;
	}
  sprintf(temp_main,"write error @%d\n ",addr);
 send_string(temp_main); 
 }
 //=================================================================================
 void GetTime(char*par)
 {
 //	uint8_t  Month;
//	uint16_t Year;
	//uint8_t  DOW;
 //send_string("GetTime\n");
 	//if(DS3231_ReadDate(&CurrentDate)){
	//sprintf(temp_main,"%d-%d-%d -%d- %2d:%2d:%2d T=%d C\n",CurrentDate.Year,CurrentDate.Month,CurrentDate.Day,CurrentDate.DOW,CurrentDate.Hours,CurrentDate.Minutes,CurrentDate.Seconds,27);
	sprintf(temp_main,"%d-%02d-%02d %02d:%02d:%02d\n",CurrentDate.Year,CurrentDate.Month,CurrentDate.Day,CurrentDate.Hours,CurrentDate.Minutes,CurrentDate.Seconds);
	send_string(temp_main);			  
	
	
//	}
//	else{
//	send_string("error!");			
	  
//	}
 }

 //=================================================================================
 void SetTime(char*par)
 {
HRF_date_TypeDef date;
int h=0,m=0,s=0,d=0,mm=0,y=0,dow=0;


 sscanf(par,"%d %d %d %d %d %d %d",&y,&mm, &d, &dow,&h, &m, &s) ;
if(y>99 || y<0)y=0;
if(mm>12 || mm<1)mm=1;
if(d>30 || d<1)d=1;
if(dow>7 || dow<1)dow=1;
if(h>23 || h<0)h=0;
if(m>59 || m<0)m=0;
if(s>59 || s<0)s=0;


 date.Year=y;
 date.Month=mm;
 date.Day=d;
 date.DOW=dow;
 date.Hours=h;
 date.Minutes=m;
 date.Seconds=s;
 //sprintf(temp_main,"date= %d %d %d %d %d %d %d\n",date.Year,date.Month,date.Day, date.DOW,date.Hours, date.Minutes, date.Seconds) ;
 //send_string(temp_main);
if(DS3231_WriteDate(&date)){
send_string("OK");
}
else send_string("Error");

 }
 //=================================================================================
 void GetPeackTime(char*par)
 {
 send_string("GetPeackTime\n");
 }
 //=================================================================================
 void SetPeakTime(char*par)
 {
 send_string("SetPeakTime\n");
 }
 //=================================================================================
 void GetConsumePattern(char*par)
 {
 send_string("GetConsumePattern\n");
 }
 //=================================================================================
 void SetConsumePattern(char*par)
 {
 send_string("SetConsumePattern\n");
 }
 //=================================================================================
 void GetLedBarLevels(char*par)
 {
 send_string("GetLedBarLevels\n");
 }
 //=================================================================================
 void SetLedBarLevels(char*par)
 {
 
 send_string("set bar\n");
 }
 //=================================================================================
void ReadRecord(char*par)
{
/*
uint8_t		Year;
uint8_t		Mon;
uint8_t		Day;
uint8_t		Hour;
uint8_t		Min;
uint32_t	Energy;
uint8_t 	Temp;
*/
int address=0;
EnergyRecord record;

sscanf(par,"%d",&address);
//ReadEnergyRecord(address,&record);
if(!ReadEnergyRecord(address,&record))
{
sprintf(temp_main,"error=%d\n",address);
send_string( temp_main );
return ;

}
sprintf(temp_main,"%d %d %d %d %d %d %d\n",2000+record.Year,record.Mon,record.Day,record.Hour,record.Min,record.Energy,record.Temp);
send_string( temp_main );

}
 //=================================================================================
 void ReadMemory(char*par)
 {
 uint8_t data=0;
 int i=0;
 unsigned long crc=0xffffffff;
send_string("read mem\n");

 for(i=0;i<4096;i++)
 {
  if(I2C_EE_ByteRead(i,&data)==0)
{
send_string("ReadError\n");
return;
}
crc=update_crc_32(data,crc);
sendchar(data);


 }
 if(i==4096){
   sprintf(temp_main,"OK %X",crc);
   send_string(temp_main);
 }
 else{
   send_string("error");
 }
 
 }
 //=================================================================================
 ////////////////////////////////////////////////////////////////////////Commandline Functions

void TIM1_UP_IRQHandler()
{
static int lastUpdateHour=-1;
TIM1->SR&=~(1<<0);
if(!DeviceInitialized)return;

if(lastUpdateHour<0){
lastUpdateHour=CurrentDate.Hours+1;
if(lastUpdateHour==24){lastUpdateHour=0;}
send_string("define last update for first\n");
}
 TotalEnergy+=InputCurrent*InputVoltage*1;
//energy test add
 //TotalEnergy+=1;
//sprintf(temp_main,"%d %d %d\n",CurrentDate.Hours,CurrentDate.Minutes,CurrentDate.Seconds) ;
//send_string(temp_main);
 //-------------------------------------
 if(CurrentDate.Hours==lastUpdateHour && CurrentDate.Minutes==0)
 {
  send_string("log to file\n");
//log to file
lastUpdateHour=CurrentDate.Hours+1;
if(lastUpdateHour==24){lastUpdateHour=0;}
  TotalEnergy=0;
 }
 //-------------------------------------

}
//=========================================================================================================================
int main(){
//float t,h;

  
stm32_Init();
//jtag_Disable();
//RGBInit(); 
DS3231Init();
//I2C_Configuration();

send_string("hello\n");


//LogInit();
//DeviceInitialized=true;
 while(1){
 
 if(!DS3231_ReadDate(&CurrentDate))
{

send_string("rtc error\n");


}
//sprintf(temp_main,"%d %d %d\n",CurrentDate.Hours,CurrentDate.Minutes,CurrentDate.Seconds) ;
//send_string(temp_main);

delay_ms(1000);

 }
  
//SSD1306_InitGPIO();
 //adc_init(1);
//DS3231Init();


	// Initialize display
	//SSD1306_InitGPIO();
	//SSD1306_Init();

	// Screen orientation normal (FPC cable at the bottom)
	//SSD1306_Orientation(LCD_ORIENT_NORMAL);

	// Mid level contrast
	//	SSD1306_Contrast(127);


	// Now do some drawing

	// Clear video buffer
	//SSD1306_Fill(0x00);

	// Drawing mode: set pixels
//	LCD_PixelMode = LCD_PSET;

	// Draw a couple of strings
	//LCD_PutStr(35,11,"SSD1306",fnt7x10,( uint8_t *)fdata);
	//LCD_PutStr(19,43,"OLED 128x64",fnt7x10);
	//	LCD_HLine(0,60,60);
		// Send video buffer to the screen


		// Draw tiled bitmap
//	for (i = 0; i < scr_width - 1; i += 16)	LCD_DrawBitmap(i,23,16,17,bmp_tile);

//	SSD1306_Flush();
// Configure display to scroll horizontally right
//	SSD1306_ScrollHSetup(LCD_SCROLL_RIGHT,2,5,LCD_SCROLL_IF25);

	// Start hardware display scrolling
//	SSD1306_ScrollStart();

	// Delay for 5 seconds
//	delay_ms(5000);

	// Stop scrolling
//	SSD1306_ScrollStop();

	// Configure display to scroll horizontally left with maximum speed
//	SSD1306_ScrollHSetup(LCD_SCROLL_LEFT,2,5,LCD_SCROLL_IF2);

	// Start hardware display scrolling
//	SSD1306_ScrollStart();

	// Delay for 5 seconds
//	delay_ms(5000);

	// Stop scrolling
//	SSD1306_ScrollStop();
//SetRGB(1,1,1);
//delay_ms(1000);
//		CurrentDate.seconds=0;
//	  	CurrentDate.minutes=0;
//	    CurrentDate.hours=0;
//		CurrentDate.date=19;
//	    CurrentDate.month=10;
//	    CurrentDate.year=18;
//		DS3231_WriteDateRAW(&CurrentDate);
//I2C_Configuration();
//send_string("itc init ok\n");
//I2C_EE_ByteWrite(0x01,175);
//I2C_EE_ByteWrite(0x02,178);

//for(i=0;i<10;i++){

//void I2C_EE_ByteWrite(u8 pBuffer, u8 WriteAddr)

//}
send_string("i2c init ok\n");	  //384ye68
//I2C_EE_ByteWrite(177,4095);
 for(i=0;i<10;i++){
 //I2C_EE_ByteWrite(i+40,i);
Rx1_Buffer[i]=0;
}
//I2C_EE_ByteWrite(Rx1_Buffer,i+1);
//} 
while(1){
//	DS3231_ReadDate(&CurrentDate);
//	sprintf(temp_main,"%d %d %d\n",CurrentDate.Hours,CurrentDate.Minutes,CurrentDate.Seconds) ;
//	delay_ms(300);
//	send_string(temp_main);
	   			//DisplayValues(200);
				 //CheckKeyPad();
	//	sprintf(temp_main,"D0=%d D1=%d\n",InputCurrent,InputVoltage) ;
	//SetRGB(0,1,0);
	delay_ms(300);
	 //void I2C_EE_BufferRead(u8* pBuffer, u8 ReadAddr, u16 NumByteToRead);
	 I2C_EE_BufferRead(Rx1_Buffer, 0, 2); 
//sprintf(temp_main,"D0=%d %d %d\n",Rx1_Buffer[0],Rx1_Buffer[1],I2C_EE_ByteRead(4095)) ;
//sprintf(temp_main,"D0=%d \n",I2C_EE_ByteRead(0)) ;
//	send_string(temp_main);

		



		}//end while
	}//end main
