#include "mpu.h"

bool MPU6050Init(void)
{
	uint8_t check;
	uint8_t data;

	// check device ID WHO_AM_I

	HAL_I2C_Mem_Read (&hi2c1, MPU6050_ADDR,WHO_AM_I_REG,1, &check, 1, 1000);

	if (check == 104)  // 0x68 will be returned by the sensor if everything goes well
	{
		//wake the sensor up
		data = 0;
		HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, PWR_MGMT_1_REG, 1,&data, 1, 1000);

		//set the data rate at 1Khz
		data = SAMPLERATE1KHZ;
		HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, SMPLRT_DIV_REG, 1, &data, 1, 1000);

		// Set accelerometer configuration in ACCEL_CONFIG Register
		data = MPU_A2G;
		HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, ACCEL_CONFIG_REG, 1, &data, 1, 1000);

		// Set Gyroscopic configuration in GYRO_CONFIG Register
		data = MPU_G250G;
		HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, GYRO_CONFIG_REG, 1, &data, 1, 1000);
		return true;
	} else 
	{
		return false;
	}

}

//read the accelero data from the IMU
void MPU6050ReadAccel(int16_t *aXRaw, int16_t *aYRaw, int16_t *aZRaw)
{
	uint8_t RecData[6];

	// Read 6 BYTES of data starting from ACCEL_XOUT_H register

	HAL_I2C_Mem_Read (&hi2c1, MPU6050_ADDR, ACCEL_XOUT_H_REG, 1, RecData, 6, 1000);

	//return the raw accel values
	*aXRaw = (int16_t)(RecData[0] << 8 | RecData [1]);
	*aYRaw = (int16_t)(RecData[2] << 8 | RecData [3]);
	*aZRaw = (int16_t)(RecData[4] << 8 | RecData [5]);
}

//read the gyro data from the IMU
void MPU6050ReadGyro(int16_t *gXRaw, int16_t *gYRaw, int16_t *gZRaw)
{
	uint8_t RecData[6];

	// Read 6 BYTES of data starting from GYRO_XOUT_H register

	HAL_I2C_Mem_Read (&hi2c1, MPU6050_ADDR, GYRO_XOUT_H_REG, 1, RecData, 6, 1000);

	//return the raw gyro values
	*gXRaw = (int16_t)(RecData[0] << 8 | RecData [1]);
	*gYRaw = (int16_t)(RecData[2] << 8 | RecData [3]);
	*gZRaw = (int16_t)(RecData[4] << 8 | RecData [5]);
}

//read both the accelero and the gyro
void MPUReadAll(int16_t *aXRaw, int16_t *aYRaw, int16_t *aZRaw, int16_t *gXRaw, int16_t *gYRaw, int16_t *gZRaw)
{
	MPU6050ReadAccel(aXRaw, aYRaw, aZRaw);
	MPU6050ReadGyro(gXRaw, gYRaw, gZRaw);
}

//set the max acceleration rate
uint8_t MPUSetAccel(uint8_t acceleration)
{
	static uint8_t localAccel = 0;
	if(acceleration == MPU_A2G || acceleration == MPU_A4G || acceleration == MPU_A8G || acceleration == MPU_A16G)
	{
		HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, ACCEL_CONFIG_REG, 1, &acceleration, 1, 1000);
		localAccel = acceleration;
	}
	return localAccel;
}

//set the max gyroscopic rate
uint8_t MPUSetGyro(uint8_t gyroSpeed)
{
	static uint8_t localGyro = 0;
	if(gyroSpeed == MPU_G250G || gyroSpeed == MPU_G500G || gyroSpeed == MPU_G1000G || gyroSpeed == MPU_G2000G)
	{
		HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, GYRO_CONFIG_REG, 1, &gyroSpeed, 1, 1000);
		localGyro = gyroSpeed;
	}
	return localGyro;
}

void MPUToUsartInit()
{
	char* nameArray[6] = {"aX", "aY", "aZ", "gX", "gY", "gZ"};

	for(uint8_t i = 0; i < 3; i++)
	{
		HAL_UART_Transmit(&huart1, (uint8_t *)nameArray[i], sizeof(nameArray[i]), HAL_MAX_DELAY);
		if(i != 5)
		{
			HAL_UART_Transmit(&huart1, (uint8_t *)",", sizeof(","), HAL_MAX_DELAY);
		}
		else
		{
			HAL_UART_Transmit(&huart1, (uint8_t *)"\n", sizeof("\n"), HAL_MAX_DELAY);
		}
	}
}

void MPUToUsartLoop()
{
	int16_t MPUData[6];
	char numbers[6];
	uint8_t counter = 0;
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_2);
	MPUReadAll(&MPUData[0], &MPUData[1], &MPUData[2], &MPUData[3], &MPUData[4], &MPUData[5]);

	while(counter < 6)
	{
		sprintf(numbers, "%d", MPUData[counter]);
		HAL_UART_Transmit(&huart1, (uint8_t *)numbers, sizeof(numbers), HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart1, (uint8_t *)"\t", sizeof("\t"), HAL_MAX_DELAY);
		counter++;
	}

	HAL_UART_Transmit(&huart1, "\n", sizeof("\n"), HAL_MAX_DELAY);
	//HAL_UART_Transmit(&huart1, "\n", sizeof("\n"), HAL_MAX_DELAY);
}
