#include "commandline.h"
#include "..//delay//delay.h"
#include "..//serial//serial.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char*p=0;


 //===============================================================================================================

static const SCMD tskcmd[] = 
{
	"Ping",Ping,
	"GetInfo",GetInfo,
	"Online",GetOnlineParameters,
	"GetDate",GetDate,
	"SetDate",SetDate,
	"GetTime",GetTime,
	"SetTime",SetTime,
	"GetPeackTime",GetPeackTime,
	"SetPeackTime",SetPeakTime,
	"GetConsumePattern",GetConsumePattern,
	"SetConsumePattern",SetConsumePattern,
	"GetLedBarLevels",GetLedBarLevels,
	"SetLedBarLevels",SetLedBarLevels,	
	"ReadRecord",ReadRecord,
	"ReadMemory",ReadMemory,
	"Erase",EraseMemory
 };
 //===============================================================================================================
int  start_with(char*str,const char*main_str)
{
	int len=0,i,n=0;
	len=strlen(main_str);
	for(i=0;i<len;i++)
	{
		if(str[i]==main_str[i]){n++;}
	}
	if(n==len)return 0;
	return -1;
}
//===============================================================================================================
void CommandlineHandle( char*buffer)
{			  
int i,command_array_size;
char com[50];
char arg[50];

clear_array(com,50);
clear_array(arg,50);
p=0;
command_array_size=(sizeof(tskcmd)/sizeof(SCMD));

sscanf(buffer,"%s %s",com,arg);
p=strstr(buffer,arg);
strcpy(arg,p);

////........................................
for(i=0;i<command_array_size;i++)
{
if(start_with(com,tskcmd[i].val)==0){tskcmd[i].func(arg);break;}
}
strcpy(com,"");
strcpy(arg,""); 
//free(p);
}
//===============================================================================================================
