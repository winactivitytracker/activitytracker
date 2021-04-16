/******************************************************************************
 * File           : Main program - Timebase
 *****************************************************************************/
#include "stm32f0xx.h"
#include "stm32f0_discovery.h"

// ----------------------------------------------------------------------------
// Function prototypes
// ----------------------------------------------------------------------------
void init_Timer3(void);
void init_Timer3_Interrupt(void);

// ----------------------------------------------------------------------------
// Setup Timer 3 to overflow every 1 second
// ----------------------------------------------------------------------------
void init_Timer3(void)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
  //[..] To use the Timer in Timing(Time base) mode, the following steps are 
  //     mandatory:
  
  //(#) Enable TIM clock using 
  //    RCC_APBxPeriphClockCmd(RCC_APBxPeriph_TIMx, ENABLE) function.
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  
  //(#) Fill the TIM_TimeBaseInitStruct with the desired parameters.
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period      = 1000-1;
  TIM_TimeBaseStructure.TIM_Prescaler   = (uint16_t)((SystemCoreClock / 1000) - 1);
  
  //(#) Call TIM_TimeBaseInit(TIMx, &TIM_TimeBaseInitStruct) to configure 
  //    the Time Base unit with the corresponding configuration.
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
}

// ----------------------------------------------------------------------------
// Setup a Timer 3 Update interrupt
// ----------------------------------------------------------------------------
void init_Timer3_Interrupt(void)
{
  NVIC_InitTypeDef        NVIC_InitStructure;

  //(#) Enable the NVIC if you need to generate the update interrupt. 
  //    Enable the TIM3 gloabal Interrupt
  NVIC_InitStructure.NVIC_IRQChannel         = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd      = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  //(#) Enable the corresponding interrupt using the function 
  //    TIM_ITConfig(TIMx, TIM_IT_Update). 
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
  
  //(#) Call the TIM_Cmd(ENABLE) function to enable the TIM counter.
  TIM_Cmd(TIM3, ENABLE);
}


// ----------------------------------------------------------------------------
// Main
// ----------------------------------------------------------------------------
int main(void)
{
  // Configure LED3 and LED4 on STM32F0-Discovery
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
  
  // Initialize User Button on STM32F0-Discovery
  STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_GPIO);
  
	init_Timer3();
	init_Timer3_Interrupt();
	
  while(1)
  {
    // Button pressed?
    if (STM_EVAL_PBGetState(BUTTON_USER) == Bit_SET)
    {
      STM_EVAL_LEDOn(LED3);
    }
    else
    {
      STM_EVAL_LEDOff(LED3);
    }
  }
}
