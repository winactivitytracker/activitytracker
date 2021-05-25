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
/* Definitions for DrawOnOledTask */
osThreadId_t DrawOnOledTaskHandle;
const osThreadAttr_t DrawOnOledTask_attributes = {
  .name = "DrawOnOledTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for ActivityTask */
osThreadId_t ActivityTaskHandle;
const osThreadAttr_t ActivityTask_attributes = {
  .name = "ActivityTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void getTime();
char * getActivity();
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartDrawing(void *argument);
void StartActivityTask(void *argument);

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

//	SSD1306_GotoXY (0,0);
//	SSD1306_Puts ("R.A.T:", &Font_11x18, 1);

	SSD1306_GotoXY (0, 20);
	sprintf(numbers, "%.2f", GPS.speed_km);
	SSD1306_Puts(numbers, &Font_11x18, 1);
	SSD1306_GotoXY (50,26);
	SSD1306_Puts ("km/u", &Font_7x10, 1);

	getTime();



	SSD1306_GotoXY (0,0);
	SSD1306_Puts(getActivity(), &Font_7x10, 1);

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
	static float time;
	static uint8_t activityPM[20];
	static uint8_t counter = 0, counterPM = 0, counterPauze = 0;
	static uint8_t trackActivity[4];
  /* Infinite loop */
  for(;;)
  {
	getActivity();

	if(time != GPS.utc_time)
	{
		time = GPS.utc_time;

		if(counter < 60)
		{
			switch (GPS.currentActivity) {
				case noMovement:
					trackActivity[noMovement]++;
					break;
				case walking:
					trackActivity[walking]++;
					break;
				case running:
					trackActivity[running]++;
					break;
				default:
					trackActivity[unknown]++;
					break;
			}

			counter++;
		}
		else
		{
			uint8_t current = 0, index = 0;

			for(int i = 0; i < 4; i++)
			{
				if(current < trackActivity[i])
				{
					current = trackActivity[i];
					trackActivity[i] = 0;
					index = i;
				}
			}

			if(counterPM < 20)
			{
				if(counterPM == 0 && index == walking || index == running)
				{
					activityPM[counterPM] = index;
					//possibility to write the activity of the last active minute to sd!!
					//not implemented
					counterPM++;
				}
				else if(counterPM != 0 && index == noMovement || index == unknown)
				{
					if(counterPauze < 2)
					{
						counterPauze++;
						counterPM++;
					}
					else
					{
						counterPM = 20;
						counterPauze = 0;
					}
				}
			}
			else
			{
				//calculate the avarage for total activity
				//and write total activity to sd
				//not implemented

				counterPM = 0;
			}
			counter = 0;
		}

	}

    osDelay(100);
  }
  /* USER CODE END StartActivityTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void getTime()
{
	SSD1306_GotoXY (0, 45);

	char toArray[10];

	float time = GPS.utc_time + 20000; //make it CET
	sprintf(toArray, "%f", time);

	if(time < 100000)
		sprintf(toArray, "%c:%c%c:%c%c", toArray[0], toArray[1], toArray[2], toArray[3], toArray[4]);
	else
		sprintf(toArray, "%c%c:%c%c:%c%c", toArray[0], toArray[1], toArray[2], toArray[3], toArray[4], toArray[5]);

	SSD1306_Puts(toArray, &Font_7x10, 1);
}

char * getActivity()
{
	char * activity;

	if(GPS.speed_km < 3.0)
		{
			activity = "Geen beweging";
			GPS.currentActivity = noMovement;
		}
		else if(GPS.speed_km >= 2.0 && GPS.speed_km < 7.0)
		{
			activity = "Wandelen     ";
			GPS.currentActivity = walking;
		}
		else if(GPS.speed_km >= 7.0 && GPS.speed_km < 15.0)
		{
			activity = "Hardlopen    ";
			GPS.currentActivity = running;
		}
		else
		{
			activity = "onbekend     ";
			GPS.currentActivity =  unknown;
		}

	return activity;
}

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
