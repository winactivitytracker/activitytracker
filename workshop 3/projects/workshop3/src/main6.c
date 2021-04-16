/******************************************************************************
 * File           : Main program - SysTick
 *****************************************************************************/
#include "stm32f0xx.h"
#include "stm32f0_discovery.h"

// ----------------------------------------------------------------------------
// Function prototypes
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Main
// ----------------------------------------------------------------------------
int main(void)
{
  // Configure LED3 and LED4 on STM32F0-Discovery
  STM_EVAL_LEDInit(LED3);
  
  // Setup SysTick Timer for 5 msec interrupts
  if(SysTick_Config((SystemCoreClock * 0.005)-1))
  { 
    // Error!
    // Do nothing, LED3 will stay off indicating an erronous situation.
    while(1);
  }

  while(1)
  {
    
  }
}
