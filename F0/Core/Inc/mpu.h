#ifndef __MPU_H_
#define __MPU_H_

#include "i2c.h"
#include "stdbool.h"

#define MPU6050_ADDR 0xD0


#define SMPLRT_DIV_REG 0x19
#define GYRO_CONFIG_REG 0x1B
#define ACCEL_CONFIG_REG 0x1C
#define ACCEL_XOUT_H_REG 0x3B
#define TEMP_OUT_H_REG 0x41
#define GYRO_XOUT_H_REG 0x43
#define PWR_MGMT_1_REG 0x6B
#define WHO_AM_I_REG 0x75

#define MPU_A2G		0x00
#define MPU_A4G		0x08 //0b00001000
#define MPU_A8G		0x10 //0b00010000
#define MPU_A16G	0x18 //0b00011000
#define MPU_G250G	0x00
#define MPU_G500G	0x08 //0b00001000
#define MPU_G1000G	0x10 //0b00010000
#define MPU_G2000G	0x18 //0b00011000

bool MPU6050Init();
void MPU6050ReadAccel(uint16_t *aXRaw, uint16_t *aYRaw, uint16_t *aZRaw);
void MPU6050ReadGyro(uint16_t *gXRaw, uint16_t *gYRaw, uint16_t *gZRaw);
void MPUReadAll(uint16_t *aXRaw, uint16_t *aYRaw, uint16_t *aZRaw, uint16_t *gXRaw, uint16_t *gYRaw, uint16_t *gZRaw);
void MPUSetAccel(uint8_t acceleration);
void MPUSetGyro(uint8_t gyroSpeed);

#endif //__MPU_H_
