
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f0xx_hal.h"
#include "math.h"
#define VRef 3.29;
#define resistantratio (1.5+10)/1.5;
#define tranceratio 230/22;//DC voltafe after Diod...

ADC_HandleTypeDef hadc;
ADC_ChannelConfTypeDef chanelcon;
TIM_HandleTypeDef htim14;
TIM_HandleTypeDef htim16;
UART_HandleTypeDef huart1;
char TempStr[40];
double CurrentADCOffset = 1930;
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM14_Init(void);
static void MX_TIM16_Init(void);
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
void send_string(char*);
void a2dconfig();
int readVoltage();
int readCurrent();
void send_digit_USART(long int );
void intrrupt();

double  voltage=0;
double TotalCurrentMesure=0;
int counter=0;
	double  max=-8000,min=8000;

void delay_us(int Delay){
unsigned long int i=0,j=0;
while(i++<Delay){
for(j=0;j< 3;j++);

} 
}


int readCurrent(){
	 float floatCurrent;
	 int   intCurrent;

	chanelcon.Channel=ADC_CHANNEL_0;

	chanelcon.Rank=2;

    HAL_ADC_DeInit(&hadc);

	HAL_ADC_Init(&hadc);

	HAL_ADC_ConfigChannel(&hadc,&chanelcon);

	HAL_ADC_Start(&hadc);

		if(HAL_ADC_PollForConversion(&hadc,1000000)==HAL_OK)
		{
	floatCurrent=HAL_ADC_GetValue(&hadc);
//	floatCurrent*=resistantratio;
	intCurrent=floatCurrent;
	return intCurrent;
	
		}

			HAL_ADC_Stop(&hadc);


 return 0;



}
int readVoltage(){
	 float floatVoltage;
	 int   intVoltage;

	chanelcon.Channel=ADC_CHANNEL_1;

	chanelcon.Rank=1;

    HAL_ADC_DeInit(&hadc);

	HAL_ADC_Init(&hadc);

	HAL_ADC_ConfigChannel(&hadc,&chanelcon);

	HAL_ADC_Start(&hadc);

		if(HAL_ADC_PollForConversion(&hadc,1000000)==HAL_OK)
		{
	floatVoltage=HAL_ADC_GetValue(&hadc);
	floatVoltage*=resistantratio;
    floatVoltage*=tranceratio;
	floatVoltage*=VRef;
	floatVoltage/=4096;
	intVoltage=floatVoltage;
	return intVoltage;//voltage coef
	
		}

			HAL_ADC_Stop(&hadc);


 return 0;
}
void send_string(char *buff){
		
	while(*buff!=0)
	{
USART1->TDR=(*buff++);
HAL_Delay(2);
	}

}


void Usart_interrupt(){

}
void send_serial(int data){
char digits[4],i;
for(i=0;i<4;i++){
digits[i]=data%10;
data/=10;		 }

for(i=4;i>0;i--){

USART1->TDR=digits[i-1]+48;
 HAL_Delay(5);
}



}

void intrrupt(){


}




void send_digit_USART(long int data){
int digits[10],i=0;
for(;data>0;data/=10){
digits[i]=data%10;
i++;
					  }
i--;

for(;i>=0;i--){USART1->TDR=digits[i]+48;HAL_Delay(1);}

}




void a2dconfig(){
  hadc.Instance = ADC1;											
  hadc.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc.Init.Resolution = ADC_RESOLUTION_12B;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_BACKWARD;
  hadc.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  hadc.Init.LowPowerAutoWait = DISABLE;
  hadc.Init.LowPowerAutoPowerOff = DISABLE;
  hadc.Init.ContinuousConvMode = ENABLE ;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.DMAContinuousRequests = DISABLE;
  hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
	HAL_ADC_Init(&hadc);
	chanelcon.SamplingTime=ADC_SAMPLETIME_239CYCLES_5;
	

		
}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI14;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.HSI14CalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLMUL=RCC_PLL_MUL12;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}


