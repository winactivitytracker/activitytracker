#include "stm32f0xx.h"
#include "stm32f0xx_it.h"
#include "main.h"
#include "stm32f0xx_i2c.h"
#include "I2C_main.h"
#include "stdbool.h"
#include <FreeRTOS.h>
#include <task.h>

uint8_t readGyro[6];
uint16_t gyroXRAW, gZ;

extern uint32_t I2C_CommStatus;

bool enable_MPU(void);

void delay(const int d);
uint8_t check_comm(void);

void vPeriodicTask(void *pvParameters)
{
  // Establish the task's period.
  const TickType_t xDelay = pdMS_TO_TICKS(100);
  TickType_t xLastWakeTime = xTaskGetTickCount();
  for (;;) {
    // Block until the next release time.
    vTaskDelayUntil(&xLastWakeTime, xDelay);
		readGyro[0] = I2C_ReadData(MPU_ADDRESS, 1, 0x43);
		readGyro[1] = I2C_ReadData(MPU_ADDRESS , 1, 0x44);
		gyroXRAW = (int16_t)(readGyro[0] << 8 | readGyro [1]);
		gZ = gyroXRAW/16384.0;
	    check_comm();
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
		
	I2C_Setup(false);
		
	if(enable_MPU())
	{	
		I2C_WriteData(MPU_ADDRESS, 2, 0x6B, 0x00);
		delay(ONE_THOUSEND_SECOND);
		check_comm();
		I2C_WriteData(MPU_ADDRESS ,2, 0x19, 0x07);
		delay(ONE_THOUSEND_SECOND);
		check_comm();
		I2C_WriteData(MPU_ADDRESS, 2, 0x1B, 0x00);
		delay(ONE_THOUSEND_SECOND);
		check_comm();
		I2C_WriteData(MPU_ADDRESS, 2, 0x1C, 0x00);
		delay(ONE_THOUSEND_SECOND);
		check_comm();
	}
	delay(HALF_SECOND);
	GPIO_WriteBit(GPIOB, GPIO_Pin_2, Bit_RESET); //GREEN
	delay(HALF_SECOND);
		
	
    xTaskCreate(vPeriodicTask, "My Task", 256, NULL, 1, NULL);
    //xTaskCreate(vPeriodicTask2, "My Task2", 256, NULL, 2, NULL);

   // Startup of the FreeRTOS scheduler.  The program should block here.
    vTaskStartScheduler();

    // The following line should never be reached.  Failure to allocate enough
    // memory from the heap would be one reason.
    for (;;){
	  	
	  }
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

bool enable_MPU(void)
{
	if(I2C_ReadData(MPU_ADDRESS, 1, 0x75) == 0x68)
		{
			GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_RESET); //RED
			GPIO_WriteBit(GPIOB, GPIO_Pin_2, Bit_SET); //GREEN
			return true;
		} else
		{
			GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_SET); //RED
			GPIO_WriteBit(GPIOB, GPIO_Pin_2, Bit_RESET); //GREEN
			return false;
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
