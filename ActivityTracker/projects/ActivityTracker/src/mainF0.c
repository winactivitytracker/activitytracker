#include "stm32f0xx.h"
#include "stdbool.h"
#include "stm32f0xx_i2c.h"
#include <FreeRTOS.h>
#include <task.h>

#define ONE_TENTHOUSEND_SECOND  		(SystemCoreClock/8/10000)
#define ONE_THOUSEND_SECOND  				(SystemCoreClock/8/1000)
#define ONE_HUNDERDTH_SECOND  			(SystemCoreClock/8/100)
#define ONE_FIFTYTH_SECOND  				(SystemCoreClock/8/50)
#define ONE_TENTH_SECOND  					(SystemCoreClock/8/10)
#define HALF_SECOND   							(SystemCoreClock/8/5)
#define ONE_SECOND         					(SystemCoreClock/8)

#define MPU_ADDRESS 0xD0 //0x68
#define MPU_6050_TIMEOUT            ((uint32_t)(0x00001000))

#define MPU_6050_COMM_OK            (0x0)
#define MPU_6050_COMM_ERROR         (0x1)

uint32_t MPU_6050_CommStatus = MPU_6050_COMM_OK;

uint8_t readGyro[6];
uint16_t gyroXRAW, gZ;

bool enable_MPU(void);

void I2C_Setup(void);
void MPU6050_WriteData(uint16_t addr, uint8_t data);
uint16_t MPU6050_ReadData(uint16_t addr);
void delay(const int d);
void MPU_WaitForI2CFlag(uint32_t flag);

void vPeriodicTask(void *pvParameters)
{
  // Establish the task's period.
  const TickType_t xDelay = pdMS_TO_TICKS(1000);
  TickType_t xLastWakeTime = xTaskGetTickCount();
  for (;;) {
    // Block until the next release time.
    vTaskDelayUntil(&xLastWakeTime, xDelay);
		readGyro[0] = MPU6050_ReadData(0x43);
		readGyro[1] = MPU6050_ReadData(0x44);
		gyroXRAW = (int16_t)(readGyro[0] << 8 | readGyro [1]);
		gZ = gyroXRAW/16384.0;
		if(MPU_6050_CommStatus == MPU_6050_COMM_OK)
		{
			GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_RESET); //RED
			GPIO_WriteBit(GPIOB, GPIO_Pin_2, Bit_SET); //GREEN
		} else
		{
			GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_SET); //RED
			GPIO_WriteBit(GPIOB, GPIO_Pin_2, Bit_RESET); //GREEN
		}
		
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
		
		I2C_Setup();
		
		if(enable_MPU())
		{
			MPU6050_WriteData(0x6B, 0x00);
			MPU6050_WriteData(0x19, 0x07);
			MPU6050_WriteData(0x1B, 0x00);
			MPU6050_WriteData(0x1C, 0x00);
		}
		delay(HALF_SECOND);
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

bool enable_MPU(void)
{
	if(MPU6050_ReadData(0x75) == 0x68)
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

void I2C_Setup(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  I2C_InitTypeDef  I2C_InitStructure;
	NVIC_InitTypeDef        NVIC_InitStructure;
  
  // Set I2C1 clock to SYSCLK (see system_stm32f0.c)
  RCC_I2CCLKConfig(RCC_I2C1CLK_SYSCLK);

  //(#) Enable peripheral clock using RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2Cx, ENABLE)
  //    function for I2C1 or I2C2.
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
  
  //(#) Enable SDA, SCL  and SMBA (when used) GPIO clocks using 
  //    RCC_AHBPeriphClockCmd() function. 
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  
  //(#) Peripherals alternate function: 
  //    (++) Connect the pin to the desired peripherals' Alternate 
  //         Function (AF) using GPIO_PinAFConfig() function.
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_1);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_1);

  //    (++) Configure the desired pin in alternate function by:
  //         GPIO_InitStruct->GPIO_Mode = GPIO_Mode_AF
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;

  //    (++) Select the type, OpenDrain and speed via  
  //         GPIO_PuPd, GPIO_OType and GPIO_Speed members
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
  //    (++) Call GPIO_Init() function.
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  //(#) Program the Mode, Timing , Own address, Ack and Acknowledged Address 
  //    using the I2C_Init() function.
  I2C_StructInit(&I2C_InitStructure);
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
  I2C_InitStructure.I2C_DigitalFilter = 0;
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_OwnAddress1 = 0;
//I2C_InitStructure.I2C_Timing = 0x00310309; // ~400 kHz. @ 8 MHz (HSI) see Ref. Man. Table 91
//I2C_InitStructure.I2C_Timing = 0x50330309; // ~400 kHz. @ 48 MHz (SYSCLK) see Ref. Man. Table 93
//  I2C_InitStructure.I2C_Timing = 0x2033030A; // =400 kHz. @ 48 MHz (SYSCLK) measured with Logic Analyzer
  I2C_InitStructure.I2C_Timing = 0xB0420F13; // =100 kHz. @ 48 MHz (SYSCLK) See Table 93

  I2C_Init(I2C1, &I2C_InitStructure);
  
  //(#) Optionally you can enable/configure the following parameters without
  //    re-initialization (i.e there is no need to call again I2C_Init() function):
  //    (++) Enable the acknowledge feature using I2C_AcknowledgeConfig() function.
  //    (++) Enable the dual addressing mode using I2C_DualAddressCmd() function.
  //    (++) Enable the general call using the I2C_GeneralCallCmd() function.
  //    (++) Enable the clock stretching using I2C_StretchClockCmd() function.
  //    (++) Enable the PEC Calculation using I2C_CalculatePEC() function.
  //    (++) For SMBus Mode:
  //         (+++) Enable the SMBusAlert pin using I2C_SMBusAlertCmd() function.

  //(#) Enable the NVIC and the corresponding interrupt using the function
  //    I2C_ITConfig() if you need to use interrupt mode.
 
  //Enable the I2C global Interrupt
  //NVIC_InitStructure.NVIC_IRQChannel         = I2C1_IRQn;
  //NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  //NVIC_InitStructure.NVIC_IRQChannelCmd      = ENABLE;
  //NVIC_Init(&NVIC_InitStructure);
  
  //(#) Enable the corresponding interrupt 
  //I2C_ITConfig(I2C1, I2C1_IRQn, ENABLE);
  
  //(#) When using the DMA mode 
  //   (++) Configure the DMA using DMA_Init() function.
  //   (++) Active the needed channel Request using I2C_DMACmd() function.
  
  //(#) Enable the I2C using the I2C_Cmd() function.
  I2C_Cmd(I2C1, ENABLE);

  //(#) Enable the DMA using the DMA_Cmd() function when using DMA mode in the 
  //    transfers.
}

