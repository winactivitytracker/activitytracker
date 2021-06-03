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
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "mpu.h"
#include "radioAPI.h"
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define MAXTIMEOUT 5

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

void alohaWait();

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void sendGyroZ()
{
	uint8_t timeoutCounter = MAXTIMEOUT;
	// Get the time
	RTC_TimeTypeDef currTime;
	RTC_DateTypeDef currDate;
	HAL_RTC_GetTime(&hrtc, &currTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &currDate, RTC_FORMAT_BIN);

	// Get the gyro Z axis
	int16_t gXRaw, gYRaw, gZRaw;
	uint8_t sec, min, hour;
	MPU6050ReadGyro(&gXRaw,&gYRaw,&gZRaw);

	char MPUString[60] = "";
	hour = currTime.Hours;
	min = currTime.Minutes;
	sec = currTime.Seconds;

	sprintf(MPUString,
		"z,%u,%u,%u,%u,%d",
		IDENTIFIER,
		hour,
		min,
		sec,
		gZRaw
	);

	// Send the initial message
	transmitterSendBlocking(MPUString);

	// Wait for an ACK
	while(!receiverWaitForAck((200)) && timeoutCounter != 0)
	{
		alohaTimer();
		//HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
		transmitterSendBlocking(MPUString);
		timeoutCounter--;
	}

	HAL_Delay(250);
}

void sendAccelFull()
{
	MPUReadAll(
		&MPUData[0],
		&MPUData[1],
		&MPUData[2],
		&MPUData[3],
		&MPUData[4],
		&MPUData[5]
	);

	// "a:,,,,,"	:  7 chars
	//		6 * 5	= 30 chars
	// 				  --
	//				  37

	char MPUDataString[38] = "";

	sprintf(MPUDataString,
		"a:%d,%d,%d,%d,%d,%d",
		MPUData[0],
		MPUData[1],
		MPUData[2],
		MPUData[3],
		MPUData[4],
		MPUData[5]
	);

	do
	{
		// Send the data
		transmitterSendBlocking(MPUDataString);
	}
	while(!receiverWaitForAck((200 + (rand() % 200))));

	HAL_Delay(5000);
}

void alohaWait()
{
	uint16_t r = 200 + (rand() % 300);
	HAL_Delay(r);
}

void askTime()
{
	uint8_t timeoutCounter = MAXTIMEOUT;

	char timeAskString[4] = "";
	sprintf(timeAskString,
		"t,%u",
		IDENTIFIER
	);

	// Send the data
	transmitterSendBlocking(timeAskString);

	while(!receiverWaitForAck(300) && timeoutCounter != 0)
	{
		alohaWait();
		transmitterSendBlocking(timeAskString);
		timeoutCounter--;
	}
}

bool getTime()
{
	unsigned int inputArray[3];
	char incoming[100];

	for(;;)
	{
		if(receiverCheckMessage())
		{
			receiverPopMessage(incoming);

			if(strncmp(incoming, "tt,", 3) == 0)
			{
				sscanf(incoming, "tt,%u:%u:%u", &inputArray[0], &inputArray[1], &inputArray[3]);
				RTC_SetTime(inputArray[0], inputArray[1], inputArray[2]);

				transmitterSendAck();

				return true;
			}
		}

		HAL_Delay(50);
	}
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

	bool hasTime = false;

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
  MX_TIM15_Init();
  MX_TIM16_Init();
  /* USER CODE BEGIN 2 */

	MPU6050Init();
	MPUSetAccel(MPU_A16G);
	MPUSetGyro(MPU_G2000G);

	// Start listening for messages
	receiverEnable();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{
		if(!hasTime)
		{
			askTime();
			if(getTime())
			{
				hasTime = true;
			}
		}
		else
		{
			sendGyroZ();
		}

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
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
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

/* USER CODE END 4 */

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
