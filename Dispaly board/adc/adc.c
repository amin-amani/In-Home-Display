#include <stm32f10x_lib.h>

void adc_init(int number_of_channels) {
// int i;
	ADC_InitTypeDef ADC_InitStructure; 
 /* PCLK2 is the APB2 clock */ 
 /* ADCCLK = PCLK2/6 = 72/6 = 12MHz*/ 
 RCC_ADCCLKConfig(RCC_PCLK2_Div6); 
 /* Enable ADC1 clock so that we can talk to it */ 
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 
 /* Put everything back to power-on defaults */ 
 
 ADC_DeInit(ADC1); /* ADC1 Configuration ------------------------------------------------------*/
 /* ADC1 and ADC2 operate independently */ 
 ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; 
 /* Disable the scan conversion so we do one at a time */
 ADC_InitStructure.ADC_ScanConvMode = DISABLE;
 /* Don't do contimuous conversions - do them on demand */
 ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; 
 /* Start conversin by software, not an external trigger */ 
 ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
 /* Conversions are 12 bit - put them in the lower 12 bits of the result */
 ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 
 /* Say how many channels would be used by the sequencer */
 ADC_InitStructure.ADC_NbrOfChannel = number_of_channels; 
 /* Now do the setup */
 ADC_Init(ADC1, &ADC_InitStructure);
 
 //常规转换序列1：通道10
 ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_13Cycles5);
 //常规转换序列2：通道16（内部温度传感器），采样时间>2.2us,(239cycles)
 ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_239Cycles5);
//常规转换序列2：通道16（内部温度传感器），采样时间>2.2us,(239cycles)
 ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_239Cycles5);
//常规转换序列2：通道16（内部温度传感器），采样时间>2.2us,(239cycles)
 ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_239Cycles5);
//常规转换序列1：通道10
 ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 5, ADC_SampleTime_13Cycles5);
 //常规转换序列2：通道16（内部温度传感器），采样时间>2.2us,(239cycles)
 ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 6, ADC_SampleTime_239Cycles5);
//常规转换序列2：通道16（内部温度传感器），采样时间>2.2us,(239cycles)
 ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 7, ADC_SampleTime_239Cycles5);
//常规转换序列2：通道16（内部温度传感器），采样时间>2.2us,(239cycles)
 ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 8, ADC_SampleTime_239Cycles5);
//常规转换序列1：通道10
 ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 9, ADC_SampleTime_13Cycles5);
 //常规转换序列2：通道16（内部温度传感器），采样时间>2.2us,(239cycles)
 ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 10, ADC_SampleTime_239Cycles5);
//常规转换序列2：通道16（内部温度传感器），采样时间>2.2us,(239cycles)
 ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 11, ADC_SampleTime_239Cycles5);
//常规转换序列2：通道16（内部温度传感器），采样时间>2.2us,(239cycles)
 ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 12, ADC_SampleTime_239Cycles5);
//常规转换序列1：通道10
 ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 13, ADC_SampleTime_13Cycles5);
 //常规转换序列2：通道16（内部温度传感器），采样时间>2.2us,(239cycles)
 ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 14, ADC_SampleTime_239Cycles5);
//常规转换序列2：通道16（内部温度传感器），采样时间>2.2us,(239cycles)
 ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 15, ADC_SampleTime_239Cycles5);
//常规转换序列2：通道16（内部温度传感器），采样时间>2.2us,(239cycles)
 ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 16, ADC_SampleTime_239Cycles5);

//for(i=0;i<number_of_channels;i++){
//ADC_RegularChannelConfig(ADC1, i, i+1, ADC_SampleTime_239Cycles5);	
	//	                             }
/* Enable ADC1 */
 ADC_Cmd(ADC1, ENABLE); 
 /* Enable ADC1 reset calibaration register */ 
 ADC_ResetCalibration(ADC1);
 
/* Check the end of ADC1 reset calibration register */

while(ADC_GetResetCalibrationStatus(ADC1)); /* Start ADC1 calibaration */
 ADC_StartCalibration(ADC1); /* Check the end of ADC1 calibration */ 
 while(ADC_GetCalibrationStatus(ADC1));
 }
 
 
 u16 read_adc() { 
//	ADC_RegularChannelConfig(ADC1, channel, pos, ADC_SampleTime_1Cycles5);
	// Start the conversion
	ADC_SoftwareStartConvCmd(ADC1, ENABLE); 
	// Wait until conversion completion 
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET); 
	// Get the conversion value 
	 return 	ADC_GetConversionValue(ADC1); }

u16 readadc(int channel) { 
int i=0;
u16 adc,result=0;	
if(channel==15){channel=0;}
else{channel++;}

for(i=0;i<16;i++){
	adc=read_adc();
	if(channel==i){result=adc;}

	}
	
return result;
	}

