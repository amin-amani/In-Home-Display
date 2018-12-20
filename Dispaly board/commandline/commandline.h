/*
!!!!!!atttention!!!!
these array size must be same size as uart buffer
char com[50];
char arg[50];

clear_array(com,50);
clear_array(arg,50)
*/
#ifndef COMMANDLINE_H
#define COMMANDLINE_H
#include <stm32f10x_lib.h>


void CommandlineHandle( char*buffer);

extern void Ping(char*par);
extern void EraseMemory(char*par);
extern void GetInfo(char*par);
extern void GetOnlineParameters(char*par);
extern void GetDate(char*par);
extern void SetDate(char*par);
extern void GetTime(char*par);
extern void SetTime(char*par);
extern void GetPeackTime(char*par);
extern void SetPeakTime(char*par);
extern void GetConsumePattern(char*par);
extern void SetConsumePattern(char*par);
extern void GetLedBarLevels(char*par);
extern void SetLedBarLevels(char*par);
extern void ReadRecord(char*par);
extern void ReadMemory(char*par);

typedef struct scmd {
 char val[30];
 void (*func)(char *par);
} SCMD;

//===============================================================================================================

#endif
