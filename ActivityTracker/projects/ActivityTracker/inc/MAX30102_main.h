#ifndef __MAX30102_main_H
#define __MAX30102_main_H

#include "stm32f0xx.h"
#include "I2C_main.h"

#define MAX30102_ADDRESS 0xae

#define MAX30102_INTERRUPT_STATUS_ALMOST_FULL 0x80
#define MAX30102_INTERRUPT_STATUS_NEW_FIFO_DATA 0x40
#define MAX30102_INTERRUPT_STATUS_ALC_OVF 0x20
#define MAX30102_INTERRUPT_STATUS_PROX_INT 0x10
#define MAX30102_INTERRUPT_STATUS_TEMPERATURE_READY 0x02
#define MAX30102_INTERRUPT_STATUS_POWER_READY 0x01

#define RES_INTERRUPT_STATUS_1 0x00
#define RES_INTERRUPT_STATUS_2 0x01
#define RES_INTERRUPT_ENABLE_1 0x02
#define RES_INTERRUPT_ENABLE_2 0x03
#define RES_FIFO_WRITE_POINTER 0x04
#define RES_OVERFLOW_COUNTER 0x05
#define RES_FIFO_READ_POINTER 0x06
#define RES_FIFO_DATA_REGISTER 0x07
#define RES_FIFO_CONFIGURATION 0x08
#define RES_MODE_CONFIGURATION 0x09
#define RES_SPO2_CONFIGURATION 0x0a
#define RES_LED_PLUSE_AMPLITUDE_1 0x0c
#define RES_LED_PLUSE_AMPLITUDE_2 0x0d
#define RES_PROXIMITY_MODE_LED_PLUSE_AMPLITUDE 0x10
#define RES_MULTI_LED_MODE_CONTROL_1 0x11
#define RES_MULTI_LED_MODE_CONTROL_2 0x12
#define RES_DIE_TEMP_INTEGER 0x1f
#define RES_DIE_TEMP_FRACTION 0x20
#define RES_DIE_TEMPERATURE_CONFIG 0x21
#define RES_PROXIMITY_INTERRUPT_THRESHOLD 0x30
#define RES_REVISION_ID 0xfe
#define RES_PART_ID 0xff

#define BUFF_SIZE 50
#define FILTER_LEVEL 8

typedef struct samplestruct
{
    uint32_t red;
    uint32_t iRed;
} SAMPLE;

void write_MAX(uint8_t addr, uint8_t data);
uint8_t read_MAX(uint8_t addr);
void max30102_init(void);
uint8_t max30102_getUnreadSampleCount(void);
void max30102_getFIFO(SAMPLE *data, uint8_t sampleCount);
uint8_t max30102_getStatus(void);
// float max30102_getTemperature();
void max30102_OFF(void);
void max30102_ON(void);

void max30102_cal(void);
uint8_t max30102_getHeartRate(void);
uint8_t max30102_getSpO2(void);
int16_t max30102_getDiff(void);

#endif //__MAX30102_main_H
