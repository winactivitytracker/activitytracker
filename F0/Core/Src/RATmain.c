/*
 * RATmain.c
 *
 *  Created on: 18 Jun 2021
 *      Author: markv
 */

#include "main.h"
#include "RATmain.h"
#include "radioAPI.h"
#include "test.h"

void RATmain()
{
	bool hasTime = false;
	uint8_t currentScale[2] = {0,0};

	//init the currentscale values
	currentScale[0] = MPU_A2G;
	currentScale[1] = MPU_G250G;

	//init the IMU and set it to its max values
	MPU6050Init();
	currentScale[0] = MPUSetAccel(MPU_A16G);
	currentScale[1] = MPUSetGyro(MPU_G2000G);

	// Start listening for messages
	receiverEnable();

	#if TEST_MPU
	HAL_UART_Transmit(&huart1, "TESTING MPU", sizeof("TESTING MPU"), HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, "\n", sizeof("\n"), HAL_MAX_DELAY);
	testMPU();
	#elif TEST_RTC
	HAL_UART_Transmit(&huart1, "TESTING RTC", sizeof("TESTING RTC"), HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, "\n", sizeof("\n"), HAL_MAX_DELAY);
	testRTC();
	#elif TEST_RADIO
	HAL_UART_Transmit(&huart1, "TESTING RADIO", sizeof("TESTING RADIO"), HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, "\n", sizeof("\n"), HAL_MAX_DELAY);
	radioTestAll();
	#endif

	while(1)
	{
		#if !TEST
			//ask the time, and keep asking the time until you you have the time
			if(!hasTime)
			{
				if(askTime())
				{
					hasTime = true;
				}
			}
			//once you have the time constantly send the gyro data
			else
			{
				sendGyroZ();
			}
		#endif
	}
}
