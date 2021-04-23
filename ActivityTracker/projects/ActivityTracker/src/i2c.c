#include "i2c.h"

uint32_t I2CCommStatus = I2C_COMM_OK;

void I2CSetup(bool intSetup)
{
	GPIO_InitTypeDef gpioB;
	I2C_InitTypeDef I2C;

	// Set I2C1 clock to SYSCLK (see system_stm32f0.c)
	RCC_I2CCLKConfig(RCC_I2C1CLK_SYSCLK);

	// Enable peripheral clock using RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2Cx, ENABLE)
	// function for I2C1 or I2C2.
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

	// Enable SDA, SCL and SMBA (when used) GPIO clocks using 
	// RCC_AHBPeriphClockCmd() function. 
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

	// Peripherals alternate function: 
	// Connect the pin to the desired peripherals' Alternate 
	// Function (AF) using GPIO_PinAFConfig() function.
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_1);

	// Configure the desired pin in alternate function by:
	// GPIO_InitStruct->GPIO_Mode = GPIO_Mode_AF
	GPIO_StructInit(&gpioB);
	gpioB.GPIO_Mode		= GPIO_Mode_AF;
	gpioB.GPIO_Pin		= GPIO_Pin_6 | GPIO_Pin_7;
	gpioB.GPIO_PuPd		= GPIO_PuPd_UP;
	gpioB.GPIO_OType	= GPIO_OType_OD;
	gpioB.GPIO_Speed	= GPIO_Speed_50MHz;

	// Call GPIO_Init() function.
	GPIO_Init(GPIOB, &gpioB);

	// Program the Mode, Timing , Own address, Ack and Acknowledged Address 
	// using the I2C_Init() function.
	I2C_StructInit(&I2C);
	I2C.I2C_Ack					= I2C_Ack_Enable;
	I2C.I2C_AcknowledgedAddress	= I2C_AcknowledgedAddress_7bit;
	I2C.I2C_AnalogFilter		= I2C_AnalogFilter_Enable;
	I2C.I2C_DigitalFilter		= 0;
	I2C.I2C_Mode				= I2C_Mode_I2C;
	I2C.I2C_OwnAddress1			= 0;
	// I2C.I2C_Timing			= 0x00310309; // ~400 kHz. @ 8 MHz (HSI) see Ref. Man. Table 91
	// I2C.I2C_Timing			= 0x50330309; // ~400 kHz. @ 48 MHz (SYSCLK) see Ref. Man. Table 93
	// I2C.I2C_Timing			= 0x2033030A; // =400 kHz. @ 48 MHz (SYSCLK) measured with Logic Analyzer
	I2C.I2C_Timing				= 0xB0420F13; // =100 kHz. @ 48 MHz (SYSCLK) See Table 93

	I2C_Init(I2C1, &I2C);
	
	if(intSetup)
	{
		I2CIntSetup();
	}
	
	// Enable the I2C using the I2C_Cmd() function.
	I2C_Cmd(I2C1, ENABLE);
}

void I2CIntSetup(void)
{
	NVIC_InitTypeDef nvic;
	
	//Enable the I2C global Interrupt
	nvic.NVIC_IRQChannel			= I2C1_IRQn;
	nvic.NVIC_IRQChannelPriority	= 0;
	nvic.NVIC_IRQChannelCmd			= ENABLE;
	
	NVIC_Init(&nvic);
	
	//(#) Enable the corresponding interrupt 
	I2C_ITConfig(I2C1, I2C1_IRQn, ENABLE);
}

void I2CWriteData(uint16_t hardwareAddr, uint8_t amountOfBytes, uint16_t addr, uint8_t data)
{
	// Wait while I2C peripheral is not ready
	I2CWaitForI2CFlag(I2C_ISR_BUSY);

	// Start I2C write transfer for "amountOfBytes" bytes
	I2C_TransferHandling(I2C1, hardwareAddr, amountOfBytes, I2C_AutoEnd_Mode, I2C_Generate_Start_Write);
	I2CWaitForI2CFlag(I2C_ISR_TXIS);

	// Write I2C device address
	I2C_SendData(I2C1, addr);			 
	I2CWaitForI2CFlag(I2C_ISR_TXIS);

	// Write data
	I2C_SendData(I2C1, data);

	// Wait for- and clear stop condition
	I2CWaitForI2CFlag(I2C_ISR_STOPF);
	I2C1->ICR = I2C_ICR_STOPCF;
}

uint16_t I2CReadData(uint16_t hardwareAddr, uint8_t amountOfBytes, uint16_t addr)
{
	uint8_t data = 0x00;

	// Wait while I2C peripheral is not ready
	I2CWaitForI2CFlag(I2C_ISR_BUSY);

	// Start I2C write transfer for 1 byte(hardware address), do not end transfer (SoftEnd_Mode)
	I2C_TransferHandling(I2C1, hardwareAddr, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);
	I2CWaitForI2CFlag(I2C_ISR_TXIS);

	// Write I2C device address
	I2C_SendData(I2C1, addr);

	// Repeated start I2C read transfer for "amountOfBytes" byte
	I2C_TransferHandling(I2C1, hardwareAddr, amountOfBytes, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);
	I2CWaitForI2CFlag(I2C_ISR_RXNE);
	
	// Read data
	data = I2C_ReceiveData(I2C1);
	
	// Wait for- and clear stop condition
	I2CWaitForI2CFlag(I2C_ISR_STOPF);

	I2C1->ICR = I2C_ICR_STOPCF;
	
	return(data);
}

void I2CWaitForI2CFlag(uint32_t flag)
{
	uint32_t timeout = I2C_TIMEOUT;

	if(flag == I2C_ISR_BUSY)
	{
		while(I2C_GetFlagStatus(I2C1, flag) != RESET)
		{
			if(timeout-- == 0)
			{
				I2CCommStatus = I2C_COMM_ERROR;
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
				I2CCommStatus = I2C_COMM_ERROR;
				return;
			}
		}
	}
}
