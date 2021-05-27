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

#include <stdlib.h>
#include "ssd1306.h"
#include "fonts.h"
#include "stdio.h"
#include "gps.h"
#include "usart.h"
#include "adc.h"
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
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for rReceiveTask */
osThreadId_t rReceiveTaskHandle;
const osThreadAttr_t rReceiveTask_attributes = {
  .name = "rReceiveTask",
  .stack_size = 64 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal1,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

void getTime();
char* getActivity();
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

	SSD1306_GotoXY (0,0);
	sprintf(numbers, "batt: %.0f", batteryPer);
	SSD1306_Puts (numbers, &Font_7x10, 1);

	SSD1306_GotoXY (0, 30);
	sprintf(numbers, "%.2f", GPS.speed_km);
	SSD1306_Puts(numbers, &Font_11x18, 1);
	SSD1306_GotoXY (50,36);
	SSD1306_Puts ("km/u", &Font_7x10, 1);

	getTime();

	SSD1306_GotoXY (0,10);
	SSD1306_Puts(getActivity(), &Font_7x10, 1);

	SSD1306_UpdateScreen();

    osDelay(100);
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
	if(batteryVol < 3.2)
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, SET);
	} else
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
    // Send a test message
	//char * msg = "yo";
	//radioSend(msg);

	osDelay(2000);
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

	radioEnableReceiver();

  /* Infinite loop */
  for(;;)
  {
	// Receive messages
	char * incoming = "";
	if(radioReceive(&incoming))
	{
		// Handle the received message
	}
    //osDelay(1);
  }
  /* USER CODE END StartRadioReceiveTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void getTime()
{
	SSD1306_GotoXY (0, 50);

	char toArray[10];

	float time = GPS.utc_time + 20000; //make it CET
	sprintf(toArray, "%f", time );

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