/* TIM14 init function */
static void MX_TIM14_Init(void)
{

  TIM_OC_InitTypeDef sConfigOC;

  htim14.Instance = TIM14;
  htim14.Init.Prescaler = 16;
  htim14.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim14.Init.Period = 1300;
  htim14.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim14.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim14) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_PWM_Init(&htim14) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 650;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim14, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  HAL_TIM_MspPostInit(&htim14);

}

/* TIM16 init function */
static void MX_TIM16_Init(void)
{

  TIM_OC_InitTypeDef sConfigOC;
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig;

  htim16.Instance = TIM16;
  htim16.Init.Prescaler = 0;
  htim16.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim16.Init.Period = 0xff;
  htim16.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim16.Init.RepetitionCounter = 26;
  htim16.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim16) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_OC_Init(&htim16) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigOC.OCMode = TIM_OCMODE_TIMING;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_OC_ConfigChannel(&htim16, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim16, &sBreakDeadTimeConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART1 init function */
static void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();


  
  /*Configure GPIO pin Output Level */
  //HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);

  /*Configure GPIO pin : PF0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

double CalcCurrent()
{
	float CurrentMesure=0,current=0;
double TempCurrent=0;
  max=-8000,min=8000;
	static double prev=0;

	for(int i=0;i<200;i++){
	TempCurrent=readCurrent();
		max=fmax(max,TempCurrent);
		min=fmin(min,TempCurrent);
	current +=fabs(TempCurrent-CurrentADCOffset);	
	delay_us(100);
		GPIOA->ODR^=(1<<13);
												}	


												

CurrentMesure=	current;											
CurrentMesure/=200;//sample count
CurrentMesure/=1241;//adc ratio
		CurrentMesure*=1.615;//resistor ratio
		CurrentMesure*=15.16;//sensor ratio		


	
												
												
			if( ( fabs((max+min)/2-CurrentADCOffset) )> 120 ){
		CurrentMesure=prev;
			
			}

				prev=CurrentMesure;
		return CurrentMesure;	

}
int main(void)
{	
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_USART1_UART_Init();
		a2dconfig();
//	MX_TIM14_Init();
//	MX_TIM16_Init();

//	HAL_TIM_MspPostInit(&htim14);
//	HAL_TIM_Base_Start(&htim14);
//HAL_TIM_PWM_Start(&htim14,TIM_CHANNEL_1);
//	HAL_TIM_Base_Start(&htim16);
//	HAL_TIM_MspPostInit(&htim16);
//	HAL_TIM_Base_Start_IT(&htim16);

	USART1->TDR=49; 
	  while (1){
			
					TotalCurrentMesure+=CalcCurrent();
		
			counter++;
					voltage=readVoltage();
			//	sprintf(TempStr,"V=%2.2f \n",voltage);			send_string(TempStr);
			if(voltage>230)voltage=230;
				if(voltage<205 && voltage>180)voltage=205;
			if(counter==10)
				{
					sprintf(TempStr,"%2.2f  %3.3f\n",(TotalCurrentMesure/10)*.9 ,voltage); //1.010265 is coeff
			send_string(TempStr);
				TotalCurrentMesure=0;
				counter=0;
				}

		
	
//send_string("v:");	
//send_digit_USART(voltage);
//send_string("\n");
//HAL_Delay(500);


	//for(int i=0;i<350;i++){
	//current=readCurrent();
//	if(currentMax<current)currentMax=current;
	//HAL_Delay(1);
		//										}
//			sprintf(TempStr,"%2.2f  %2.2f %2.2f %2.2f\n",CalcCurrent(),max,min,((max+min)/2)-CurrentADCOffset);
//			send_string(TempStr);

	
			
//			
//send_digit_USART(current/1241);
//send_string("\n");
    
  }
  /* USER CODE END 3 */

}