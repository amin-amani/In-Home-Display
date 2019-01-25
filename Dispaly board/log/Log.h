#ifndef __LOG_H
#define __LOG_H
#define ENERGY_RECORD_MAX_ADDRESS 4032

#include "..//ds3231//ds3231.h"
#include "..//crc//lib_crc.h"
#include "..//serial//serial.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
//=======================================================================================================
#pragma pack(push, 1)
typedef struct {
uint8_t		Year;
uint8_t		Mon;
uint8_t		Day;
uint8_t		Hour;
uint8_t		Min;
uint32_t	Energy;
uint8_t 	Temp;
uint16_t	Crc;
} EnergyRecord;


//=======================================================================================================
void LogInit(void);
//=======================================================================================================
bool EraseEnergyRecords(void);
//=======================================================================================================
bool ReadEnergyRecord(int recordNum ,EnergyRecord *record);
//=======================================================================================================
bool WriteEnergyRecord(int recordNum ,EnergyRecord record);
//=======================================================================================================
void EraseAll(void);

#endif
