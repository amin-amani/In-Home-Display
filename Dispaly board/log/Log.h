#ifndef __LOG_H
#define __LOG_H
#define ENERGY_RECORD_MAX_ADDRESS 4032

typedef struct {
	uint8_t Year;
} EnergyRecord;
void Init();
void SaveEnergyRecord();
void EraseAll();
#endif
