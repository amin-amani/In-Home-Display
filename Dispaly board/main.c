/*
hardware test....................
led red			ok			 
led green		ok
ledbar			ok
RTC				ok
buzzer			ok
Temp			OK
wifi
MeterData		ok
lcd				ok
key				ok
			
*/
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

#define LEDR_ON GPIOB->ODR|=1<<0 
#define LEDR_OFF GPIOB->ODR&=~(1<<0)
#define LEDG_ON GPIOB->ODR|=(1<<1)
#define LEDG_OFF GPIOB->ODR&=~(1<<1)


//--------------------------------------------------------------------------------------------------------------------
char Usart1Buff[USART1_BUFFER_SIZE];
int Usart1BuffIndex=0;
char Usart3Buff[USART3_BUFFER_SIZE];
int Usart3BuffIndex=0;
char temp_main[70];
float InputCurrent=0,InputVoltage=0;
bool DeviceInitialized=false;
float TotalEnergy=0;
//float Voltage=0,Current=0;
int CurrentTemperature=23;
int i;
HRF_date_TypeDef CurrentDate;
u8 Rx1_Buffer[10];
int DisplayPage=0;

const float ConsumePattern[24]={14.17,13.73,12.96,12.97,13.43,12.99,13.33,14.23,15.46,16.47,16.37,16.49,16.28,
16.29,16.07,16.42,17.32,20.43,21.00,20.56,19.69,18.69,17.04,15.60};



// Tile bitmap
//static const uint8_t bmp_tile[] = {
//		0x38,0x44,0x82,0x29,0x11,0x29,0x82,0x44,0x38,0x44,0x82,0x11,0x29,0x11,
//		0x82,0x44,0x38,0x44,0x82,0x11,0x29,0x11,0x82,0x44,0x38,0x44,0x82,0x29,
//		0x11,0x29,0x82,0x44,0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x00,0x00,0x00,
//		0x00,0x01,0x01,0x01,0x00,0x00
//};

//=========================================================================================================================
void SetLedBar(int level)
{
GPIOA->ODR&=~(0x18fe);

if(level>0){
GPIOA->ODR|=1<<1;

}
if(level>1){
GPIOA->ODR|=1<<2;

}
if(level>2){
GPIOA->ODR|=1<<3;

}
if(level>3){

GPIOA->ODR|=1<<4;
}
if(level>4){
GPIOA->ODR|=1<<5;

}
if(level>5){
GPIOA->ODR|=1<<6;

}
if(level>6){
GPIOA->ODR|=1<<7;
}
if(level>7){
GPIOA->ODR|=1<<11;
}
if(level>8){
GPIOA->ODR|=1<<12;
}

}
//=========================================================================================================================

