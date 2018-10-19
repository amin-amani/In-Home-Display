#ifndef _DS3231_H
#define _DS3231_H
 #include <stm32f10x_lib.h>
#include <stdio.h>
#include <string.h>
#define I2C_Speed              100000
#define I2C1_SLAVE_ADDRESS7    0x00
#define I2C_PageSize           16

#define DS3231_addr     0xD0 // I2C 7-bit slave address shifted for 1 bit to the left
#define DS3231_seconds  0x00 // DS3231 seconds address
#define DS3231_control  0x0E // DS3231 control register address
#define DS3231_tmp_MSB  0x11 // DS3231 temperature MSB

 typedef char               int8_t;
typedef unsigned char      uint8_t;
typedef short              int16_t;
typedef unsigned short     uint16_t;
typedef int                int32_t;
typedef unsigned int       uint32_t;
// All DS3231 registers

typedef struct {
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
	uint8_t day;
	uint8_t date;
	uint8_t month;
	uint8_t year;
	uint8_t alarm1_secconds;
	uint8_t alarm1_minutes;
	uint8_t alarm1_hours;
	uint8_t alarm1_day;
	uint8_t alarm1_date;
	uint8_t alarm2_minutes;
	uint8_t alarm2_hours;
	uint8_t alarm2_day;
	uint8_t alarm2_date;
	uint8_t control;
	uint8_t status;
	uint8_t aging;
	uint8_t msb_temp;
	uint8_t lsb_temp;
} DS3231_registers_TypeDef;

// DS3231 date
typedef struct {
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
	uint8_t day_of_week;
	uint8_t date;
	uint8_t month;
	uint8_t year;
} DS3231_date_TypeDef;

// Human Readable Format date
typedef struct {
	uint8_t  Seconds;
	uint8_t  Minutes;
	uint8_t  Hours;
	uint8_t  Day;
	uint8_t  Month;
	uint16_t Year;
	uint8_t  DOW;
} HRF_date_TypeDef;



//=========================================================================================================================
char DS3231_ReadDateRAW(DS3231_date_TypeDef* date) ;
char DS3231_WriteDateRAW(DS3231_date_TypeDef* date);

void DS3231_ReadDate(HRF_date_TypeDef* hrf_date);
void DS3231_DateToTimeStr(DS3231_date_TypeDef* raw_date, char *str) ;
void DS3231_DateToDateStr(DS3231_date_TypeDef* raw_date, char *str) ;
uint8_t DS3231_ReadTemp(void);

 //=========================================================================
void i2c_init(void);
void DS3231Init(void);





#endif
