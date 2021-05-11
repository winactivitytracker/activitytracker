#include "mpu.h"

uint8_t currentAccelScale, currentGyroScale;

bool MPU6050Init()
{
	uint8_t check;
	uint8_t data;

	// check device ID WHO_AM_I

	HAL_I2C_Mem_Read (&hi2c1, MPU6050_ADDR,WHO_AM_I_REG,1, &check, 1, 1000);

	if (check == 104)  // 0x68 will be returned by the sensor if everything goes well
	{
		// power management register 0X6B we should write all 0's to wake the sensor up
		data = 0;
		HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, PWR_MGMT_1_REG, 1,&data, 1, 1000);

		// Set DATA RATE of 1KHz by writing SMPLRT_DIV register
		data = 0x07;
		HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, SMPLRT_DIV_REG, 1, &data, 1, 1000);

		// Set accelerometer configuration in ACCEL_CONFIG Register
		// XA_ST=0,YA_ST=0,ZA_ST=0, FS_SEL=0 -> � 2g
		data = MPU_A2G;
		currentAccelScale = data;
		HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, ACCEL_CONFIG_REG, 1, &data, 1, 1000);

		// Set Gyroscopic configuration in GYRO_CONFIG Register
		// XG_ST=0,YG_ST=0,ZG_ST=0, FS_SEL=0 -> � 250 �/s
		data = MPU_G250G;
		currentGyroScale = data;
		HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, GYRO_CONFIG_REG, 1, &data, 1, 1000);
		return true;
	} else 
	{
		return false;
	}

}

void MPU6050ReadAccel(uint16_t *aXRaw, uint16_t *aYRaw, uint16_t *aZRaw)
{
	uint8_t RecData[6];

	// Read 6 BYTES of data starting from ACCEL_XOUT_H register

	HAL_I2C_Mem_Read (&hi2c1, MPU6050_ADDR, ACCEL_XOUT_H_REG, 1, RecData, 6, 1000);

	*aXRaw = (int16_t)(RecData[0] << 8 | RecData [1]);
	*aYRaw = (int16_t)(RecData[2] << 8 | RecData [3]);
	*aZRaw = (int16_t)(RecData[4] << 8 | RecData [5]);

	/*** convert the RAW values into acceleration in 'g'
	     we have to divide according to the Full scale value set in FS_SEL
	     I have configured FS_SEL = 0. So I am dividing by 16384.0
	     for more details check ACCEL_CONFIG Register              ****/

	//Ax = Accel_X_RAW/16384.0;
	//Ay = Accel_Y_RAW/16384.0;
	//Az = Accel_Z_RAW/16384.0;
}

void MPU6050ReadGyro(uint16_t *gXRaw, uint16_t *gYRaw, uint16_t *gZRaw)
{
	uint8_t RecData[6];

	// Read 6 BYTES of data starting from GYRO_XOUT_H register

	HAL_I2C_Mem_Read (&hi2c1, MPU6050_ADDR, GYRO_XOUT_H_REG, 1, RecData, 6, 1000);

	*gXRaw = (int16_t)(RecData[0] << 8 | RecData [1]);
	*gYRaw = (int16_t)(RecData[2] << 8 | RecData [3]);
	*gZRaw = (int16_t)(RecData[4] << 8 | RecData [5]);

	/*** convert the RAW values into dps (�/s)
	     we have to divide according to the Full scale value set in FS_SEL
	     I have configured FS_SEL = 0. So I am dividing by 131.0
	     for more details check GYRO_CONFIG Register              ****/

	//Gx = Gyro_X_RAW/131.0;
	//Gy = Gyro_Y_RAW/131.0;
	//Gz = Gyro_Z_RAW/131.0;
}

void MPUReadAll(uint16_t *aXRaw, uint16_t *aYRaw, uint16_t *aZRaw, uint16_t *gXRaw, uint16_t *gYRaw, uint16_t *gZRaw)
{
	MPU6050ReadAccel(aXRaw, aYRaw, aZRaw);
	MPU6050ReadGyro(gXRaw, gYRaw, gZRaw);
}

void MPUSetAccel(uint8_t acceleration)
{
	uint8_t data;
	if(acceleration == 2)
	{
		data = MPU_A2G;
	}
	else if(acceleration == 4)
	{
		data = MPU_A4G;
	}
	else if(acceleration == 8)
	{
		data = MPU_A8G;
	}
	else if(acceleration == 16)
	{
		data = MPU_A16G;
	}
	currentAccelScale = data;
	HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, ACCEL_CONFIG_REG, 1, &data, 1, 1000);
}

void MPUSetGyro(uint8_t gyroSpeed)
{
	uint8_t data;
	if(gyroSpeed == 2)
	{
		data = MPU_G250G;
	} else if(gyroSpeed == 5)
	{
		data = MPU_G500G;
	} else if(gyroSpeed == 10)
	{
		data = MPU_G1000G;
	} else if(gyroSpeed == 20)
	{
		data = MPU_G2000G;
	}
	currentGyroScale = data;
	HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, GYRO_CONFIG_REG, 1, &data, 1, 1000);
}
