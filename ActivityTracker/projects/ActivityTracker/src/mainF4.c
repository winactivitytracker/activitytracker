//#include "stm32f0_discovery.h"
//#include "stm32f0xx.h"
#include "stm32f4xx.h"
#include <FreeRTOS.h>
#include <task.h>

void delay(const int d);
void initGpio(void);

void vPeriodicTask(void *pvParameters)
{
  // Establish the task's period.
  const TickType_t xDelay = pdMS_TO_TICKS(200);
  TickType_t xLastWakeTime = xTaskGetTickCount();
  for (;;) {
    // Block until the next release time.
    vTaskDelayUntil(&xLastWakeTime, xDelay);
		//GPIO_WriteBit(GPIOB, GPIO_Pin_8, Bit_RESET);
    //GPIO_WriteBit(GPIOB, GPIO_Pin_9, Bit_SET);
		GPIOB->BSRR = GPIO_BSRR_BR_8;
		GPIOB->BSRR = GPIO_BSRR_BS_9;
  }
}

void vPeriodicTask2(void *pvParameters)
{
  // Establish the task's period.
  const TickType_t xDelay = pdMS_TO_TICKS(100);
  TickType_t xLastWakeTime = xTaskGetTickCount();
  for (;;) {
    // Block until the next release time.
    vTaskDelayUntil(&xLastWakeTime, xDelay);
		//GPIO_WriteBit(GPIOB, GPIO_Pin_8, Bit_RESET);
    //GPIO_WriteBit(GPIOB, GPIO_Pin_9, Bit_SET);
		GPIOB->BSRR = GPIO_BSRR_BR_9;
		GPIOB->BSRR = GPIO_BSRR_BS_8;
  }
}


int main()
{
	
  // GPIOC Periph clock enable
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; 

  // PC8 and PC9 in output mode
  GPIOB->MODER |= (GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0);

  // Push pull mode selected
  GPIOB->OTYPER &= ~(GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_9);

  // Maximum speed setting
  GPIOB->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR8 | GPIO_OSPEEDER_OSPEEDR9);

  // Pull-up and pull-down resistors disabled
  GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR8 | GPIO_PUPDR_PUPDR9);

  //GPIOB->BSRR = GPIO_BSRR_BS_9;
  while(1)
  {
    // Set PC9
		GPIOB->BSRR = GPIO_BSRR_BS_9;
    // Reset PC8
    GPIOB->BSRR = GPIO_BSRR_BR_8;
    // Delay ~ 1 sec.
    delay(SystemCoreClock/8);    
    
    // Reset PC9
    GPIOB->BSRR = GPIO_BSRR_BR_9;
    // Set PC8
    GPIOB->BSRR = GPIO_BSRR_BS_8;
    // Delay ~ 1 sec.
    delay(SystemCoreClock/8);
		
  }
	
	
//	initGpio();
	
  xTaskCreate(vPeriodicTask, "My Task", 256, NULL, 1, NULL);
  xTaskCreate(vPeriodicTask2, "My Task2", 256, NULL, 2, NULL);

  // Startup of the FreeRTOS scheduler.  The program should block here.
  vTaskStartScheduler();

  // The following line should never be reached.  Failure to allocate enough
  // memory from the heap would be one reason.
  for (;;){
		
	}
}

void initGpio(void){
	/*GPIO_InitTypeDef gpioB;
	
	GPIO_StructInit(&gpioB);

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	
	gpioB.GPIO_Pin        = GPIO_Pin_8 | GPIO_Pin_9;
	gpioB.GPIO_PuPd       = GPIO_PuPd_NOPULL;
	gpioB.GPIO_Speed    	= GPIO_Speed_Level_2;
	gpioB.GPIO_Mode       = GPIO_Mode_OUT;

	GPIO_Init(GPIOB, &gpioB);*/
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
