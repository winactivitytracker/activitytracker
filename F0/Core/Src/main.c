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
uint8_t forCounter;

char* nameArray[6] = {"aX", "aY", "aZ"};
RTC_TimeTypeDef currTime, currDate = {0};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

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

  for(uint8_t i = 0; i < 3; i++)
  {
	  HAL_UART_Transmit(&huart1, nameArray[i], sizeof(nameArray[i]), HAL_MAX_DELAY);
	  if(i != 2)
	  {
		  HAL_UART_Transmit(&huart1, ",", sizeof(","), HAL_MAX_DELAY);
	  }
	  if(i == 2)
	  {
		  HAL_UART_Transmit(&huart1, "\n", sizeof("\n"), HAL_MAX_DELAY);
	  }
  }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  char numbers[6];
	  float current;
	  float previous[3];
//	  if(counter == 20)
//	  {
//		  char numbers[6];
//		  float current;
//		  for(uint8_t i = 0; i < 20; i++)
//		  {
//			  MPUData[0] = MPUData[0] + allData[i][0];
//			  MPUData[1] = MPUData[1] + allData[i][1];
//			  MPUData[2] = MPUData[2] + allData[i][2];
//			  MPUData[3] = MPUData[3] + allData[i][3];
//			  MPUData[4] = MPUData[4] + allData[i][4];
//			  MPUData[5] = MPUData[5] + allData[i][5];
//		  }
//		  forCounter = 0;
//		  while(forCounter < 6)
//		  {
//			  MPUData[forCounter] = MPUData[forCounter] / counter;
//			  if(forCounter < 3)
//			  {
//			  	  current = (float)MPUData[forCounter] / 2048.0;
//			  } else
//			  {
//			   	  current = (float)MPUData[forCounter] / 16.4;
//			  }
//			  MPUData[forCounter] = 0;
//			  sprintf(numbers, "%.3f", current);
//			  HAL_UART_Transmit(&huart1, numbers, sizeof(numbers), HAL_MAX_DELAY);
//			 if(forCounter != 5)
//			 {
//				 HAL_UART_Transmit(&huart1, ", ", sizeof(", "), HAL_MAX_DELAY);
//			 }
//			 if(forCounter == 5)
//			 {
//				 HAL_UART_Transmit(&huart1, "\n", sizeof("\n"), HAL_MAX_DELAY);
//			 }
//			 forCounter++;
//		  }
//		   counter = 0;
//		   HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);
//	  }
	  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_2);
	  MPUReadAll(&allData[counter][0], &allData[counter][1], &allData[counter][2], &allData[counter][3], &allData[counter][4], &allData[counter][5]);
	  //counter++;

	  forCounter = 0;
	  		  while(forCounter < 3)
	  		  {
	  			 HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);
	  			  //MPUData[forCounter] = MPUData[forCounter] / counter;
	  			  if(forCounter < 3)
	  			  {
	  			  	  current = (float)allData[0][forCounter] / 2048.0;
	  			  } else
	  			  {
	  			   	  current = (float)allData[0][forCounter] / 16.4;
	  			  }
	  			  allData[0][forCounter] = 0;

	  			  if(forCounter == 2 && current > previous[forCounter] + 0.8)
	  			  {
					  HAL_UART_Transmit(&huart1, "STAP", sizeof(numbers), HAL_MAX_DELAY);
					  forCounter = 2;
				  }
//	  			  else
//	  			  {
//					  sprintf(numbers, "%.3f", current);
//					  HAL_UART_Transmit(&huart1, numbers, sizeof(numbers), HAL_MAX_DELAY);
//	  			  }
//
//	  			  if(forCounter != 2)
//	  			 {
//	  				 HAL_UART_Transmit(&huart1, ", ", sizeof(", "), HAL_MAX_DELAY);
//	  			 }
	  			 if(forCounter == 2)
	  			 {
	  				HAL_UART_Transmit(&huart1, "\n", sizeof("\n"), HAL_MAX_DELAY);

	  			 }
	  			 previous[forCounter] = current;
	  			 forCounter++;
	  		  }
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