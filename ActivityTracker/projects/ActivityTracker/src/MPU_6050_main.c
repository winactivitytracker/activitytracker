#include "MPU_6050_main.h"


bool init_MPU(void)
{
	if(MPU_available())
	{
		MPU_wakeup();								//wake up
		I2C_WriteData(MPU_ADDRESS ,2, 0x19, 0x07);	//set speed at 1Khz
		MPU_set_accel(2);							//standard setup, Accel at 2G
		MPU_set_gyro(2);							//standard setup, Gyro at 250 D/S
		return true;
	} else
	{
		return false;
	}
}

bool MPU_available(void)
{
	if(I2C_ReadData(MPU_ADDRESS, 1, 0x75) == 0x68)
		{
			GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_RESET); //RED
			GPIO_WriteBit(GPIOB, GPIO_Pin_2, Bit_SET); //GREEN
			return true;
		} else
		{
			GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_SET); //RED
			GPIO_WriteBit(GPIOB, GPIO_Pin_2, Bit_RESET); //GREEN
			return false;
		}
}

uint16_t MPU_read_data(uint16_t addr)
{
	return I2C_ReadData(MPU_ADDRESS, 1, addr);
}

void MPU_read_accel(uint16_t *AXRAW, uint16_t *AYRAW, uint16_t *AZRAW)
{
	uint8_t registerArray[6];
	registerArray[0] = MPU_read_data(0x3B);
	registerArray[1] = MPU_read_data(0x3C);
	registerArray[2] = MPU_read_data(0x3D);
	registerArray[3] = MPU_read_data(0x3E);
	registerArray[4] = MPU_read_data(0x3F);
	registerArray[5] = MPU_read_data(0x40);
	*AXRAW = registerArray[0] << 8 | registerArray[1];
	*AYRAW = registerArray[2] << 8 | registerArray[3];
	*AZRAW = registerArray[4] << 8 | registerArray[5];
}

void MPU_read_gyro(uint16_t *GXRAW, uint16_t *GYRAW, uint16_t *GZRAW)
{
	uint8_t registerArray[6];
	registerArray[0] = MPU_read_data(0x43);
	registerArray[1] = MPU_read_data(0x44);
	registerArray[2] = MPU_read_data(0x45);
	registerArray[3] = MPU_read_data(0x46);
	registerArray[4] = MPU_read_data(0x47);
	registerArray[5] = MPU_read_data(0x48);
	*GXRAW = registerArray[0] << 8 | registerArray[1];
	*GYRAW = registerArray[2] << 8 | registerArray[3];
	*GZRAW = registerArray[4] << 8 | registerArray[5];
}

void MPU_read_all(uint16_t *AXRAW, uint16_t *AYRAW, uint16_t *AZRAW, uint16_t *GXRAW, uint16_t *GYRAW, uint16_t *GZRAW)
{
	MPU_read_accel(AXRAW, AYRAW, AZRAW);
	MPU_read_gyro(GXRAW, GYRAW, GZRAW);
}

void MPU_set_accel(uint8_t acceleration)
{
	if(acceleration == 2)
	{
		I2C_WriteData(MPU_ADDRESS, 2, 0x1B, MPU_A2G);
	} else if(acceleration == 4)
	{
		I2C_WriteData(MPU_ADDRESS, 2, 0x1B, MPU_A4G);
	} else if(acceleration == 8)
	{
		I2C_WriteData(MPU_ADDRESS, 2, 0x1B, MPU_A8G);
	} else if(acceleration == 16)
	{
		I2C_WriteData(MPU_ADDRESS, 2, 0x1B, MPU_A16G);
	}
}

void MPU_set_gyro(uint8_t gyroSpeed)
{
	if(gyroSpeed == 2)
	{
		I2C_WriteData(MPU_ADDRESS, 2, 0x1C, MPU_G250G);
	} else if(gyroSpeed == 5)
	{
		I2C_WriteData(MPU_ADDRESS, 2, 0x1C, MPU_G500G);
	} else if(gyroSpeed == 10)
	{
		I2C_WriteData(MPU_ADDRESS, 2, 0x1C, MPU_G1000G);
	} else if(gyroSpeed == 20)
	{
		I2C_WriteData(MPU_ADDRESS, 2, 0x1C, MPU_G2000G);
	}
}

void MPU_sleep(void)
{
	I2C_WriteData(MPU_ADDRESS, 2, 0x6B, 0x40);
}

void MPU_wakeup(void)
{
	I2C_WriteData(MPU_ADDRESS, 2, 0x6B, 0x00);
}
