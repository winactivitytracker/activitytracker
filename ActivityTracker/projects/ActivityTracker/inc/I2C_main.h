#ifndef __I2C_main_h
#define __I2C_main_h

#include "stdbool.h"
#include "stm32f0xx.h"

#define I2C_TIMEOUT            ((uint32_t)(0x00001000))
#define I2C_COMM_OK            (0x0)
#define I2C_COMM_ERROR         (0x1)

void I2C_Setup(bool intSetup);
void I2C_int_setup(void);
void I2C_WriteData(uint16_t hardwareaddr, uint8_t amountOfBytes, uint16_t addr, uint8_t data);
uint16_t I2C_ReadData(uint16_t hardwareaddr, uint8_t amountOfBytes, uint16_t addr);
void I2C_WaitForI2CFlag(uint32_t flag);
uint8_t check_comm(void);

#endif //__I2C_main_h
