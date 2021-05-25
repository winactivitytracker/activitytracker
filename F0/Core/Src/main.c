/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "rtc.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "mpu.h"
#include <stdio.h>
#include <stdbool.h>

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

/* USER CODE BEGIN PV */

int32_t MPUData[6];
int16_t allData[20][6];
int16_t MPURTCData[40][4][6];

uint8_t counter = 0;

char* nameArray[6] = {"aX", "aY", "aZ", "gX", "gY", "gZ"};
RTC_TimeTypeDef currTime, currDate = {0};

uint8_t orientationLeg[2];

int16_t previous;

uint8_t forceCounter;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
float editMPUData();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */

  MPU6050Init();
  MPUSetAccel(MPU_A16G);
  MPUSetGyro(MPU_G2000G);

  MPUOrientation(&orientationLeg[0], &orientationLeg[1]);

  if(orientationLeg[1] == 1)
  {
	  HAL_UART_Transmit(&huart1, "-", sizeof("-"), HAL_MAX_DELAY);
  }
  if(orientationLeg[0] == 0)
  {
	  HAL_UART_Transmit(&huart1, "X", sizeof("X"), HAL_MAX_DELAY);
  }
  else if(orientationLeg[0] == 1)
  {
	  HAL_UART_Transmit(&huart1, "Y", sizeof("Y"), HAL_MAX_DELAY);
  }
  else
  {
	  HAL_UART_Transmit(&huart1, "Z", sizeof("Z"), HAL_MAX_DELAY);
  }
  HAL_UART_Transmit(&huart1, "\n", sizeof("\n"), HAL_MAX_DELAY);

  for(uint8_t i = 0; i < 3; i++)
  {
	  HAL_UART_Transmit(&huart1, nameArray[i], sizeof(nameArray[i]), HAL_MAX_DELAY);
	  if(i != 5)
	  {
		  HAL_UART_Transmit(&huart1, ",", sizeof(","), HAL_MAX_DELAY);
	  }
	  else
	  {
		  HAL_UART_Transmit(&huart1, "\n", sizeof("\n"), HAL_MAX_DELAY);
	  }
  }
  char numberss[6];
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_2);

	  stap();

	  MPUReadAll(&allData[0][0], &allData[0][1], &allData[0][2], &allData[0][3], &allData[0][4], &allData[0][5]);

	  forceCounter = 0;
	  while(forceCounter < 6)
	  {
		sprintf(numberss, "%d", allData[0][forceCounter]);
		HAL_UART_Transmit(&huart1, numberss, sizeof(numberss), HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart1, "\t", sizeof("\t"), HAL_MAX_DELAY);
		forceCounter++;
	  }
	  HAL_UART_Transmit(&huart1, "\n", sizeof("\n"), HAL_MAX_DELAY);

	  //HAL_UART_Transmit(&huart1, "\n", sizeof("\n"), HAL_MAX_DELAY);
	  HAL_Delay(250);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_I2C1
                              |RCC_PERIPHCLK_RTC;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_SYSCLK;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void getMPUData()
{
	HAL_RTC_GetTime(&hrtc, &currTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &currDate, RTC_FORMAT_BIN);
	MPUReadAll(&MPURTCData[currTime.Seconds][counter][0], &MPURTCData[currTime.Seconds][counter][1], &MPURTCData[currTime.Seconds][counter][2], &MPURTCData[currTime.Seconds][counter][3], &MPURTCData[currTime.Seconds][counter][4], &MPURTCData[currTime.Seconds][counter][5]);
}

void stap()
{
//	uint8_t forCounter = 0;
//	char numbers[6];
	int16_t data[3];

	MPU6050ReadAccel(&data[0], &data[1], &data[2]);
/*
	sprintf(numbers, "%d", data[0]);
	HAL_UART_Transmit(&huart1, numbers, sizeof(numbers), HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, "_", sizeof("_"), HAL_MAX_DELAY);
	sprintf(numbers, "%d", data[1]);
	HAL_UART_Transmit(&huart1, numbers, sizeof(numbers), HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, "_", sizeof("_"), HAL_MAX_DELAY);
	sprintf(numbers, "%d", data[2]);
	HAL_UART_Transmit(&huart1, numbers, sizeof(numbers), HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, "_", sizeof("_"), HAL_MAX_DELAY);*/
	if(orientationLeg[1] == 0)
	{
		if(data[orientationLeg[0]] > (previous + 2000))
		{
			HAL_UART_Transmit(&huart1, "STAP", sizeof("STAP"), HAL_MAX_DELAY);
		 }
	} else
	{
		if(data[orientationLeg[0]] < (previous - 2000))
		{
		 	HAL_UART_Transmit(&huart1, "STAP", sizeof("STAP"), HAL_MAX_DELAY);
		}
	}
	previous = data[orientationLeg[0]];
	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);
	HAL_UART_Transmit(&huart1, "\n", sizeof("\n"), HAL_MAX_DELAY);
}

/* USER CODE END 4 */

 /**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM3 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM3) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
