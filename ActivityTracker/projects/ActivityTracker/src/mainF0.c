#include "stm32f0_discovery.h"
#include "stm32f0xx.h"
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
		GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_RESET);
    GPIO_WriteBit(GPIOB, GPIO_Pin_2, Bit_SET);
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
		GPIO_WriteBit(GPIOB, GPIO_Pin_2, Bit_RESET);
    GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_SET);
  }
}


int main()
{
  GPIO_InitTypeDef gpioA;
    GPIO_InitTypeDef gpioB;
    
    GPIO_StructInit(&gpioA);
    GPIO_StructInit(&gpioB);

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    
    gpioA.GPIO_Pin        = GPIO_Pin_2;
    gpioA.GPIO_PuPd        = GPIO_PuPd_NOPULL;
    gpioA.GPIO_Speed    = GPIO_Speed_Level_2;
    gpioA.GPIO_Mode        = GPIO_Mode_OUT;
    
    gpioB.GPIO_Pin        = GPIO_Pin_2;
    gpioB.GPIO_PuPd        = GPIO_PuPd_NOPULL;
    gpioB.GPIO_Speed    = GPIO_Speed_Level_2;
    gpioB.GPIO_Mode        = GPIO_Mode_OUT;

    GPIO_Init(GPIOA, &gpioA);
    GPIO_Init(GPIOB, &gpioB);
	
  xTaskCreate(vPeriodicTask, "My Task", 256, NULL, 1, NULL);
  xTaskCreate(vPeriodicTask2, "My Task2", 256, NULL, 2, NULL);

  // Startup of the FreeRTOS scheduler.  The program should block here.
  vTaskStartScheduler();

  // The following line should never be reached.  Failure to allocate enough
  // memory from the heap would be one reason.
  for (;;){
		
	}
}