void MPU6050_WriteData(uint16_t addr, uint8_t data)
{
  // Wait while I2C peripheral is not ready
  MPU_WaitForI2CFlag( I2C_ISR_BUSY );
	//delay(ONE_THOUSEND_SECOND);

  // Start I2C write transfer for 3 bytes
  I2C_TransferHandling( I2C1, MPU_ADDRESS, 2, I2C_AutoEnd_Mode, I2C_Generate_Start_Write );
  MPU_WaitForI2CFlag( I2C_ISR_TXIS );
	//delay(ONE_THOUSEND_SECOND);
	
  // Write eeprom address
  I2C_SendData(I2C1, addr >> 8);       // Address High byte
  MPU_WaitForI2CFlag(I2C_ISR_TXIS);
	//delay(ONE_THOUSEND_SECOND);
  I2C_SendData(I2C1, addr & 0x00FF);   // Address Low byte
  MPU_WaitForI2CFlag(I2C_ISR_TXIS);
	//delay(ONE_THOUSEND_SECOND);
	
  // Write data
  I2C_SendData(I2C1, data);
    
  // Wait for- and clear stop condition
	//delay(ONE_THOUSEND_SECOND);
  MPU_WaitForI2CFlag(I2C_ISR_STOPF);
  I2C_ClearFlag(I2C1, I2C_ICR_STOPCF);
}

uint16_t MPU6050_ReadData(uint16_t addr)
{
  uint8_t data = 0x00;
	
  // Wait while I2C peripheral is not ready
  MPU_WaitForI2CFlag(I2C_ISR_BUSY);
	//delay(ONE_THOUSEND_SECOND);

  // Start I2C write transfer for 2 bytes, do not end transfer (SoftEnd_Mode)
  I2C_TransferHandling(I2C1, MPU_ADDRESS, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);
  MPU_WaitForI2CFlag(I2C_ISR_TXIS);
	//delay(ONE_THOUSEND_SECOND);

  // Write eeprom address
  I2C_SendData(I2C1, addr);       // Address High byte
	//delay(ONE_THOUSEND_SECOND);
  // Repeated start I2C read transfer for 1 byte
  I2C_TransferHandling(I2C1, MPU_ADDRESS, 2, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);
  MPU_WaitForI2CFlag(I2C_ISR_RXNE);
	//delay(ONE_THOUSEND_SECOND);
  
  // Read data
  data = I2C_ReceiveData(I2C1);
    
  // Wait for- and clear stop condition
  MPU_WaitForI2CFlag(I2C_ISR_STOPF);
	//delay(ONE_THOUSEND_SECOND);
  I2C1->ICR = I2C_ICR_STOPCF;
  
  return( data );
}

void MPU_WaitForI2CFlag(uint32_t flag)
{
  uint32_t timeout = MPU_6050_TIMEOUT;
  
  if(flag == I2C_ISR_BUSY)
  {
    while(I2C_GetFlagStatus(I2C1, flag) != RESET)
    {
      if(timeout-- == 0)
      {
        MPU_6050_CommStatus = MPU_6050_COMM_ERROR;
        return;
      }
    }
  }
  else
  {
    while(I2C_GetFlagStatus(I2C1, flag) == RESET)
    {
      if(timeout-- == 0)
      {
        MPU_6050_CommStatus = MPU_6050_COMM_ERROR;
        return;
      }
    }
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
