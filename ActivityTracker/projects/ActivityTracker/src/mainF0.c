#include "main.h"

#include <stdbool.h>
#include <FreeRTOS.h>
#include <task.h>

#include "stm32f0xx.h"
#include "stm32f0xx_it.h"
#include "stm32f0xx_i2c.h"

#include "i2c.h"
#include "mpu.h"
#include "usart.h"

extern uint32_t I2CCommStatus;

void delay(const int d);
uint8_t checkComm(void);
void LEDInit(void);

uint16_t aX, aY, aZ, gX, gY, gZ, lastRead;

void vPeriodicTask(void *pvParameters)
{
	// Establish the task's period.
	const TickType_t xDelay = pdMS_TO_TICKS(1000);
	TickType_t xLastWakeTime = xTaskGetTickCount();
	
	for (;;)
	{
		// Block until the next release time.
		vTaskDelayUntil(&xLastWakeTime, xDelay);
		MPUWakeup();
		MPUReadAll(&aX, &aY, &aZ, &gX, &gY, &gZ);
		MPUSleep();
		
		/*
		if(lastRead == aX)
		{
			GPIOWriteBit(GPIOA, GPIO_Pin_2, Bit_SET); // Red
			GPIOWriteBit(GPIOB, GPIO_Pin_2, Bit_RESET); // Green
		} else
		{
			GPIOWriteBit(GPIOA, GPIO_Pin_2, Bit_RESET); // Red
			GPIOWriteBit(GPIOB, GPIO_Pin_2, Bit_SET); // Green
		}
		*/
		
		USARTPrintNumber(aX, "\naX RAW: ");
		USARTPrintNumber(aY, "\naY RAW: ");
		USARTPrintNumber(aZ, "\naZ RAW: ");
		USARTPrintNumber(gX, "\ngX RAW: ");
		USARTPrintNumber(gY, "\ngY RAW: ");
		USARTPrintNumber(gZ, "\ngZ RAW: ");
		lastRead = aX;
		//check_comm();
	}
}
/*
void vPeriodicTask2(void *pvParameters)
{
	// Establish the task's period.
	const TickType_t xDelay = pdMS_TO_TICKS(100);
	TickType_t xLastWakeTime = xTaskGetTickCount();

	for(;;)
	{
		// Block until the next release time.
		vTaskDelayUntil(&xLastWakeTime, xDelay);
		GPIOWriteBit(GPIOB, GPIO_Pin_2, Bit_RESET);
		GPIOWriteBit(GPIOA, GPIO_Pin_2, Bit_SET);
	}
}
*/

int main()
{
	LEDInit();
	lastRead = 0;
	USARTSetup();
	USARTClearScreen();
	I2CSetup(false);
	
	/*
	if(!init_MPU())
	{
		GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_SET); //RED
		GPIO_WriteBit(GPIOB, GPIO_Pin_2, Bit_RESET); //GREEN
	}
	*/
	MPUInit();
	MPUReadAll(&aX, &aY, &aZ, &gX, &gY, &gZ);
	MPUSleep();
	lastRead = aX;
	GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_RESET); //RED
	GPIO_WriteBit(GPIOB, GPIO_Pin_2, Bit_RESET); //GREEN
	
	xTaskCreate(vPeriodicTask, "My Task", 256, NULL, 1, NULL);
	//xTaskCreate(vPeriodicTask2, "My Task2", 256, NULL, 2, NULL);

	 // Startup of the FreeRTOS scheduler.	The program should block here.
	vTaskStartScheduler();

	// The following line should never be reached.	Failure to allocate enough
	// memory from the heap would be one reason.
	for (;;)
	{
		
	}
}

void LEDInit(void)
{
	GPIO_InitTypeDef gpioA;
	GPIO_InitTypeDef gpioB;

	GPIO_StructInit(&gpioA);
	GPIO_StructInit(&gpioB);

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

	gpioA.GPIO_Pin		= GPIO_Pin_2;
	gpioA.GPIO_PuPd		= GPIO_PuPd_NOPULL;
	gpioA.GPIO_Speed	= GPIO_Speed_Level_2;
	gpioA.GPIO_Mode		= GPIO_Mode_OUT;

	gpioB.GPIO_Pin		= GPIO_Pin_2;
	gpioB.GPIO_PuPd		= GPIO_PuPd_NOPULL;
	gpioB.GPIO_Speed	= GPIO_Speed_Level_2;
	gpioB.GPIO_Mode		= GPIO_Mode_OUT;

	GPIO_Init(GPIOA, &gpioA);
	GPIO_Init(GPIOB, &gpioB);
}

uint8_t checkComm(void)
{
	if(I2CCommStatus == I2C_COMM_OK)
	{
		GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_RESET); // Red
		GPIO_WriteBit(GPIOB, GPIO_Pin_2, Bit_SET); // Green
		
		return 0;
	}
	else
	{
		GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_SET); // Red
		GPIO_WriteBit(GPIOB, GPIO_Pin_2, Bit_RESET); // Green
		
		return 1;
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
