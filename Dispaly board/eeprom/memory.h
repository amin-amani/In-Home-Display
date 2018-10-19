#ifndef _MEMORY_DEFINED
#define _MEMORY_DEFINED
#include <stm32f10x_type.h>
#include <stdbool.h>
#include "eeprom.h"
//=========================================================================================================

typedef struct {
int MillSpeed;
int MotorSpeed;
int FanSpeed;
int Time;
unsigned int crc;
}FlashMemory;
 //=========================================================================================================
extern FlashMemory *Memory;
extern u16 buffer[sizeof(FlashMemory)/2];
//=========================================================================================================
 u16 SaveMemory(void);
//=========================================================================================================
bool LoadMemory(void);
 #endif
