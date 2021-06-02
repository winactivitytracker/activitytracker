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

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "ssd1306.h"
#include "fonts.h"
#include "gps.h"
#include "usart.h"
#include "adc.h"
#include "sdCard.h"
#include "activity.h"
#include "radioAPI.h"

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
extern Activity_T CurrentActivity;
float batteryPer = 0, batteryVol = 0;

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 64 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for DrawOnOledTask */
osThreadId_t DrawOnOledTaskHandle;
const osThreadAttr_t DrawOnOledTask_attributes = {
  .name = "DrawOnOledTask",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for ActivityTask */
osThreadId_t ActivityTaskHandle;
const osThreadAttr_t ActivityTask_attributes = {
  .name = "ActivityTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for readBattery */
osThreadId_t readBatteryHandle;
const osThreadAttr_t readBattery_attributes = {
  .name = "readBattery",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for rSendTask */
osThreadId_t rSendTaskHandle;
const osThreadAttr_t rSendTask_attributes = {
  .name = "rSendTask",
  .stack_size = 64 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for rReceiveTask */
osThreadId_t rReceiveTaskHandle;
const osThreadAttr_t rReceiveTask_attributes = {
  .name = "rReceiveTask",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityRealtime,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

float calculateBattery(uint8_t whatCalculation);

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartDrawing(void *argument);
void StartActivityTask(void *argument);
void StartReadBattery(void *argument);
void StartRadioSendTask(void *argument);
void StartRadioReceiveTask(void *argument);

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

  /* creation of DrawOnOledTask */
  DrawOnOledTaskHandle = osThreadNew(StartDrawing, NULL, &DrawOnOledTask_attributes);

  /* creation of ActivityTask */
  ActivityTaskHandle = osThreadNew(StartActivityTask, NULL, &ActivityTask_attributes);

  /* creation of readBattery */
  readBatteryHandle = osThreadNew(StartReadBattery, NULL, &readBattery_attributes);

  /* creation of rSendTask */
  rSendTaskHandle = osThreadNew(StartRadioSendTask, NULL, &rSendTask_attributes);

  /* creation of rReceiveTask */
  rReceiveTaskHandle = osThreadNew(StartRadioReceiveTask, NULL, &rReceiveTask_attributes);

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

/* USER CODE BEGIN Header_StartDrawing */
/**
 * @brief Function implementing the DrawOnOledTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDrawing */
void StartDrawing(void *argument)
{
  /* USER CODE BEGIN StartDrawing */
	  /* Infinite loop */
	  for(;;)
	  {
		char numbers[10];
		char *currentTime;

		SSD1306_GotoXY (0,0);
		sprintf(numbers, "batt: %.0f", batteryPer);
		SSD1306_Puts (numbers, &Font_7x10, 1);

		SSD1306_GotoXY (80,0);
		SSD1306_Puts ("ADM:", &Font_7x10, 1);

		SSD1306_GotoXY (110, 0);
		sprintf(numbers, "%d", CurrentActivity.activeDailyMinutes);
		SSD1306_Puts(numbers, &Font_7x10, 1);

		SSD1306_GotoXY (0, 25);
		sprintf(numbers, "%.2f", GPS.speed_km);
		SSD1306_Puts(numbers, &Font_11x18, 1);
		SSD1306_GotoXY (50,31);
		SSD1306_Puts ("km/u", &Font_7x10, 1);

		SSD1306_GotoXY (0,10);
		//SSD1306_Puts(getActivity(), &Font_7x10, 1);
		SSD1306_Puts(activityToString(CurrentActivity.lastActiveMinute), &Font_7x10, 1);
		SSD1306_Puts("          ",&Font_7x10, 1);

		currentTime = getTime();
		SSD1306_GotoXY (0, 45);
		SSD1306_Puts(currentTime, &Font_7x10, 1);

		free(currentTime);

		SSD1306_GotoXY (80,10);
		SSD1306_Puts ("AL:", &Font_7x10, 1);

		SSD1306_GotoXY (110, 10);
		sprintf(numbers, "%d", CurrentActivity.length);
		SSD1306_Puts(numbers, &Font_7x10, 1);

		SSD1306_UpdateScreen();

	    osDelay(1000);
	  }
  /* USER CODE END StartDrawing */
}

/* USER CODE BEGIN Header_StartActivityTask */
/**
 * @brief Function implementing the ActivityTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartActivityTask */
void StartActivityTask(void *argument)
{
  /* USER CODE BEGIN StartActivityTask */

	  /* Infinite loop */
	  for(;;)
	  {
		  getActivity();

		  ActivityTotal();

		  osDelay(100);
	  }
  /* USER CODE END StartActivityTask */
}

/* USER CODE BEGIN Header_StartReadBattery */
/**
 * @brief Function implementing the readBattery thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartReadBattery */
void StartReadBattery(void *argument)
{
  /* USER CODE BEGIN StartReadBattery */
	/* Infinite loop */
	  for(;;)
	  {
		batteryPer = calculateBattery(CALCULATEPERCENTAGE);
		batteryVol = calculateBattery(CALCULATEVOLTAGE);
		if(batteryVol < 3.0)
		{
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, SET);
		}
		else
		{
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, RESET);
		}
	    osDelay(5000);
	  }
  /* USER CODE END StartReadBattery */
}

/* USER CODE BEGIN Header_StartRadioSendTask */
/**
* @brief Function implementing the rSendTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartRadioSendTask */
void StartRadioSendTask(void *argument)
{
  /* USER CODE BEGIN StartRadioSendTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartRadioSendTask */
}

/* USER CODE BEGIN Header_StartRadioReceiveTask */
/**
* @brief Function implementing the rReceiveTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartRadioReceiveTask */
void StartRadioReceiveTask(void *argument)
{
  /* USER CODE BEGIN StartRadioReceiveTask */

	// Start listening on the receiver
	receiverEnable();

	bool doAck = false;
	char incoming[100];

	for(;;)
	{
		doAck = false;

		// See if there are messages in the queue
		if(receiverCheckMessage())
		{
			// Get the first message from the queue
			receiverPopMessage(incoming);

			if(strncmp(incoming, "z,",2) == 0)
			{
				//// Read the contents of the file
				char z = '?';
				unsigned int id,hours,minutes,seconds;
				int gyroZ;
				id = hours = minutes = seconds = 0;
				gyroZ = -1;

				int rv = sscanf(incoming,
						"z,%u,%u,%u,%u,%d",
						&id,
						&hours,
						&minutes,
						&seconds,
						&gyroZ
				);

				RadioToBuffer(id, hours, minutes, seconds, gyroZ);


				doAck = true;
			}

			else if(strncmp(incoming, "a,", 2) == 0)
			{
				// Read accelero/gyro data
				int MPUData[6] = {0,0,0,0,0,0};

				sscanf(incoming,
						"a:%d,%d,%d,%d,%d,%d",
						&MPUData[0],
						&MPUData[1],
						&MPUData[2],
						&MPUData[3],
						&MPUData[4],
						&MPUData[5]
				);

				// TODO: Handle accelero/gyro data

				doAck = true;
			}
			else if(strncmp(incoming, "s,", 2) == 0)
			{
				// TODO: Handle step

				doAck = true;
			}

			if(doAck)
			{
				receiverDisable();
				transmitterSendAck();
				receiverEnable();
			}
		}

		// If there is no delay here, other tasks will never run
		osDelay(100);
	}
  /* USER CODE END StartRadioReceiveTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

float calculateBattery(uint8_t whatCalculation)
{
	float result = -1.0;
	uint16_t localADC = ADCValue;	//grab the last conversion value
	HAL_ADC_Start_IT(&hadc1);		//start a new conversion already so it's ready for the next time the function is called

	if(whatCalculation == CALCULATEPERCENTAGE)
	{
		result = 71.428571428571 * calculateBattery(CALCULATEVOLTAGE) - 200;
	}
	else if(whatCalculation == CALCULATEVOLTAGE)
	{
		result = (localADC / (float)4095) * 4.2;
	}
	return result;
}

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
