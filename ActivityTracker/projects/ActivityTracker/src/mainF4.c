#include <FreeRTOS.h>
#include <task.h>

#include "stm32f0_discovery.h"
#include "stm32f0xx.h"


void delay(const int d);
void GPIOInit(void);

void vPeriodicTask(void *pvParameters)
{
	// Establish the task's period.
	const TickType_t xDelay = pdMS_TO_TICKS(1000);
	TickType_t xLastWakeTime = xTaskGetTickCount();
	
	for (;;)
	{
		// Block until the next release time.
		vTaskDelayUntil(&xLastWakeTime, xDelay);
		GPIO_WriteBit(GPIOB, GPIO_Pin_8, Bit_RESET);
		GPIO_WriteBit(GPIOB, GPIO_Pin_9, Bit_SET);
	}
}

void vPeriodicTask2(void *pvParameters)
{
	// Establish the task's period.
	const TickType_t xDelay = pdMS_TO_TICKS(700);
	TickType_t xLastWakeTime = xTaskGetTickCount();
	
	for (;;)
	{
		// Block until the next release time.
		vTaskDelayUntil(&xLastWakeTime, xDelay);
		GPIO_WriteBit(GPIOB, GPIO_Pin_8, Bit_RESET);
		GPIO_WriteBit(GPIOB, GPIO_Pin_9, Bit_SET);
	}
}

int main()
{
	GPIOInit();
	
	xTaskCreate(vPeriodicTask, "My Task", 256, NULL, 1, NULL);
	xTaskCreate(vPeriodicTask2, "My Task2", 256, NULL, 2, NULL);

	// Startup of the FreeRTOS scheduler. The program should block here.
	vTaskStartScheduler();

	// The following line should never be reached.	Failure to allocate enough
	// memory from the heap would be one reason.
	for (;;)
	{
		
	}
}

void GPIOInit(void){
	GPIO_InitTypeDef gpioB;
	
	GPIO_StructInit(&gpioB);

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	
	gpioB.GPIO_Pin		= GPIO_Pin_8 | GPIO_Pin_9;
	gpioB.GPIO_PuPd		= GPIO_PuPd_NOPULL;
	gpioB.GPIO_Speed	= GPIO_Speed_Level_2;
	gpioB.GPIO_Mode		= GPIO_Mode_OUT;

	GPIO_Init(GPIOB, &gpioB);
}
