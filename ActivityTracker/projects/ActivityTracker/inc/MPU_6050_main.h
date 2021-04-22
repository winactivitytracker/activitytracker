#ifndef __MPU_6050_main_h
#define __MPU_6050_main_h

#include "I2C_main.h"

#define MPU_ADDRESS 0xD0 //0x68
#define MPU_A2G 0x00
#define MPU_A4G 0x08	//0b00001000
#define MPU_A8G 0x10	//0b00010000
#define MPU_A16G 0x18	//0b00011000
#define MPU_G250G 0x00
#define MPU_G500G 0x08	//0b00001000
#define MPU_G1000G 0x10	//0b00010000
#define MPU_G2000G 0x18 //0b00011000


bool init_MPU(void);
bool MPU_available(void);
uint16_t MPU_read_data(uint16_t addr);
void MPU_read_accel(uint16_t *AXRAW, uint16_t *AYRAW, uint16_t *AZRAW);
void MPU_read_gyro(uint16_t *GXRAW, uint16_t *GYRAW, uint16_t *GZRAW);
void MPU_read_all(uint16_t *AXRAW, uint16_t *AYRAW, uint16_t *AZRAW, uint16_t *GXRAW, uint16_t *GYRAW, uint16_t *GZRAW);
void MPU_set_accel(uint8_t acceleration);
void MPU_set_gyro(uint8_t gyroSpeed);
void MPU_sleep(void);
void MPU_wakeup(void);


#endif //__MPU_6050_main_h
