/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "user_diskio_spi.h"
#include "ssd1306.h"
#include "fonts.h"
#include "test.h"
#include "stdio.h"
#include "gps.h"
#include "usart.h"
#include "sdCard.h"
#include "adc.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define CALCULATEPERCENTAGE 0
#define CALCULATEVOLTAGE 1

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

extern uint16_t ADCValue;
extern GPS_t GPS;

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Gps */
osThreadId_t GpsHandle;
const osThreadAttr_t Gps_attributes = {
  .name = "Gps",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for ADCSemaphore */
osSemaphoreId_t ADCSemaphoreHandle;
const osSemaphoreAttr_t ADCSemaphore_attributes = {
  .name = "ADCSemaphore"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

void ADC_IRQHandler();
float calculateBattery(uint8_t whatCalculation);

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void GpsTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of ADCSemaphore */
  ADCSemaphoreHandle = osSemaphoreNew(1, 1, &ADCSemaphore_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of Gps */
  GpsHandle = osThreadNew(GpsTask, NULL, &Gps_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {

	  //SSD1306_Clear();
	  char numbers[16];
	  char battNumber[16];
	  float battVoltage = 0.0;

	  SSD1306_GotoXY (0,0);
	  SSD1306_Puts ("ADC:", &Font_7x10, 1);

	  sprintf(numbers, "%d", ADCValue);

	  SSD1306_GotoXY (50, 0);
	  SSD1306_Puts(numbers, &Font_7x10, 1);

	  battVoltage = calculateBattery(CALCULATEPERCENTAGE);
	  SSD1306_GotoXY (0,15);
	  SSD1306_Puts ("BattADC:", &Font_7x10, 1);
	  sprintf(battNumber, "%.0f", battVoltage);
	  SSD1306_GotoXY (50, 15);
	  SSD1306_Puts(battNumber, &Font_7x10, 1);

	  battVoltage = calculateBattery(CALCULATEVOLTAGE);
	  SSD1306_GotoXY (0,30);
	  SSD1306_Puts ("Batt:", &Font_7x10, 1);
	  sprintf(battNumber, "%.2f volt", battVoltage);
	  SSD1306_GotoXY (50, 30);
	  SSD1306_Puts(battNumber, &Font_7x10, 1);
	  SSD1306_UpdateScreen();

	  osDelay(500);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_GpsTask */
/**
* @brief Function implementing the Gps thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_GpsTask */
void GpsTask(void *argument)
{
  /* USER CODE BEGIN GpsTask */
  /* Infinite loop */
  for(;;)
  {
	  static uint16_t counter = 0;
	  char numbers[15];
	  sprintf(numbers, "%d", counter);
	  	  if(writeFile("CSVTest.csv", numbers))
	  	  {
	  		  writeFile("CSVTest.csv", ", ");
	  		  writeFile("CSVTest.csv", numbers);
	  		  writeFile("CSVTest.csv", ", ");
	  		  writeFile("CSVTest.csv", numbers);
	  		  writeFile("CSVTest.csv", ", ");
	  		  writeFile("CSVTest.csv", numbers);
	  		  writeFile("CSVTest.csv", ", ");
	  		  writeFile("CSVTest.csv", numbers);
	  		  writeFile("CSVTest.csv", "\n");
	  		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
	  		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
	  	  } else
	  	  {
	  		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
	  		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
	  	  }
	  	  counter++;
	  	osDelay(1000);
  }
  /* USER CODE END GpsTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

float calculateBattery(uint8_t whatCalculation)
{
	uint16_t localADC = ADCValue;	//grab the last conversion value
	HAL_ADC_Start_IT(&hadc1);		//start a new conversion already so it's ready for the next time the function is called
	if(whatCalculation == CALCULATEPERCENTAGE)
	{
		return 71.428571428571 * ((localADC / (float)4095) * 4.2) - 200;
		//return (localADC / (float)4095) * 100;
	} else if(whatCalculation == CALCULATEVOLTAGE)
	{
		return (localADC / (float)4095) * 4.2;
	}
	return -1.0;
}

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
