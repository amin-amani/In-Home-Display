#include <stm32f10x_lib.h>
#include "..//stm32_init.h"
#define KeyLockThreshold 4000
#define AnyKeyPressed ((GPIOA->IDR&0x1ff)!=0x1ff)
#define SelectKEY ((GPIOA->IDR>>8)&0x01)
//#define ModeButton GPIOA->IDR&(1<<3)>>3
#define DownButton  !((GPIOA->IDR&(1<<11))>>11) //ok
#define MinusButton !((GPIOA->IDR&(1<<12))>>12) //ok
#define UpButton   !((GPIOA->IDR&(1<<13))>>13) //hit down!
#define PlusButton  !((GPIOA->IDR&(1<<8))>>8)  //ok


#define BuzzerOn GPIOB->ODR|=(1<<13)
#define BuzzerOff GPIOB->ODR&=~(1<<13)
//===================================================================================
void Tim3Init(void);
void Tim4Init(void);
void Tim2Init(void);
void Beep(int delay);
void RGBInit(void);
void SetRGB(char r,char g,char b);
