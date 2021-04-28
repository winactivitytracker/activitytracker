#include "max30102.h"
#include "i2c.h"

void max30102_init(void)
{
    uint8_t data = 0;
    /*reset*/
    data = 0x40;
    HAL_I2C_Mem_Write(&hi2c1, MAX30102_ADDR_WRITE, RES_MODE_CONFIGURATION, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
    do
    {
        HAL_I2C_Mem_Read(&hi2c1, MAX30102_ADDR_READ, RES_MODE_CONFIGURATION, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
    } while (data & 0x40);
	
    data = 0x40;
    HAL_I2C_Mem_Write(&hi2c1, MAX30102_ADDR_WRITE, RES_INTERRUPT_ENABLE_1, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
    
    data = 0x63;
    HAL_I2C_Mem_Write(&hi2c1, MAX30102_ADDR_WRITE, RES_SPO2_CONFIGURATION, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
    
    data = 0x47;
    HAL_I2C_Mem_Write(&hi2c1, MAX30102_ADDR_WRITE, RES_LED_PLUSE_AMPLITUDE_1, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
    HAL_I2C_Mem_Write(&hi2c1, MAX30102_ADDR_WRITE, RES_LED_PLUSE_AMPLITUDE_2, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
    HAL_I2C_Mem_Write(&hi2c1, MAX30102_ADDR_WRITE, RES_PROXIMITY_MODE_LED_PLUSE_AMPLITUDE, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
    /*FIFO clear*/
    data = 0;
    HAL_I2C_Mem_Write(&hi2c1, MAX30102_ADDR_WRITE, RES_FIFO_WRITE_POINTER, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
    HAL_I2C_Mem_Write(&hi2c1, MAX30102_ADDR_WRITE, RES_OVERFLOW_COUNTER, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
    HAL_I2C_Mem_Write(&hi2c1, MAX30102_ADDR_WRITE, RES_FIFO_READ_POINTER, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
    /*interrupt status clear*/
    max30102_getStatus();
    
    // data = 1;
    // HAL_I2C_Mem_Write(&hi2c1, MAX30102_ADDR_WRITE, RES_DIE_TEMPERATURE_CONFIG, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
    /*SPO2 Mode*/
    data = 0x03;
    HAL_I2C_Mem_Write(&hi2c1, MAX30102_ADDR_WRITE, RES_MODE_CONFIGURATION, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
}

uint8_t max30102_getUnreadSampleCount(void)
{
    uint8_t wr = 0, rd = 0;
    HAL_I2C_Mem_Read(&hi2c1, MAX30102_ADDR_READ, RES_FIFO_WRITE_POINTER, I2C_MEMADD_SIZE_8BIT, &wr, 1, 10);
    HAL_I2C_Mem_Read(&hi2c1, MAX30102_ADDR_READ, RES_FIFO_READ_POINTER, I2C_MEMADD_SIZE_8BIT, &rd, 1, 10);
    if ((wr - rd) < 0)
        return wr - rd + 32;
    else
        return wr - rd;
}

void max30102_getFIFO(SAMPLE *data, uint8_t sampleCount)
{
    uint8_t dataTemp[5 * 6];
    if (sampleCount > 5)
        sampleCount = 5;
    HAL_I2C_Mem_Read(&hi2c1, MAX30102_ADDR_READ, RES_FIFO_DATA_REGISTER,
                     I2C_MEMADD_SIZE_8BIT, dataTemp,
                     6 * sampleCount, 25);
    uint8_t i;
    for (i = 0; i < sampleCount; i++)
    {
        data[i].red = (((uint32_t)dataTemp[i * 6]) << 16 | ((uint32_t)dataTemp[i * 6 + 1]) << 8 | dataTemp[i * 6 + 2]) & 0x3ffff;
        data[i].iRed = (((uint32_t)dataTemp[i * 6 + 3]) << 16 | ((uint32_t)dataTemp[i * 6 + 4]) << 8 | dataTemp[i * 6 + 5]) & 0x3ffff;
    }
}

uint8_t max30102_getStatus(void)
{
    uint8_t data = 0, dataTemp = 0;
    HAL_I2C_Mem_Read(&hi2c1, MAX30102_ADDR_READ, RES_INTERRUPT_STATUS_1, I2C_MEMADD_SIZE_8BIT, &dataTemp, 1, 10);
    data = dataTemp;
    HAL_I2C_Mem_Read(&hi2c1, MAX30102_ADDR_READ, RES_INTERRUPT_STATUS_2, I2C_MEMADD_SIZE_8BIT, &dataTemp, 1, 10);
    return data | dataTemp;
}

void max30102_OFF(void)
{
    uint8_t data = 0;
    HAL_I2C_Mem_Read(&hi2c1, MAX30102_ADDR_READ, RES_MODE_CONFIGURATION, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
    data |= 0x80;
    HAL_I2C_Mem_Write(&hi2c1, MAX30102_ADDR_WRITE, RES_MODE_CONFIGURATION, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
}

void max30102_ON()
{
    uint8_t data = 0;
    HAL_I2C_Mem_Read(&hi2c1, MAX30102_ADDR_READ, RES_MODE_CONFIGURATION, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
    data &= ~(0x80);
    HAL_I2C_Mem_Write(&hi2c1, MAX30102_ADDR_WRITE, RES_MODE_CONFIGURATION, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
}

/******************************??******************************/
