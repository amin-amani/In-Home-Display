#include <stm32f10x_lib.h>
#include "..//stm32_init.h"
#define KeyLockThreshold 1
#define AnyKeyPressed ((GPIOA->IDR&0x1ff)!=0x1ff)
//#define ModeButton GPIOA->IDR&(1<<3)>>3
#define DownButton  !((GPIOA->IDR&(1<<11))>>11) //ok
#define MinusButton !((GPIOA->IDR&(1<<2))>>2) //ok
#define SetButton   !((GPIOA->IDR&(1<<3))>>3) //hit down!
#define PlusButton  !((GPIOA->IDR&(1<<4))>>4)  //ok
#define UPButton    !((GPIOA->IDR&(1<<8))>>8)
#define StopButton  !((GPIOA->IDR&(1<<6))>>6)
#define StartButton !((GPIOA->IDR&(1<<7))>>7)

#define BuzzerOn GPIOB->ODR|=(1<<13)
#define BuzzerOff GPIOB->ODR&=~(1<<13)
//===================================================================================
void Tim3Init(void);
void Tim4Init(void);
void Tim2Init(void);
void Beep(int delay);
void RGBInit(void);
void SetRGB(char r,char g,char b);
