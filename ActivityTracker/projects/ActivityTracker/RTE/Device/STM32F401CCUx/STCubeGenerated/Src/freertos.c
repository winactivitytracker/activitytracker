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

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for LEDOn */
osThreadId_t LEDOnHandle;
const osThreadAttr_t LEDOn_attributes = {
  .name = "LEDOn",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for LEDOff */
osThreadId_t LEDOffHandle;
const osThreadAttr_t LEDOff_attributes = {
  .name = "LEDOff",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void LEDTurnOn(void *argument);
void LEDTurnOff(void *argument);

extern void MX_USB_DEVICE_Init(void);
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

  /* creation of LEDOn */
  LEDOnHandle = osThreadNew(LEDTurnOn, NULL, &LEDOn_attributes);

  /* creation of LEDOff */
  LEDOffHandle = osThreadNew(LEDTurnOff, NULL, &LEDOff_attributes);

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
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_LEDTurnOn */
/**
* @brief Function implementing the LEDOn thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_LEDTurnOn */
void LEDTurnOn(void *argument)
{
  /* USER CODE BEGIN LEDTurnOn */
  /* Infinite loop */
  for(;;)
  {
	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_8);
	//HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_9);
    osDelay(300);
  }
  /* USER CODE END LEDTurnOn */
}

/* USER CODE BEGIN Header_LEDTurnOff */
/**
* @brief Function implementing the LEDOff thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_LEDTurnOff */
void LEDTurnOff(void *argument)
{
  /* USER CODE BEGIN LEDTurnOff */
  /* Infinite loop */
  for(;;)
  {
	static uint16_t counter = 0;
	char numbers[5];
	  

	
	//SSD1306_Clear();
	//SSD1306_GotoXY (1,10); // goto 10, 10 
	//SSD1306_Puts ("The Real Activity Tracker", &Font_7x10, 1); // print Hello 
	if(counter >= 10 && counter < 100)
		SSD1306_GotoXY (55, 24); 
	else if(counter >= 100)
		SSD1306_GotoXY (48, 24); 
	else
		SSD1306_GotoXY (60, 24);
	
	
	  
	 //SSD1306_GotoXY (1,10); // goto 10, 10 
	SSD1306_DrawFilledCircle(64, 32, 30, 1);
	sprintf(numbers, "%d", counter);
	SSD1306_Puts(numbers, &Font_11x18, 0);
	
	SSD1306_UpdateScreen();
   
	counter++;
	osDelay(1000);
  }
  /* USER CODE END LEDTurnOff */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