double ReadTemp()
{ 	  
double result=0;
for(i=0;i<50;i++)
result+=readadc(0);
result/=50;

 result =result*330/4095.0;
 result-=273.0;


return result;
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

void CalcConsumptionPatern(float Current,uint8_t hour)
{
 float calc=0;
calc=100*(Current-ConsumePattern[hour]);
calc/=(ConsumePattern[hour]);

 // sprintf(temp_main,"I=%f h=%d  pat=%f calc=%f\n",Current,hour,ConsumePattern[hour],calc);
 // send_string(temp_main);
if(calc<-12.5)
{
SetLedBar(0);
return;
}
if(calc<-7.5)
{
SetLedBar(1);
return;
}
if(calc<-2.5)
{
SetLedBar(2);
return;
}
if(calc<1.5)
{
SetLedBar(3);
return;
}
if(calc<4.5)
{
SetLedBar(4);
return;
}
if(calc<7.5)
{
SetLedBar(5);
return;
}
if(calc<10.5)
{
SetLedBar(6);
return;
}
 if(calc<13.5)
{
SetLedBar(7);
return;
}
 if(calc<17.5)
{
SetLedBar(8);
return;
}
SetLedBar(9);

}
//=========================================================================================================================

void DisplayValues(unsigned long int refereshTime)
{ 
float temp;
static unsigned long int roundRobinLcd=0;
 if(roundRobinLcd++>refereshTime)
 {

if(!DS3231_ReadDate(&CurrentDate))
{

send_string("rtc error\n");


}

//=========================================================================================================================

CalcConsumptionPatern(InputCurrent,CurrentDate.Hours);

	SSD1306_Fill(0x00);
	//DS3231_ReadDate(&CurrentDate);
	if(DisplayPage==0){
	sprintf(temp_main,"    %02d:%02d:%02d",CurrentDate.Hours,CurrentDate.Minutes,CurrentDate.Seconds);
	LCD_PutStr(1,10,temp_main,fnt7x10,( uint8_t *)fdata);
		sprintf(temp_main,"    %04d/%02d/%02d",CurrentDate.Year,CurrentDate.Month,CurrentDate.Day);
	LCD_PutStr(1,30,temp_main,fnt7x10,( uint8_t *)fdata);
	CurrentTemperature=ReadTemp();
	sprintf(temp_main,"TEMP: %d C",(int)CurrentTemperature);
	LCD_PutStr(28,50,temp_main,fnt7x10,( uint8_t *)fdata);
	
						   }
	if(DisplayPage==1){
//	sprintf(temp_main,"Time %2d:%2d:%2d",CurrentDate.Hours,CurrentDate.Minutes,CurrentDate.Seconds);
	LCD_PutStr(30,5,"Metering",fnt7x10,( uint8_t *)fdata);
	sprintf(temp_main,"I=%2.2f",InputCurrent);
	LCD_PutStr(1,20,temp_main,fnt7x10,( uint8_t *)fdata);
	sprintf(temp_main,"V=%3.2f",InputVoltage);
	LCD_PutStr(1,35,temp_main,fnt7x10,( uint8_t *)fdata);
	sprintf(temp_main,"P=%3.2f",InputVoltage*InputCurrent*0.9);
	LCD_PutStr(1,50,temp_main,fnt7x10,( uint8_t *)fdata);
						   }
	SSD1306_Flush();
	roundRobinLcd=0;

//sprintf(temp_main,"%x",GPIOA->IDR);
//	send_string(temp_main);
	 // SetLedBar(InputCurrent)
  
}

}
//----------------------------------------------------------------------------------

void OnSelectKeyPresed()
{
  Beep(30);
  DisplayPage++;
  if(DisplayPage>1)DisplayPage=0;
     	// send_string("down\n");
}

//=================================================================================
void CheckKeyPad()
{
  static bool lock=false;
  static int lockCount=0;
  if(SelectKEY)
  {
  	 lock=0;
	 lockCount=0;
	   
 return;
  }
if(!lock)
{
lock=true;



if(!SelectKEY)
OnSelectKeyPresed();


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
   send_string("Pong\n");
 }
 //=================================================================================
 void GetInfo(char*par)
 {
 send_string("GetInfo\n");

 }
 //=================================================================================
 void EraseMemory(char*par)
 {
    send_string("Erase Mem...\n");
	if(EraseEnergyRecords())
	{
	      send_string("OK\n");
		  return;
	}
	    send_string("Error\n");
 }

 //=================================================================================
 void GetOnlineParameters(char*par)
 {
sprintf(temp_main,"%3.1f,%3.1f,%6.1f,%d\n",InputVoltage,InputCurrent,TotalEnergy,CurrentTemperature);
send_string(temp_main); 


 }

 //=================================================================================
 void GetTime(char*par)
 {
	sprintf(temp_main,"%d-%02d-%02d %02d:%02d:%02d\n",CurrentDate.Year,CurrentDate.Month,CurrentDate.Day,CurrentDate.Hours,CurrentDate.Minutes,CurrentDate.Seconds);
	send_string(temp_main);			  
	
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
 void SetLedBarLevels(char*par)
 {
int level=0;


sscanf(par,"%d",&level);
SetLedBar(level);
 }
  //=================================================================================
 void SetVI(char*par)
 {



sscanf(par,"%f %f",&InputVoltage,&InputCurrent);

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
jtag_Disable();
delay_ms(1000);
//RGBInit(); 
DS3231Init();
I2C_Configuration();
  adc_init(1);
send_string("hello\n");
//----------------LCD INITIALIZE---------------
SSD1306_InitGPIO();
SSD1306_Init();
 // Screen orientation normal (FPC cable at the bottom)
SSD1306_Orientation(LCD_ORIENT_NORMAL);
// Mid level contrast
SSD1306_Contrast(127);
//Clear video buffer
SSD1306_Fill(0x00);
// Drawing mode: set pixels
LCD_PixelMode = LCD_PSET;
LCD_PutStr(28,11,"Rasana Mehr",fnt7x10,( uint8_t *)fdata);
SSD1306_Flush();
//LCD_PutStr(19,43,"OLED 128x64",fnt7x10);
//---------------------------------------------

 delay_ms(1000);
LogInit();
DeviceInitialized=true;
 while(1){
 
// if(!DS3231_ReadDate(&CurrentDate))
//{
//
//send_string("rtc error\n");
//
//
//}
CheckKeyPad();
 DisplayValues(1000);
delay_ms(1);

 }//end while
	}//end main
