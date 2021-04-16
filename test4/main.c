#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>

void delay(const int d);

void vPeriodicTask(void *pvParameters)
{
  // Establish the task's period.
  const TickType_t xDelay = pdMS_TO_TICKS(1000);
  TickType_t xLastWakeTime = xTaskGetTickCount();
  for (;;) {
    // Block until the next release time.
    vTaskDelayUntil(&xLastWakeTime, xDelay);
		
		GPIOC->BSRR = GPIO_BSRR_BR_8;
  }
}

void vPeriodicTask2(void *pvParameters)
{
  // Establish the task's period.
  const TickType_t xDelay = pdMS_TO_TICKS(700);
  TickType_t xLastWakeTime = xTaskGetTickCount();
  for (;;) {
    // Block until the next release time.
    vTaskDelayUntil(&xLastWakeTime, xDelay);
		
		GPIOC->BSRR = GPIO_BSRR_BS_8;
  }
}


int main()
{
  // GPIOC Periph clock enable
  RCC->AHBENR |= RCC_AHBENR_GPIOCEN; 

  // PC8 and PC9 in output mode
  GPIOC->MODER |= (GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0);

  // Push pull mode selected
  GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_9);

  // Maximum speed setting
  GPIOC->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR8 | GPIO_OSPEEDER_OSPEEDR9);

  // Pull-up and pull-down resistors disabled
  GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR8 | GPIO_PUPDR_PUPDR9);

  GPIOC->BSRR = GPIO_BSRR_BS_9;
	
  xTaskCreate(vPeriodicTask, "My Task", 256, NULL, 1, NULL);
  xTaskCreate(vPeriodicTask2, "My Task2", 256, NULL, 2, NULL);

  // Startup of the FreeRTOS scheduler.  The program should block here.
  vTaskStartScheduler();

  // The following line should never be reached.  Failure to allocate enough
  // memory from the heap would be one reason.
  for (;;){
		
	}
}
