/*----------------------------------------------------------------------------
 *      R T L   F l a s h   F i l e   S y s t e m   E x a m p l e
 *----------------------------------------------------------------------------
 *      Name:    SERIAL.C
 *      Purpose: Serial Input Output for STmicroelectronics STM32
 *      Rev.:    V3.20
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2008 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <stm32f10x_lib.h>              /* STM32F10x Library Definitions     */
 #include "stm32f10x_type.h"
 #include <string.h>
 #define UDIR_BASE ((u32)0x1FFFF7E8)
 #define __IWDG_PERIOD             0x001E8480

#if   (__IWDG_PERIOD >  16384000UL)
  #define __IWDG_PR             (6)
  #define __IWDGCLOCK (32000UL/256)
#elif (__IWDG_PERIOD >   8192000UL)
  #define __IWDG_PR             (5)
  #define __IWDGCLOCK (32000UL/128)
#elif (__IWDG_PERIOD >   4096000UL)
  #define __IWDG_PR             (4)
  #define __IWDGCLOCK  (32000UL/64)
#elif (__IWDG_PERIOD >   2048000UL)
  #define __IWDG_PR             (3)
  #define __IWDGCLOCK  (32000UL/32)
#elif (__IWDG_PERIOD >   1024000UL)
  #define __IWDG_PR             (2)
  #define __IWDGCLOCK  (32000UL/16)
#elif (__IWDG_PERIOD >    512000UL)
  #define __IWDG_PR             (1)
  #define __IWDGCLOCK   (32000UL/8)
#else
  #define __IWDG_PR             (0)
  #define __IWDGCLOCK   (32000UL/4)
#endif
#define __IWGDCLK  (32000UL/(0x04<<__IWDG_PR))
#define __IWDG_RLR (__IWDG_PERIOD*__IWGDCLK/1000000UL-1)


typedef	struct
{
	vu16 udir0;
	vu16 udir1;
	vu32 udir2;
	vu32 udir3;	
}UDIR_TYPE;
#define UDIR		   ((UDIR_TYPE *)UDIR_BASE)

 GetUniqID(char *value)
{
		 char ch;
		 ch=UDIR->udir0;
		 value[0]=ch;
		 ch=((UDIR->udir0)>>8);
		 value[1]=ch;
		 ch=UDIR->udir1;
		 value[2]=ch;
		 ch=((UDIR->udir1)>>8);
		 value[3]=ch;
		 ch=UDIR->udir2;
		 value[4]=ch;
		 ch=((UDIR->udir2)>>8);
		 value[5]=ch;
		 ch=((UDIR->udir2)>>16);
		 value[6]=ch;
		 ch=((UDIR->udir2)>>24);
		 value[7]=ch;
		 ch=UDIR->udir3;
		 value[8]=ch;
		 ch=((UDIR->udir3)>>8);
		 value[9]=ch;
		 ch=((UDIR->udir3)>>16);
		 value[10]=ch;
		 ch=((UDIR->udir3)>>24);
		 value[11]=ch;
	 
}
  char Encrypt(char*id,char*key){
  int i;
  			  
		  for(i=0;i<12;i++){
		  
		 if(key[i]!=(id[i]^(i+1))) {return 0;}
		  
		  }

		  return 1;
		  

}

/*----------------------------------------------------------------------------
 *       disable jtag
 *---------------------------------------------------------------------------*/

void jtag_Disable(void)
{
		RCC->APB2ENR |=1;
		AFIO->MAPR &= ~((long int)(3<<24));
 		AFIO->MAPR |= (long int)(1<<26);
}
/*----------------------------------------------------------------------------
 *       sendchar:  Write a character to Serial Port
 *---------------------------------------------------------------------------*/

void ltoa(long int n,char *str)
{
unsigned long i;
unsigned char j,p;
i=1000000000L;
p=0;
if (n<0)
   {
   n=-n;
   *str++='-';
   };
do
  {
  j=(unsigned char) (n/i);
  if (j || p || (i==1))
     {
     *str++=j+'0';
     p=1;
     }
  n%=i;
  i/=10L;
  }
while (i!=0);
*str=0;
}


int sendchar (int ch) {
   if (ch == '\n')  {
      while (!(USART1->SR & USART_FLAG_TXE));
      USART1->DR = '\r';
   }
   while (!(USART1->SR & USART_FLAG_TXE));
   return (USART1->DR = (ch & 0x0FF));
}

