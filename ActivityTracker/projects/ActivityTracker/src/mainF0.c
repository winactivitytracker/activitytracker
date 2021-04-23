#include "stm32f0xx.h"
#include "stm32f0xx_it.h"
#include "main.h"
#include <FreeRTOS.h>
#include <task.h>

extern uint32_t I2C_CommStatus;
extern SAMPLE sampleBuff[BUFF_SIZE];

void delay(const int d);
void init_LED(void);

void initTimer14(void);
void initTimer14Interrupt(void);

char UartBuffer[32];

uint16_t aX, aY, aZ, gX, gY, gZ, lastRead;

void vPeriodicTask(void *pvParameters)
{
	uint8_t heartRate, Spo2;
    // Establish the task's period.
    const TickType_t xDelay = pdMS_TO_TICKS(1000);
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for (;;) {
        // Block until the next release time.
        vTaskDelayUntil(&xLastWakeTime, xDelay);
		//sprintf(UartBuffer, "%i", max30102_getHeartRate());
		//max30102_ON();
		max30102_cal();
		heartRate = max30102_getHeartRate();
		Spo2 = max30102_getSpO2();
		Spo2 = Spo2 % 10;
		heartRate = heartRate % 10;
		USART_print_number(heartRate, "\nHeartRate: ");
		USART_print_number(Spo2, "\nOxy: ");
		//max30102_OFF();
		
		
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
	USART_Setup();
	USART_Clearscreen();
	I2C_Setup(false);
	initTimer14();
	initTimer14Interrupt();
	
	max30102_init();
	//USART_print_number(I2C_ReadData(0xae, 1, 0x09), "\nMAX Number: ");
	
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
    
    gpioB.GPIO_Pin	    = GPIO_Pin_2 | GPIO_Pin_3;
    gpioB.GPIO_PuPd     = GPIO_PuPd_NOPULL;
    gpioB.GPIO_Speed	= GPIO_Speed_Level_2;
    gpioB.GPIO_Mode     = GPIO_Mode_OUT;

    GPIO_Init(GPIOA, &gpioA);
    GPIO_Init(GPIOB, &gpioB);
}

void initTimer14(void)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef       TIM_OCInitStructure;
	
  //(#) Enable TIM clock
  RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM14EN, ENABLE);
  
  //100 * 10Hz = 1000 for the prescaler division
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period        = 10;													//period was chosen for easy calculation
  TIM_TimeBaseStructure.TIM_Prescaler     = (SystemCoreClock / 30000000) - 1; //prescaler calculation = ((period * preferred frequency) / SystemCoreClock) - 1
  TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure);
  
  TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse       = 0;
  TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_High;
  
  //(#) Call TIM_OCxInit(TIMx, &TIM_OCInitStruct) to configure the desired 
  //    channel with the corresponding configuration.
  TIM_OC1Init(TIM14, &TIM_OCInitStructure);

  //(#) Call the TIM_Cmd(ENABLE) function to enable the TIM counter.
  TIM_Cmd(TIM14, ENABLE);	
}

//initialize the interrupt for the ADC
void initTimer14Interrupt(void)
{
	NVIC_InitTypeDef        NVIC_InitStructure;
	
	//Enable the TIM2 global Interrupt
  NVIC_InitStructure.NVIC_IRQChannel         = TIM14_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd      = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  //(#) Enable the corresponding interrupt 
  TIM_ITConfig(TIM14, TIM_IT_Update, ENABLE);
  
  //(#) Call the TIM_Cmd(ENABLE) function to enable the TIM counter.
  TIM_Cmd(TIM14, ENABLE);
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
