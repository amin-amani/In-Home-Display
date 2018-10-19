#include "ds3231.h"
//=========================================================================================================================
char DS3231_ReadDateRAW(DS3231_date_TypeDef* date) {
	unsigned int i;
	unsigned long int counter,countervalue=20000;
	unsigned char buffer[7];
	
	I2C_AcknowledgeConfig(I2C1,ENABLE); // Enable I2C acknowledge

	I2C_GenerateSTART(I2C1,ENABLE); // Send START condition
	counter=countervalue;
	while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)){if((counter--)==0)return 1;} // Wait for EV5

	I2C_Send7bitAddress(I2C1,DS3231_addr,I2C_Direction_Transmitter); // Send DS3231 slave address
	counter=countervalue;
	while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){if((counter--)==0)return 2;} // Wait for EV6

	I2C_SendData(I2C1,DS3231_seconds); // Send DS3231 seconds register address
	counter=countervalue;
	while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)){if((counter--)==0)return 3;} // Wait for EV8

	I2C_GenerateSTART(I2C1,ENABLE); // Send repeated START condition (aka Re-START)
	counter=countervalue;
	while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)){if((counter--)==0)return 4;} // Wait for EV5

	I2C_Send7bitAddress(I2C1,DS3231_addr,I2C_Direction_Receiver); // Send DS3231 slave address for READ
	counter=countervalue;
	while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)){if((counter--)==0)return 5;} // Wait for EV6

	for (i = 0; i < 6; i++) {
	    counter=countervalue;
		while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED)){if((counter--)==0)return 6;} // Wait for EV7 (Byte received from slave)
		buffer[i] = I2C_ReceiveData(I2C1); // Receive byte
	}

	I2C_AcknowledgeConfig(I2C1,DISABLE); // Disable I2C acknowledgement
	I2C_GenerateSTOP(I2C1,ENABLE); // Send STOP condition
    counter=countervalue; 
	while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED)){if((counter--)==0)return 7;} // Wait for EV7 (Byte received from slave)
	buffer[i] = I2C_ReceiveData(I2C1); // Receive last byte

	memcpy(date,&buffer[0],7);
	return 0;
}
char DS3231_WriteDateRAW(DS3231_date_TypeDef* date) {
	unsigned int i;
	char buffer[7];
	unsigned long int counter,countervalue=20000; 
	memcpy(&buffer[0],date,7);

	I2C_AcknowledgeConfig(I2C1,ENABLE); // Enable I2C acknowledge

	I2C_GenerateSTART(I2C1,ENABLE); // Send START condition
	counter=countervalue;
	while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)){if((counter--)==0)return 1;} // Wait for EV5

	I2C_Send7bitAddress(I2C1,DS3231_addr,I2C_Direction_Transmitter); // Send DS3231 slave address
	counter=countervalue;
	while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){if((counter--)==0)return 2;} // Wait for EV6

	I2C_SendData(I2C1,DS3231_seconds); // Send DS3231 seconds register address
	counter=countervalue;
	while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)){if((counter--)==0)return 3;} // Wait for EV8

	for (i = 0; i < 7; i++) {
		I2C_SendData(I2C1,buffer[i]); // Send DS3231 seconds register address
		counter=countervalue;
		while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)){if((counter--)==0)return 4;} // Wait for EV8
	}

	I2C_GenerateSTOP(I2C1,ENABLE);
	return 0;
}

void DS3231_ReadDate(HRF_date_TypeDef* hrf_date) {
	DS3231_date_TypeDef raw_date;

	DS3231_ReadDateRAW(&raw_date);

	hrf_date->Seconds = (raw_date.seconds >> 4) * 10 + (raw_date.seconds & 0x0f);
	hrf_date->Minutes = (raw_date.minutes >> 4) * 10 + (raw_date.minutes & 0x0f);
	hrf_date->Hours   = (raw_date.hours   >> 4) * 10 + (raw_date.hours   & 0x0f);
	hrf_date->Day     = (raw_date.date    >> 4) * 10 + (raw_date.date    & 0x0f);
	hrf_date->Month   = (raw_date.month   >> 4) * 10 + (raw_date.month   & 0x0f);
	hrf_date->Year    = (raw_date.year    >> 4) * 10 + (raw_date.year    & 0x0f) + 2000;
	hrf_date->DOW     = raw_date.day_of_week;
}

void DS3231_DateToTimeStr(DS3231_date_TypeDef* raw_date, char *str) {
	*str++ = (raw_date->hours >> 4)     + '0';
    *str++ = (raw_date->hours & 0x0f)   + '0';
    *str++ = ':';
    *str++ = (raw_date->minutes >> 4)   + '0';
    *str++ = (raw_date->minutes & 0x0f) + '0';
    *str++ = ':';
    *str++ = (raw_date->seconds >> 4)   + '0';
    *str++ = (raw_date->seconds & 0x0f) + '0';
    *str++ = 0;
}