/*----------------------------------------------------------------------------
 *       getkey:  Read a character from Serial Port
 *---------------------------------------------------------------------------*/
int getkey (void) {
  while (!(USART1->SR & USART_FLAG_RXNE));
  return ((int)(USART1->DR & 0x1FF));
}

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
 int sendchar2 (int ch) {
   if (ch == '\n')  {
      while (!(USART2->SR & USART_FLAG_TXE));
      USART2->DR = '\r';
   }
   while (!(USART2->SR & USART_FLAG_TXE));
   return (USART2->DR = (ch & 0x0FF));
}

/*----------------------------------------------------------------------------
 *       getkey:  Read a character from Serial Port
 *---------------------------------------------------------------------------*/
int getkey2 (void) {
  while (!(USART2->SR & USART_FLAG_RXNE));
  return ((int)(USART2->DR & 0x1FF));
}
 int getkey3 (void) {
  while (!(USART3->SR & USART_FLAG_RXNE));
  return ((int)(USART3->DR & 0x1FF));
}




  void send_static_string(const char *str)
{
	char ch;
	
	while(*str!=0)
	{
		ch=*str++;
		sendchar(ch);
	}
}
void send_string( char *str)
{
	char ch;
	
	while(*str!=0)
	{
		ch=*str++;
		sendchar(ch);
	}
}
  void send_static_string2(const char *str)
{
	char ch;
	
	while(*str!=0)
	{
		ch=*str++;
		sendchar2(ch);
	}
}
int sendbyte (unsigned char ch) {
   
   while (!(USART1->SR & USART_FLAG_TXE));
   return (USART1->DR = (ch & 0x0FF));
}

void send_bytes( unsigned char *str,int cnt)
{
unsigned char ch;
	int i;
	
	for(i=0;i<cnt;i++)
	{
		ch=*str++;
		sendbyte(ch);
	}
}

  void send_string2(char *str)
{
	char ch;
	
	while(*str!=0)
	{
		ch=*str++;
		sendchar2(ch);
	}
}
char get_udir(char *value)
{
		 char st[10],ch,i,ud[15],total[100];
		 
		 ch=UDIR->udir0;
		 ud[0]=ch;
		 
		 ch=((UDIR->udir0)>>8);
		 ud[1]=ch;
		 
		 ch=UDIR->udir1;
		 ud[2]=ch;
		 
		 ch=((UDIR->udir1)>>8);
		 ud[3]=ch;
		 
		 ch=UDIR->udir2;
		 ud[4]=ch;
		
		 ch=((UDIR->udir2)>>8);
		 ud[5]=ch;
		 
		 ch=((UDIR->udir2)>>16);
		 ud[6]=ch;
		 
		 ch=((UDIR->udir2)>>24);
		 ud[7]=ch;
		
		 ch=UDIR->udir3;
		 ud[8]=ch;
		 
		 ch=((UDIR->udir3)>>8);
		 ud[9]=ch;
		 
		 ch=((UDIR->udir3)>>16);
		 ud[10]=ch;
		 
		 ch=((UDIR->udir3)>>24);
		 ud[11]=ch;
		 
		 ud[12]=0x00;
		 
		 
		 	send_static_string("\nUnique device ID :");
		 	for(i=0;i<12;i++)
			{
				ltoa(ud[i],st);
				send_string(st);
				strcat(total,st);
				
				if(i<11){send_static_string(",");strcat(total,",");}
			}
			//send_string(total);
			send_static_string("\n->");
		 

		   if(strcmp(total,value)==0)return 1;
		    return 0 ;
		 		 
}
/* 
 void watchdog_init(){
 
		IWDG->KR  = 0x5555;                                           // enable write to PR, RLR
  		IWDG->PR  = __IWDG_PR;                                        // Init prescaler
  		IWDG->RLR = __IWDG_RLR;                                       // Init RLR
  		IWDG->KR  = 0xAAAA;                                           // Reload the watchdog  
  		IWDG->KR  = 0xCCCC;																	  
		
 
 
 }
 void watchdog_reset(){
 IWDG->KR  = 0xAAAA;
 } */
void clear_array(char*array,int size){
int i=0;
for(i=0;i<size;i++)array[i]=0;
}
