#include "stm32f0xx.h"
#include "stm32f0xx_it.h"
#include "main.h"
#include "stm32f0xx_i2c.h"
#include "I2C_main.h"
#include "MPU_6050_main.h"
#include "USART_main.h"
#include "stdbool.h"
#include <FreeRTOS.h>
#include <task.h>

extern uint32_t I2C_CommStatus;

void delay(const int d);
uint8_t check_comm(void);
void init_LED(void);

uint16_t aX, aY, aZ, gX, gY, gZ, lastRead;

void vPeriodicTask(void *pvParameters)
{
    // Establish the task's period.
    const TickType_t xDelay = pdMS_TO_TICKS(1000);
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for (;;) {
        // Block until the next release time.
        vTaskDelayUntil(&xLastWakeTime, xDelay);
		MPU_wakeup();
		MPU_read_all(&aX, &aY, &aZ, &gX, &gY, &gZ);
		MPU_sleep();
		/*
		if(lastRead == aX)
		{
			GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_SET); //RED
			GPIO_WriteBit(GPIOB, GPIO_Pin_2, Bit_RESET); //GREEN
		} else
		{
			GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_RESET); //RED
			GPIO_WriteBit(GPIOB, GPIO_Pin_2, Bit_SET); //GREEN
		}
		*/
		USART_print_number(aX, "\naX RAW: ");
		USART_print_number(aY, "\naY RAW: ");
		USART_print_number(aZ, "\naZ RAW: ");
		USART_print_number(gX, "\ngX RAW: ");
		USART_print_number(gY, "\ngY RAW: ");
		USART_print_number(gZ, "\ngZ RAW: ");
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
  for (;;) {
    // Block until the next release time.
    vTaskDelayUntil(&xLastWakeTime, xDelay);
		GPIO_WriteBit(GPIOB, GPIO_Pin_2, Bit_RESET);
    GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_SET);
  }
}
*/

int main()
{
	init_LED();
	lastRead = 0;	
	USART_Setup();
	USART_Clearscreen();
	I2C_Setup(false);
	/*
	if(!init_MPU())
	{
		GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_SET); //RED
		GPIO_WriteBit(GPIOB, GPIO_Pin_2, Bit_RESET); //GREEN
	}
	*/
	init_MPU();
	MPU_read_all(&aX, &aY, &aZ, &gX, &gY, &gZ);
	MPU_sleep();
	lastRead = aX;
	GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_RESET); //RED
	GPIO_WriteBit(GPIOB, GPIO_Pin_2, Bit_RESET); //GREEN
		
	
    xTaskCreate(vPeriodicTask, "My Task", 256, NULL, 1, NULL);
    //xTaskCreate(vPeriodicTask2, "My Task2", 256, NULL, 2, NULL);

   // Startup of the FreeRTOS scheduler.  The program should block here.
    vTaskStartScheduler();

    // The following line should never be reached.  Failure to allocate enough
    // memory from the heap would be one reason.
    for (;;){
	  	
	  }
}

void init_LED(void)
{
	GPIO_InitTypeDef gpioA;
    GPIO_InitTypeDef gpioB;
    
    GPIO_StructInit(&gpioA);
    GPIO_StructInit(&gpioB);

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    
    gpioA.GPIO_Pin      = GPIO_Pin_2;
    gpioA.GPIO_PuPd     = GPIO_PuPd_NOPULL;
    gpioA.GPIO_Speed    = GPIO_Speed_Level_2;
    gpioA.GPIO_Mode     = GPIO_Mode_OUT;
    
    gpioB.GPIO_Pin	    = GPIO_Pin_2;
    gpioB.GPIO_PuPd     = GPIO_PuPd_NOPULL;
    gpioB.GPIO_Speed	= GPIO_Speed_Level_2;
    gpioB.GPIO_Mode     = GPIO_Mode_OUT;

    GPIO_Init(GPIOA, &gpioA);
    GPIO_Init(GPIOB, &gpioB);
}

uint8_t check_comm(void)
{
	if(I2C_CommStatus == I2C_COMM_OK)
		{
			GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_RESET); //RED
			GPIO_WriteBit(GPIOB, GPIO_Pin_2, Bit_SET); //GREEN
			return 0;
		} else
		{
			GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_SET); //RED
			GPIO_WriteBit(GPIOB, GPIO_Pin_2, Bit_RESET); //GREEN
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
