#include "mpu.h"

bool MPUInit(void)
{
	if(MPUAvailable())
	{
		MPUWakeup();								//wake up
		I2CWriteData(MPU_ADDRESS ,2, 0x19, 0x07);	//set speed at 1Khz
		MPUSetAccel(2);								//standard setup, Accel at 2G
		MPUSetGyro(2);								//standard setup, Gyro at 250 D/S
		
		return true;
	}
	else
	{
		return false;
	}
}

bool MPUAvailable(void)
{
	if(I2CReadData(MPU_ADDRESS, 1, 0x75) == 0x68)
	{
		GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_RESET); //RED
		GPIO_WriteBit(GPIOB, GPIO_Pin_2, Bit_SET); //GREEN
		return true;
	}
	else
	{
		GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_SET); //RED
		GPIO_WriteBit(GPIOB, GPIO_Pin_2, Bit_RESET); //GREEN
		return false;
	}
}

uint16_t MPUReadData(uint16_t addr)
{
	return I2CReadData(MPU_ADDRESS, 1, addr);
}

void MPUReadAccel(uint16_t *aXRaw, uint16_t *aYRaw, uint16_t *aZRaw)
{
	uint8_t registerArray[6];
	
	registerArray[0] = MPUReadData(0x3B);
	registerArray[1] = MPUReadData(0x3C);
	registerArray[2] = MPUReadData(0x3D);
	registerArray[3] = MPUReadData(0x3E);
	registerArray[4] = MPUReadData(0x3F);
	registerArray[5] = MPUReadData(0x40);
	
	*aXRaw = registerArray[0] << 8 | registerArray[1];
	*aYRaw = registerArray[2] << 8 | registerArray[3];
	*aZRaw = registerArray[4] << 8 | registerArray[5];
}

void MPUReadGyro(uint16_t *gXRaw, uint16_t *gYRaw, uint16_t *gZRaw)
{
	uint8_t registerArray[6];
	
	registerArray[0] = MPUReadData(0x43);
	registerArray[1] = MPUReadData(0x44);
	registerArray[2] = MPUReadData(0x45);
	registerArray[3] = MPUReadData(0x46);
	registerArray[4] = MPUReadData(0x47);
	registerArray[5] = MPUReadData(0x48);
	
	*gXRaw = registerArray[0] << 8 | registerArray[1];
	*gYRaw = registerArray[2] << 8 | registerArray[3];
	*gZRaw = registerArray[4] << 8 | registerArray[5];
}

void MPUReadAll(uint16_t *aXRaw, uint16_t *aYRaw, uint16_t *aZRaw, uint16_t *gXRaw, uint16_t *gYRaw, uint16_t *gZRaw)
{
	MPUReadAccel(aXRaw, aYRaw, aZRaw);
	MPUReadGyro(gXRaw, gYRaw, gZRaw);
}

void MPUSetAccel(uint8_t acceleration)
{
	if(acceleration == 2)
	{
		I2CWriteData(MPU_ADDRESS, 2, 0x1B, MPU_A2G);
	}
	else if(acceleration == 4)
	{
		I2CWriteData(MPU_ADDRESS, 2, 0x1B, MPU_A4G);
	}
	else if(acceleration == 8)
	{
		I2CWriteData(MPU_ADDRESS, 2, 0x1B, MPU_A8G);
	}
	else if(acceleration == 16)
	{
		I2CWriteData(MPU_ADDRESS, 2, 0x1B, MPU_A16G);
	}
}

void MPUSetGyro(uint8_t gyroSpeed)
{
	if(gyroSpeed == 2)
	{
		I2CWriteData(MPU_ADDRESS, 2, 0x1C, MPU_G250G);
	} else if(gyroSpeed == 5)
	{
		I2CWriteData(MPU_ADDRESS, 2, 0x1C, MPU_G500G);
	} else if(gyroSpeed == 10)
	{
		I2CWriteData(MPU_ADDRESS, 2, 0x1C, MPU_G1000G);
	} else if(gyroSpeed == 20)
	{
		I2CWriteData(MPU_ADDRESS, 2, 0x1C, MPU_G2000G);
	}
}

void MPUSleep(void)
{
	I2CWriteData(MPU_ADDRESS, 2, 0x6B, 0x40);
}

void MPUWakeup(void)
{
	I2CWriteData(MPU_ADDRESS, 2, 0x6B, 0x00);
}
