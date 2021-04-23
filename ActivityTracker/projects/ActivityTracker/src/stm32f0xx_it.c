/**
  ******************************************************************************
  * @file    stm32f0xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-March-2012
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_it.h"
#include "main.h"
//#include "STM32F0_discovery.h"

// ----------------------------------------------------------------------------
// Global variables
// ----------------------------------------------------------------------------

int LEDState = 0;
extern uint32_t MPU_6050_CommStatus;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  *
void SVC_Handler(void)
{
}


  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  *
void PendSV_Handler(void)
{
}

**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  *
void SysTick_Handler(void)
{
  static uint32_t ticks=0;

  if(ticks++ ==   0){GPIOC->BSRR = 0x0200;} // Green LED on
  if(ticks   ==  15){GPIOC->BRR  = 0x0200;} // Green LED off
  if(ticks   ==  30){GPIOC->BSRR = 0x0200;} // Green LED on
  if(ticks   ==  45){GPIOC->BRR  = 0x0200;} // Green LED off
  if(ticks   == 300){ticks=0;}
}

******************************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f0xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles TIM3 global interrupt request.
  * @param  None
  * @retval None
  */
void TIM3_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
  }
}

void I2C1_IRQnHandler(void)
{
	if(I2C_GetITStatus(I2C1, I2C_IT_TXIS) != RESET)
	{
		I2C_ClearITPendingBit(I2C1, I2C_IT_TXIS);
	}
	if(I2C_GetITStatus(I2C1, I2C_IT_RXNE) != RESET)
	{
		I2C_ClearITPendingBit(I2C1, I2C_IT_RXNE);
	}
	if(I2C_GetITStatus(I2C1, I2C_IT_TIMEOUT) != RESET)
	{
		I2C_ClearITPendingBit(I2C1, I2C_IT_TIMEOUT);
	}
	if(LEDState)
	{
		GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_RESET); //RED
		LEDState = 0;
	} else 
	{
		GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_SET); //RED
		LEDState = 1;
	}
	
}

void TIM14_IRQHandler(void)
{
	static bool outputOn = false;
	if(TIM_GetITStatus(TIM14, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM14, TIM_IT_Update);
		if(outputOn)
		{
			GPIO_WriteBit(GPIOB, GPIO_Pin_3, Bit_RESET);
			outputOn = false;
		} else
		{
			GPIO_WriteBit(GPIOB, GPIO_Pin_3, Bit_SET);
			outputOn = true;
		}
	}
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
