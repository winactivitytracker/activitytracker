#ifndef __MPU_6050_main_h
#define __MPU_6050_main_h

#include "I2C_main.h"

#define MPU_ADDRESS 0xD0 //0x68

bool init_MPU(void);
bool MPU_available(void);
uint16_t MPU_read_data(uint16_t addr);
void MPU_read_accel(uint16_t *AXRAW, uint16_t *AYRAW, uint16_t *AZRAW);
void MPU_read_gyro(uint16_t *GXRAW, uint16_t *GYRAW, uint16_t *GZRAW);
void MPU_read_all(uint16_t *AXRAW, uint16_t *AYRAW, uint16_t *AZRAW, uint16_t *GXRAW, uint16_t *GYRAW, uint16_t *GZRAW);


#endif //__MPU_6050_main_h
