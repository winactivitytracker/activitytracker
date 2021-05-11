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

#include "mpu.h"
#include <stdio.h>

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

uint16_t MPUData[6];

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 64 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for REDLED */
osThreadId_t REDLEDHandle;
const osThreadAttr_t REDLED_attributes = {
  .name = "REDLED",
  .stack_size = 64 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for ReadMPU */
osThreadId_t ReadMPUHandle;
const osThreadAttr_t ReadMPU_attributes = {
  .name = "ReadMPU",
  .stack_size = 64 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for GREENLED */
osThreadId_t GREENLEDHandle;
const osThreadAttr_t GREENLED_attributes = {
  .name = "GREENLED",
  .stack_size = 64 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartREDLED(void *argument);
void StartReadMPU(void *argument);
void StartGREENLED(void *argument);

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

  /* creation of REDLED */
  REDLEDHandle = osThreadNew(StartREDLED, NULL, &REDLED_attributes);

  /* creation of ReadMPU */
  ReadMPUHandle = osThreadNew(StartReadMPU, NULL, &ReadMPU_attributes);

  /* creation of GREENLED */
  GREENLEDHandle = osThreadNew(StartGREENLED, NULL, &GREENLED_attributes);

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
	  osDelay(200);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartREDLED */
/**
* @brief Function implementing the REDLED thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartREDLED */
void StartREDLED(void *argument)
{
  /* USER CODE BEGIN StartREDLED */
  /* Infinite loop */
  for(;;)
  {
	  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_2);
	  osDelay(100);
  }
  /* USER CODE END StartREDLED */
}

/* USER CODE BEGIN Header_StartReadMPU */
/**
* @brief Function implementing the ReadMPU thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartReadMPU */
void StartReadMPU(void *argument)
{
  /* USER CODE BEGIN StartReadMPU */
  /* Infinite loop */
  for(;;)
  {
	  MPUReadAll(MPUData[0], MPUData[1], MPUData[2], MPUData[3], MPUData[4], MPUData[5]);
	  osDelay(50);
  }
  /* USER CODE END StartReadMPU */
}

/* USER CODE BEGIN Header_StartGREENLED */
/**
* @brief Function implementing the GREENLED thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartGREENLED */
void StartGREENLED(void *argument)
{
  /* USER CODE BEGIN StartGREENLED */
  /* Infinite loop */
  for(;;)
  {
	  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);
	  osDelay(100);
  }
  /* USER CODE END StartGREENLED */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
