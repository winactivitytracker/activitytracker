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

#include "ssd1306.h"
#include "fonts.h"
#include "test.h"
#include "stdio.h"
#include "gps.h"
#include "usart.h"
#include <stdlib.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
extern GPS_t GPS;
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 64 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Gps */
osThreadId_t GpsHandle;
const osThreadAttr_t Gps_attributes = {
  .name = "Gps",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

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
    osDelay(1);
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

	    char numbers[5];
	  	char data [10];
	  	uint32_t mask;

		//HAL_UART_Receive_IT(&huart1, data, 10);

	  	SSD1306_GotoXY (0,0);
	  	SSD1306_Puts ("LAT:", &Font_7x10, 1);

	  	SSD1306_GotoXY (40, 0);
	  	sprintf(numbers, "%f", GPS.dec_latitude);
	  	SSD1306_Puts(numbers, &Font_7x10, 1);

	  	SSD1306_GotoXY (0,14);
	  	SSD1306_Puts ("LONG:", &Font_7x10, 1);

	  	SSD1306_GotoXY (40, 14);
	  	sprintf(numbers, "%f", GPS.dec_longitude);
		SSD1306_Puts(numbers, &Font_7x10, 1);

	  	SSD1306_GotoXY (0,28);
		SSD1306_Puts ("KMPH:", &Font_7x10, 1);

		SSD1306_GotoXY (40, 28);
		sprintf(numbers, "%f", GPS.speed_km);
		SSD1306_Puts(numbers, &Font_7x10, 1);

		SSD1306_GotoXY (0,42);
		SSD1306_Puts ("TIME:", &Font_7x10, 1);

		SSD1306_GotoXY (40, 42);
		sprintf(numbers, "%T", GPS.utc_time);
		SSD1306_Puts(numbers , &Font_7x10, 1);

	  	SSD1306_UpdateScreen();



	  	osDelay(500);
  }
  /* USER CODE END GpsTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
