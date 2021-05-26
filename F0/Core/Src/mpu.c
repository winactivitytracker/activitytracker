#include "mpu.h"

uint8_t counter = 0;
char* nameArray[6] = {"aX", "aY", "aZ", "gX", "gY", "gZ"};
RTC_TimeTypeDef currTime, currDate = {0};

uint8_t currentAccelScale, currentGyroScale;
int16_t MPUData[6];
uint8_t orientationLeg[2];
int16_t previous;
uint8_t forceCounter;

bool MPU6050Init(void)
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

void MPU6050ReadAccel(int16_t *aXRaw, int16_t *aYRaw, int16_t *aZRaw)
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

void MPU6050ReadGyro(int16_t *gXRaw, int16_t *gYRaw, int16_t *gZRaw)
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

void MPUReadAll(int16_t *aXRaw, int16_t *aYRaw, int16_t *aZRaw, int16_t *gXRaw, int16_t *gYRaw, int16_t *gZRaw)
{
	MPU6050ReadAccel(aXRaw, aYRaw, aZRaw);
	MPU6050ReadGyro(gXRaw, gYRaw, gZRaw);
}

void MPUSetAccel(uint8_t acceleration)
{

	currentAccelScale = acceleration;
	HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, ACCEL_CONFIG_REG, 1, &acceleration, 1, 1000);
}

void MPUSetGyro(uint8_t gyroSpeed)
{
	currentGyroScale = gyroSpeed;
	HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, GYRO_CONFIG_REG, 1, &gyroSpeed, 1, 1000);
}

void MPUOrientation(uint8_t *orientationAxis, uint8_t *orientationNegative)
{
	uint16_t highestData = 0;
	uint8_t negative = 0;
	uint8_t forceCounter = 0;
	int16_t axisData[6];

	MPU6050ReadAccel(&axisData[0], &axisData[1], &axisData[2]);

	while(forceCounter < 3)
	{
		if(axisData[forceCounter] < 0)
	  	{
	  		axisData[forceCounter] = axisData[forceCounter]*-1;
	  		negative = 1;
	  	}else
	  	{
	  		negative = 0;
	  	}
	  	if(highestData < axisData[forceCounter])
	  	{
	  		highestData = axisData[forceCounter];
	  		*orientationAxis = forceCounter;
	  		*orientationNegative = negative;
	  	}
		forceCounter++;
	}
}

void getMPUData()
{
	HAL_RTC_GetTime(&hrtc, &currTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &currDate, RTC_FORMAT_BIN);
	//MPUReadAll(&MPURTCData[currTime.Seconds][counter][0], &MPURTCData[currTime.Seconds][counter][1], &MPURTCData[currTime.Seconds][counter][2], &MPURTCData[currTime.Seconds][counter][3], &MPURTCData[currTime.Seconds][counter][4], &MPURTCData[currTime.Seconds][counter][5]);
}

void stap()
{
	//	uint8_t forCounter = 0;
	//	char numbers[6];

	int16_t data[3];

	MPU6050ReadAccel(&data[0], &data[1], &data[2]);

	/*
	sprintf(numbers, "%d", data[0]);
	HAL_UART_Transmit(&huart1, numbers, sizeof(numbers), HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, "_", sizeof("_"), HAL_MAX_DELAY);
	sprintf(numbers, "%d", data[1]);
	HAL_UART_Transmit(&huart1, numbers, sizeof(numbers), HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, "_", sizeof("_"), HAL_MAX_DELAY);
	sprintf(numbers, "%d", data[2]);
	HAL_UART_Transmit(&huart1, numbers, sizeof(numbers), HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, "_", sizeof("_"), HAL_MAX_DELAY);
	*/

	if(orientationLeg[1] == 0)
	{
		if(data[orientationLeg[0]] > (previous + 2000))
		{
			HAL_UART_Transmit(&huart1, "STAP", sizeof("STAP"), HAL_MAX_DELAY);
		}
	}
	else
	{
		if(data[orientationLeg[0]] < (previous - 2000))
		{
			HAL_UART_Transmit(&huart1, "STAP", sizeof("STAP"), HAL_MAX_DELAY);
		}
	}

	previous = data[orientationLeg[0]];
	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);
	HAL_UART_Transmit(&huart1, "\n", sizeof("\n"), HAL_MAX_DELAY);
}

void MPUToUsartInit()
{
	MPUOrientation(&orientationLeg[0], &orientationLeg[1]);

	if(orientationLeg[1] == 1)
	{
		HAL_UART_Transmit(&huart1, "-", sizeof("-"), HAL_MAX_DELAY);
	}

	if(orientationLeg[0] == 0)
	{
		HAL_UART_Transmit(&huart1, "X", sizeof("X"), HAL_MAX_DELAY);
	}
	else if(orientationLeg[0] == 1)
	{
		HAL_UART_Transmit(&huart1, "Y", sizeof("Y"), HAL_MAX_DELAY);
	}
	else
	{
		HAL_UART_Transmit(&huart1, "Z", sizeof("Z"), HAL_MAX_DELAY);
	}

	HAL_UART_Transmit(&huart1, "\n", sizeof("\n"), HAL_MAX_DELAY);

	for(uint8_t i = 0; i < 3; i++)
	{
		HAL_UART_Transmit(&huart1, nameArray[i], sizeof(nameArray[i]), HAL_MAX_DELAY);
		if(i != 5)
		{
			HAL_UART_Transmit(&huart1, ",", sizeof(","), HAL_MAX_DELAY);
		}
		else
		{
			HAL_UART_Transmit(&huart1, "\n", sizeof("\n"), HAL_MAX_DELAY);
		}
	}
}

void MPUToUsartLoop()
{
	char numberss[6];
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_2);
	stap();
	MPUReadAll(&MPUData[0], &MPUData[1], &MPUData[2], &MPUData[3], &MPUData[4], &MPUData[5]);
	forceCounter = 0;

	while(forceCounter < 6)
	{
		sprintf(numberss, "%d", MPUData[forceCounter]);
		HAL_UART_Transmit(&huart1, numberss, sizeof(numberss), HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart1, "\t", sizeof("\t"), HAL_MAX_DELAY);
		forceCounter++;
	}

	HAL_UART_Transmit(&huart1, "\n", sizeof("\n"), HAL_MAX_DELAY);
	//HAL_UART_Transmit(&huart1, "\n", sizeof("\n"), HAL_MAX_DELAY);
}
