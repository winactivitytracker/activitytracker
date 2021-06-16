#ifndef __MPU_H_
#define __MPU_H_

#include <stdio.h>
#include <stdbool.h>

#include "i2c.h"
#include "usart.h"
#include "rtc.h"

#define MPU6050_ADDR 0xD0

#define SMPLRT_DIV_REG 0x19
#define SAMPLERATE1KHZ 0x07
#define GYRO_CONFIG_REG 0x1B
#define ACCEL_CONFIG_REG 0x1C
#define ACCEL_XOUT_H_REG 0x3B
#define TEMP_OUT_H_REG 0x41
#define GYRO_XOUT_H_REG 0x43
#define PWR_MGMT_1_REG 0x6B
#define WHO_AM_I_REG 0x75

#define MPU_A2G		0x00 // 00000000
#define MPU_A4G		0x08 // 00001000
#define MPU_A8G		0x10 // 00010000
#define MPU_A16G	0x18 // 00011000
#define MPU_G250G	0x00 // 00000000
#define MPU_G500G	0x08 // 00001000
#define MPU_G1000G	0x10 // 00010000
#define MPU_G2000G	0x18 // 00011000

bool MPU6050Init(void);
void MPU6050ReadAccel(int16_t *aXRaw, int16_t *aYRaw, int16_t *aZRaw);
void MPU6050ReadGyro(int16_t *gXRaw, int16_t *gYRaw, int16_t *gZRaw);
void MPUReadAll(int16_t *aXRaw, int16_t *aYRaw, int16_t *aZRaw, int16_t *gXRaw, int16_t *gYRaw, int16_t *gZRaw);
uint8_t MPUSetAccel(uint8_t acceleration);
uint8_t MPUSetGyro(uint8_t gyroSpeed);
void MPUToUsartInit();
void MPUToUsartLoop();

#endif //__MPU_H_
