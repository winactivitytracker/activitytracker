/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "mpu.h"
#include "radioAPI.h"
#include "radioHandler.h"
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

// F0 Radio timers
#define SEND_TIMER		&htim15
#define RECEIVE_TIMER	&htim16

// 1 for wrist, 0 for ankle
#define IDENTIFIER		0
#define TEST			1
#define TEST_MPU		1
#define TEST_RTC 		0
#define TEST_RADIO 		0
#define DO_NOT_ENCRYPT	1

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_RED_Pin GPIO_PIN_2
#define LED_RED_GPIO_Port GPIOA
#define RADIO_RECEIVE_Pin GPIO_PIN_1
#define RADIO_RECEIVE_GPIO_Port GPIOB
#define LED_GREEN_Pin GPIO_PIN_2
#define LED_GREEN_GPIO_Port GPIOB
#define USART_TX_Pin GPIO_PIN_9
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_10
#define USART_RX_GPIO_Port GPIOA
#define SWD_IO_Pin GPIO_PIN_13
#define SWD_IO_GPIO_Port GPIOA
#define SWD_CLOCK_Pin GPIO_PIN_14
#define SWD_CLOCK_GPIO_Port GPIOA
#define RADIO_SEND_Pin GPIO_PIN_3
#define RADIO_SEND_GPIO_Port GPIOB
#define I2C_CLOCK_Pin GPIO_PIN_6
#define I2C_CLOCK_GPIO_Port GPIOB
#define I2C_DATA_Pin GPIO_PIN_7
#define I2C_DATA_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
