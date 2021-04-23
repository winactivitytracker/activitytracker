#ifndef MPU_6050_MAIN_H
#define MPU_6050_MAIN_H

#include "i2c.h"

#define MPU_ADDRESS	0xD0 //0x68
#define MPU_A2G		0x00
#define MPU_A4G		0x08 //0b00001000
#define MPU_A8G		0x10 //0b00010000
#define MPU_A16G	0x18 //0b00011000
#define MPU_G250G	0x00
#define MPU_G500G	0x08 //0b00001000
#define MPU_G1000G	0x10 //0b00010000
#define MPU_G2000G	0x18 //0b00011000

bool MPUInit(void);
bool MPUAvailable(void);
uint16_t MPUReadData(uint16_t addr);
void MPUReadAccel(uint16_t *aXRaw, uint16_t *aYRaw, uint16_t *aZRaw);
void MPUReadGyro(uint16_t *gXRaw, uint16_t *gYRaw, uint16_t *gZRaw);
void MPUReadAll(uint16_t *aXRaw, uint16_t *aYRaw, uint16_t *aZRaw, uint16_t *gXRaw, uint16_t *gYRaw, uint16_t *gZRaw);
void MPUSetAccel(uint8_t acceleration);
void MPUSetGyro(uint8_t gyroSpeed);
void MPUSleep(void);
void MPUWakeup(void);


#endif // MPU_6050_MAIN_H
