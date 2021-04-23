#ifndef I2C_MAIN_H
#define I2C_MAIN_H

#include "stdbool.h"
#include "stm32f0xx.h"

#define I2C_TIMEOUT		((uint32_t)(0x00001000))
#define I2C_COMM_OK		(0x0)
#define I2C_COMM_ERROR	(0x1)

void I2CSetup(bool intSetup);
void I2CIntSetup(void);
void I2CWriteData(uint16_t hardwareAddr, uint8_t amountOfBytes, uint16_t addr, uint8_t data);
uint16_t I2CReadData(uint16_t hardwareAddr, uint8_t amountOfBytes, uint16_t addr);
void I2CWaitForI2CFlag(uint32_t flag);

#endif // I2C_MAIN_H
