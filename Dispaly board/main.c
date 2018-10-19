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
//#include "ssd1306//ssd1306.h"
#include "eeprom//memory.h"



unsigned  char r=0,g=0,b=0;
int step=0;
int i;
// Tile bitmap
static const uint8_t bmp_tile[] = {
		0x38,0x44,0x82,0x29,0x11,0x29,0x82,0x44,0x38,0x44,0x82,0x11,0x29,0x11,
		0x82,0x44,0x38,0x44,0x82,0x11,0x29,0x11,0x82,0x44,0x38,0x44,0x82,0x29,
		0x11,0x29,0x82,0x44,0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x00,0x00,0x00,
		0x00,0x01,0x01,0x01,0x00,0x00
};
//----------------------------------------------------------------------------------
char temp_main[100];
//uint8_t LCD_PixelMode = LCD_PSET;
//=========================================================================================================================
int main(){
//float t,h;
stm32_Init();
jtag_Disable();
RGBInit();
send_string("hello\n");
SSD1306_InitGPIO();


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
delay_ms(1000);

while(1){
		//delay_ms(300);
	//	send_string("salam\n");
		 SetRGBColor(r,g,b,50);
		 if(i++>15000){
		 if(step==0)b++;
		 if(b==255 && step==0){step++;}
		 if(step==1)b--;
		 if(b==0 && step==1){step++;}
		 if(step==2)r++;
		 if(r==255 && step==2 ){step++;}
		 if(step==3)r--;
		 if(r==0 && step==3){step++;}
		 if(step==4)g++;
		 if(g==255 && step==4 ){step++;}
		 if(step==5){g--;}
		  if(g==0 && step==5){step++;}
		  if(step==6){r++;}
		  if(r==255 && step==6){step++;}
		  if(step==7){g++;}
		  if(g==255 && step==7 ){step++;}
		  if(step==8){r--;}
		  if(r==0 && step==8 ){step++;}
		   if(step==9){b++;}
		  if(b==255 && step==9 ){step++;}
		   if(step==10){r++;}
		  if(r==255 && step==10 ){step++;}
		   if(step==11){g--;}
		  if(g==0 && step==11 ){step=0;r=0;g=0;b=0;}



		  





			


		i=0; 
		 }
		 	
		



		}//end while
	}//end main
