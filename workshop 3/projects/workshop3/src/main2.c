/******************************************************************************
 * File           : Main program
 *****************************************************************************/
#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include <string.h>

#define PRESS_A_KEY        "Press a key: "


// ----------------------------------------------------------------------------
// Function prototypes
// ----------------------------------------------------------------------------
void init_USART(void);
void delay(const int d);


void init_USART(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  // Setup Tx- and Rx pin
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);
  
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  // Initialize USART1
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  
  USART_StructInit(&USART_InitStructure);
  USART_InitStructure.USART_BaudRate = 115200;
  USART_Init(USART1, &USART_InitStructure);

  USART_Cmd(USART1, ENABLE);
}

// ----------------------------------------------------------------------------
// Main
// ----------------------------------------------------------------------------
int main(void)
{
  uint16_t key, i;

	init_USART();

  while(1)
  {
		
    // Transmit information: "Press a key: "
		for (i=0; i<strlen(PRESS_A_KEY); i++)
		{
      while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
		  USART_SendData(USART1, (uint16_t) (PRESS_A_KEY[i]));
		}
		
    // Receive data
    while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET){;}
    key = USART_ReceiveData(USART1);
    
    // Make uppercase
    if(key >= (uint16_t)'a' && key <= (uint16_t)'z')
    {
      key -= 32;
    }
    
    // Transmit uppercase and goto next line
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, key);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, (uint16_t)'\n');
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, (uint16_t)'\r');    
  }
}

#pragma push
#pragma O3
void delay(const int d)
{
  volatile int i;

  for(i=d; i>0; i--){ ; }

  return;
}
#pragma pop
