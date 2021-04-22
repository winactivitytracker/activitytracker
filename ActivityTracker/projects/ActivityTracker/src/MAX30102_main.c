#include "MAX30102_main.h"

void write_MAX(uint8_t addr, uint8_t data)
{
	I2C_WriteData(MAX30102_ADDRESS, 2, addr, data);
}

uint8_t read_MAX(uint8_t addr)
{
	return I2C_ReadData(MAX30102_ADDRESS, 1, addr);
}

void max30102_init(void)
{
	uint8_t data = 0;
	data = 0x40;
	write_MAX(RES_MODE_CONFIGURATION, data);
	check_comm();
    do
    {
		data = read_MAX(RES_MODE_CONFIGURATION);
    } while (data & 0x40);
	check_comm();
	data = 0x40;
	write_MAX(RES_INTERRUPT_ENABLE_1, data);
	
	data = 0x63;
	write_MAX(RES_SPO2_CONFIGURATION, data);
	
	data = 0x47;
	write_MAX(RES_LED_PLUSE_AMPLITUDE_1, data);
	write_MAX(RES_LED_PLUSE_AMPLITUDE_2, data);
	write_MAX(RES_PROXIMITY_MODE_LED_PLUSE_AMPLITUDE, data);
	check_comm();

	data = 0;
	write_MAX(RES_FIFO_WRITE_POINTER, data);
	write_MAX(RES_OVERFLOW_COUNTER, data);
	write_MAX(RES_FIFO_READ_POINTER, data);
    
	max30102_getStatus();
	
	data = 0x03;
	write_MAX(RES_MODE_CONFIGURATION, data);
}

uint8_t max30102_getUnreadSampleCount(void)
{
    uint8_t wr = 0, rd = 0;
	wr = read_MAX(RES_FIFO_WRITE_POINTER);
	rd = read_MAX(RES_FIFO_READ_POINTER);
    if ((wr - rd) < 0)
        return wr - rd + 32;
    else
        return wr - rd;
}

uint8_t max30102_getStatus(void)
{
    uint8_t data = 0, dataTemp = 0;
	dataTemp = read_MAX(RES_INTERRUPT_STATUS_1);
    
    data = dataTemp;
	dataTemp = read_MAX(RES_INTERRUPT_STATUS_2);
    
    return data | dataTemp;
}

void max30102_OFF(void)
{
    uint8_t data = 0;
	data = read_MAX(RES_MODE_CONFIGURATION);
    //HAL_I2C_Mem_Read(&hi2c2, MAX30102_ADDR_READ, RES_MODE_CONFIGURATION, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
    data |= 0x80;
	write_MAX(RES_MODE_CONFIGURATION, data);
    //HAL_I2C_Mem_Write(&hi2c2, MAX30102_ADDR_WRITE, RES_MODE_CONFIGURATION, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
}

void max30102_ON(void)
{
    uint8_t data = 0;
	data = read_MAX(RES_MODE_CONFIGURATION);
    //HAL_I2C_Mem_Read(&hi2c2, MAX30102_ADDR_READ, RES_MODE_CONFIGURATION, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
    data &= ~(0x80);
	write_MAX(RES_MODE_CONFIGURATION, data);
    //HAL_I2C_Mem_Write(&hi2c2, MAX30102_ADDR_WRITE, RES_MODE_CONFIGURATION, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
}