void DS3231_DateToDateStr(DS3231_date_TypeDef* raw_date, char *str) {
	*str++ = (raw_date->date >> 4)   + '0';
    *str++ = (raw_date->date & 0x0f) + '0';
    *str++ = '.';
    *str++ = (raw_date->month >> 4)   + '0';
    *str++ = (raw_date->month & 0x0f) + '0';
    *str++ = '.';
    *str++ = '2'; *str++ = '0';
    *str++ = (raw_date->year >> 4)   + '0';
    *str++ = (raw_date->year & 0x0f) + '0';
    *str++ = 0;
}

uint8_t DS3231_ReadTemp(void) {
	uint8_t temperature ;
	unsigned long int counter,countervalue=20000; 
	I2C_AcknowledgeConfig(I2C1,ENABLE); // Enable I2C acknowledge

	I2C_GenerateSTART(I2C1,ENABLE); // Send START condition
	counter=countervalue;
	while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)){if((counter--)==0)return 1;} // Wait for EV5

	I2C_Send7bitAddress(I2C1,DS3231_addr,I2C_Direction_Transmitter); // Send DS3231 slave address
	counter=countervalue;
	while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){if((counter--)==0)return 2;} // Wait for EV6

	I2C_SendData(I2C1,DS3231_tmp_MSB); // Send DS3231 temperature MSB register address
	counter=countervalue;
	while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)){if((counter--)==0)return 3;} // Wait for EV8

	I2C_GenerateSTART(I2C1,ENABLE); // Send repeated START condition (aka Re-START)
	counter=countervalue;
	while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)){if((counter--)==0)return 4;} // Wait for EV5

	I2C_Send7bitAddress(I2C1,DS3231_addr,I2C_Direction_Receiver); // Send DS3231 slave address for READ
	counter=countervalue;
	while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)){if((counter--)==0)return 5;} // Wait for EV6

	counter=countervalue;
	while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED)){if((counter--)==0)return 6;} // Wait for EV7 (Byte received from slave)
	 temperature = I2C_ReceiveData(I2C1); // Receive temperature MSB

	I2C_AcknowledgeConfig(I2C1,DISABLE); // Disable I2C acknowledgement

	I2C_GenerateSTOP(I2C1,ENABLE); // Send STOP condition
	counter=countervalue;
	while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED)){if((counter--)==0)return 7;} // Wait for EV7 (Byte received from slave)

	return temperature;
}

 //=========================================================================
void i2c_init(){
   I2C_InitTypeDef  I2C_InitStructure; 
  GPIO_InitTypeDef  GPIO_InitStructure; 

  /* Configure I2C1 pins: SCL and SDA */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
  /* I2C configuration */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = I2C1_SLAVE_ADDRESS7;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;
  
  /* I2C Peripheral Enable */
  I2C_Cmd(I2C1, ENABLE);
  /* Apply I2C configuration after enabling it */
  I2C_Init(I2C1, &I2C_InitStructure);

}
void DS3231Init(){

  

   GPIO_InitTypeDef  GPIO_InitStructure; 
   	I2C_InitTypeDef I2CInit;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD; // PP onboard?
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);


	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE); // Enable I2C clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

	I2C_DeInit(I2C1); // I2C reset to initial state
	I2CInit.I2C_Mode = I2C_Mode_I2C; // I2C mode is I2C ^_^
	I2CInit.I2C_DutyCycle = I2C_DutyCycle_2; // I2C fast mode duty cycle (WTF is this?)
	I2CInit.I2C_OwnAddress1 = 1; // This device address (7-bit or 10-bit)
	I2CInit.I2C_Ack = I2C_Ack_Enable; // Acknowledgement enable
	I2CInit.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; // choose 7-bit address for acknowledgement
	I2CInit.I2C_ClockSpeed = 400000; // 400kHz ?
	I2C_Cmd(I2C1,ENABLE); // Enable I2C
	I2C_Init(I2C1,&I2CInit); // Configure I2C
	   //send_string("Wait until I2C free...");
	while (I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY)); // Wait until I2C free
//   send_string(" ok\n");
	// Check connection to DS3231
	I2C_GenerateSTART(I2C1,ENABLE); // Send START condition
	while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)); // Wait for EV5
	I2C_Send7bitAddress(I2C1,DS3231_addr,I2C_Direction_Transmitter); // Send DS3231 slave address
	while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)); // Wait for EV6
	I2C_GenerateSTOP(I2C1,ENABLE);

	// Wait for 250ms for DS3231 startup
	delay_ms(300);

	// DS3231 init
	I2C_GenerateSTART(I2C1,ENABLE);
	while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)); // Wait for EV5
	I2C_Send7bitAddress(I2C1,DS3231_addr,I2C_Direction_Transmitter); // Send DS3231 slave address
	while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)); // Wait for EV6
	I2C_SendData(I2C1,DS3231_control); // Send DS3231 control register address
	while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)); // Wait for EV8
	I2C_SendData(I2C1,0x00); // DS3231 EOSC enabled, INTCN enabled, SQW set to 1Hz
	while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)); // Wait for EV8
	I2C_SendData(I2C1,0x00); // DS3231 clear alarm flags
	while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)); // Wait for EV8
	I2C_GenerateSTOP(I2C1,ENABLE);

}
