#include <stm32f10x_lib.h>
#include <stdio.h>
#include <string.h>
///#include <stdbool.h>

#include "stm32_init.h"
#include "serial//serial.h"
#include "delay//delay.h"
#include "SystemInit/SystemInit.h"
#include "SSD1306//SSD1306.h" 
#include "SSD1306//font7X10.h"
#include "SSD1306//font5X7.h"
#include "eeprom//memory.h"
#include "ds3231//ds3231.h"


// Tile bitmap
static const uint8_t bmp_tile[] = {
		0x38,0x44,0x82,0x29,0x11,0x29,0x82,0x44,0x38,0x44,0x82,0x11,0x29,0x11,
		0x82,0x44,0x38,0x44,0x82,0x11,0x29,0x11,0x82,0x44,0x38,0x44,0x82,0x29,
		0x11,0x29,0x82,0x44,0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x00,0x00,0x00,
		0x00,0x01,0x01,0x01,0x00,0x00
};
//----------------------------------------------------------------------------------
char temp_main[60];
int i;
HRF_date_TypeDef CurrentDate;

//uint8_t LCD_PixelMode = LCD_PSET;
//=========================================================================================================================
int main(){
//float t,h;
stm32_Init();
jtag_Disable();
RGBInit();
send_string("hello\n");
SSD1306_InitGPIO();
//DS3231Init();


	// Initialize display
	SSD1306_InitGPIO();
	SSD1306_Init();

	// Screen orientation normal (FPC cable at the bottom)
	SSD1306_Orientation(LCD_ORIENT_NORMAL);

	// Mid level contrast
	SSD1306_Contrast(127);


	// Now do some drawing

	// Clear video buffer
	SSD1306_Fill(0x00);

	// Drawing mode: set pixels
//	LCD_PixelMode = LCD_PSET;

	// Draw a couple of strings
	//LCD_PutStr(35,11,"SSD1306",fnt7x10,( uint8_t *)fdata);
	//LCD_PutStr(19,43,"OLED 128x64",fnt7x10);
	//	LCD_HLine(0,60,60);
		// Send video buffer to the screen


		// Draw tiled bitmap
	for (i = 0; i < scr_width - 1; i += 16)	LCD_DrawBitmap(i,23,16,17,bmp_tile);

	SSD1306_Flush();
// Configure display to scroll horizontally right
	SSD1306_ScrollHSetup(LCD_SCROLL_RIGHT,2,5,LCD_SCROLL_IF25);

	// Start hardware display scrolling
	SSD1306_ScrollStart();

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
I2C_Configuration();
send_string("itc init ok\n");
//I2C_EE_ByteWrite(0x01,175);
//I2C_EE_ByteWrite(0x02,178);
send_string("i2c write ok\n");
	 
while(1){
//	DS3231_ReadDate(&CurrentDate);
//	sprintf(temp_main,"%d %d %d\n",CurrentDate.Hours,CurrentDate.Minutes,CurrentDate.Seconds) ;
//	delay_ms(300);
//	send_string(temp_main);

		sprintf(temp_main,"D0=%d D1=%d\n",I2C_EE_ByteRead(1),I2C_EE_ByteRead(2)) ;
	//SetRGB(0,1,0);
	delay_ms(300);
	send_string(temp_main);

		



		}//end while
	}//end main
