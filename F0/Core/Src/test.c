/*
 * test.c
 *
 *  Created on: 16 Jun 2021
 *      Author: markv
 */

/*
 * This file will test the functions of the project
 * You can run the tests by calling the "testAll()" function in the main.
 */
#include "main.h"
#include "rtc.h"
#include "mpu.h"
#include "test.h"
#include "radioAPI.h"



void testAll()
{
	testMPU();
	testRTC();
}

#if TEST_MPU
void testMPU()
{
	setCorrectMPU();
	setIncorrectMPU();
	testGyro();
	testAccel();
}
#endif
void testRadio()
{

}
#if TEST_RTC
void testRTC()
{
	setCorrectTime();
	setIncorrectTime();
}
#endif

#if TEST_MPU
void setCorrectMPU()
{
	//Arrange
	uint8_t setAccel = MPU_A8G, setGyro = MPU_G1000G;
	uint8_t accelTest, gyroTest;
	char shouldBe[90], isCurrent[60];
	for(uint8_t i = 0; i < 90; i++)
	{
		if(i < 60)
		{
			isCurrent[i] = (char)0;
		}
		shouldBe[i] = (char)0;
	}
	char* result = "";
	//Act
	accelTest = MPUSetAccel(setAccel);
	gyroTest = MPUSetGyro(setGyro);
	uint8_t accelExpected = MPU_A8G, gyroExpected = MPU_G1000G;
	//Assert
	if(accelTest == accelExpected)
	{
		result = "accel set is correct";
	} else
	{
		result = "accel set is incorrect";
		sprintf(shouldBe, "%d", accelExpected);
		sprintf(isCurrent, "%d", accelTest);
		printWrongData(shouldBe, isCurrent);
	}
	printTestResult("MPU", "MPU accel set correct data", result);
	if(gyroTest == gyroExpected)
	{
		result = "gyro set is correct";
	} else
	{
		result = "gyro set is incorrect";
		sprintf(shouldBe, "%d", accelExpected);
		sprintf(isCurrent, "%d", accelTest);
		printWrongData(shouldBe, isCurrent);
	}
	printTestResult("MPU", "MPU gyro set correct data", result);
}

void setIncorrectMPU()
{
	//Arrange
	uint8_t setAccel = 50, setGyro = 50;
	uint8_t accelTest, gyroTest;
	char shouldBe[90], isCurrent[60];
	for(uint8_t i = 0; i < 90; i++)
	{
		if(i < 60)
		{
			isCurrent[i] = (char)0;
		}
		shouldBe[i] = (char)0;
	}
	char* result = "";
	//Act
	accelTest = MPUSetAccel(setAccel);
	gyroTest = MPUSetGyro(setGyro);
	uint8_t accelExpected = MPU_A8G, gyroExpected = MPU_G1000G;
	//Assert
	if(accelTest == accelExpected)
	{
		result = "accel set is correct";
	} else
	{
		result = "accel set is incorrect";
		sprintf(shouldBe, "%d", accelExpected);
		sprintf(isCurrent, "%d", accelTest);
		printWrongData(shouldBe, isCurrent);
	}
	printTestResult("MPU", "MPU accel set incorrect data", result);
	if(gyroTest == gyroExpected)
	{
		result = "gyro set is correct";
	} else
	{
		result = "gyro set is incorrect";
		sprintf(shouldBe, "%d", accelExpected);
		sprintf(isCurrent, "%d", accelTest);
		printWrongData(shouldBe, isCurrent);
	}
	printTestResult("MPU", "MPU gyro set incorrect data", result);
}

void testGyro()
{
	//make sure the board is stationary on the table before running this test
	//Arrange
	int16_t gyroTest[3] = {0,0,0};
	char shouldBe[90], isCurrent[60];
	for(uint8_t i = 0; i < 90; i++)
	{
		if(i < 60)
		{
			isCurrent[i] = (char)0;
		}
		shouldBe[i] = (char)0;
	}
	char* result = "";
	//Act
	MPU6050ReadGyro(&gyroTest[0], &gyroTest[1], &gyroTest[2]);
	//Assert
	if(gyroTest[0] > (int16_t)MINACCELORGYRO && gyroTest[0] < (int16_t)MAXACCELORGYRO)
	{
		result = "gyro data is within max allowable limits";
	} else
	{
		result = "gyro data is out of allowable limits";
		sprintf(shouldBe, "%s %d %s %d", "between", MINACCELORGYRO, "and", MAXACCELORGYRO);
		HAL_Delay(20);
		sprintf(isCurrent, "%d", gyroTest[0]);
		printWrongData(shouldBe, isCurrent);
	}
	printTestResult("MPU", "MPU gyro test", result);

	if(gyroTest[1] > (int16_t)MINACCELORGYRO && gyroTest[1] < (int16_t)MAXACCELORGYRO)
	{
		result = "gyro data is within max allowable limits";
	} else
	{
		result = "gyro data is out of allowable limits";
		sprintf(shouldBe, "%s-%d-%s-%d", "between", MINACCELORGYRO, "and", MAXACCELORGYRO);
		HAL_Delay(20);
		sprintf(isCurrent, "%d", gyroTest[1]);
		printWrongData(shouldBe, isCurrent);
	}
	printTestResult("MPU", "MPU gyro test", result);

	if(gyroTest[2] > (int16_t)MINACCELORGYRO && gyroTest[2] < (int16_t)MAXACCELORGYRO)
	{
		result = "gyro data is within max allowable limits";
	} else
	{
		result = "gyro data is out of allowable limits";
		sprintf(shouldBe, "%s %d %s %d", "between", MINACCELORGYRO, "and", MAXACCELORGYRO);
		HAL_Delay(20);
		sprintf(isCurrent, "%d", gyroTest[2]);
		printWrongData(shouldBe, isCurrent);
	}
	printTestResult("MPU", "MPU gyro test", result);
}

void testAccel()
{
	//make sure the board is stationary on the table before running this test
	//Arrange
	int16_t accelTest[3] = {0,0,0};
	char shouldBe[90], isCurrent[60];
	for(uint8_t i = 0; i < 90; i++)
	{
		if(i < 60)
		{
			isCurrent[i] = (char)0;
		}
		shouldBe[i] = (char)0;
	}
	char* result = "";
	//Act
	MPU6050ReadGyro(&accelTest[0], &accelTest[1], &accelTest[2]);
	//Assert
	if(accelTest[0] > (int16_t)MINACCELORGYRO && accelTest[0] < (int16_t)MAXACCELORGYRO)
	{
		result = "accel data is within max allowable limits";
	} else
	{
		result = "accel data is out of allowable limits";
		sprintf(shouldBe, "%s %d %s %d", "between", MINACCELORGYRO, "and", MAXACCELORGYRO);
		HAL_Delay(20);
		sprintf(isCurrent, "%d", accelTest[0]);
		printWrongData(shouldBe, isCurrent);
	}
	printTestResult("MPU", "MPU accel test", result);

	if(accelTest[1] > (int16_t)MINACCELORGYRO && accelTest[1] < (int16_t)MAXACCELORGYRO)
	{
		result = "accel data is within max allowable limits";
	} else
	{
		result = "accel data is out of allowable limits";
		sprintf(shouldBe, "%s-%d-%s-%d", "between", MINACCELORGYRO, "and", MAXACCELORGYRO);
		HAL_Delay(20);
		sprintf(isCurrent, "%d", accelTest[1]);
		printWrongData(shouldBe, isCurrent);
	}
	printTestResult("MPU", "MPU accel test", result);

	if(accelTest[2] > (int16_t)MINACCELORGYRO && accelTest[2] < (int16_t)MAXACCELORGYRO)
	{
		result = "accel data is within max allowable limits";
	} else
	{
		result = "accel data is out of allowable limits";
		sprintf(shouldBe, "%s %d %s %d", "between", MINACCELORGYRO, "and", MAXACCELORGYRO);
		HAL_Delay(20);
		sprintf(isCurrent, "%d", accelTest[2]);
		printWrongData(shouldBe, isCurrent);
	}
	printTestResult("MPU", "MPU accel test", result);
}
#endif

#if TEST_RTC
void setCorrectTime()
{
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;	//this isn't used but should be called for correct updates.
	char shouldBe[90], isCurrent[60];
	for(uint8_t i = 0; i < 90; i++)
	{
		if(i < 60)
		{
			isCurrent[i] = (char)0;
		}
		shouldBe[i] = (char)0;
	}
	char* result = "";
	//Arrange
	uint8_t RTCTestData[3] = {12, 12, 12};
	uint8_t RTCTestDataExpected[3];
	RTC_SetTime(RTCTestData[0], RTCTestData[1], RTCTestData[2]);
	//Act
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	RTCTestDataExpected[0] = 12;
	RTCTestDataExpected[1] = 12;
	RTCTestDataExpected[2] = 12;
	//Assert
	if(sTime.Hours == RTCTestDataExpected[0])
	{
		result = "hours = correct";
	} else
	{
		result = "hours = incorrect";
		sprintf(shouldBe, "%d", RTCTestDataExpected[0]);
		sprintf(isCurrent, "%d", sTime.Hours);
		printWrongData(shouldBe, isCurrent);
	}
	printTestResult("RTC", "RTC set correct time", result);

	if(sTime.Minutes == RTCTestDataExpected[1])
	{
		result = "minutes = correct";
	} else
	{
		result = "minutes = incorrect";
		sprintf(shouldBe, "%d", RTCTestDataExpected[1]);
		sprintf(isCurrent, "%d", sTime.Minutes);
		printWrongData(shouldBe, isCurrent);
	}
	printTestResult("RTC", "RTC set correct time", result);

	if(sTime.Seconds == RTCTestDataExpected[2])
	{
		result = "seconds = correct";
	} else
	{
		result = " seconds = incorrect";
		sprintf(shouldBe, "%d", RTCTestDataExpected[2]);
		sprintf(isCurrent, "%d", sTime.Seconds);
		printWrongData(shouldBe, isCurrent);
	}
	printTestResult("RTC", "RTC set correct time", result);
}

void setIncorrectTime()
{
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;	//this isn't used but should be called for correct updates.
	char shouldBe[90], isCurrent[60];
	for(uint8_t i = 0; i < 90; i++)
	{
		if(i < 60)
		{
			isCurrent[i] = (char)0;
		}
		shouldBe[i] = (char)0;
	}
	char* result = "";
	//Arrange
	uint8_t RTCTestData[3] = {200, 200, 200};
	uint8_t RTCTestDataExpected[3];
	RTC_SetTime(RTCTestData[0], RTCTestData[1], RTCTestData[2]);
	//Act
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	RTCTestDataExpected[0] = 0;
	RTCTestDataExpected[1] = 0;
	RTCTestDataExpected[2] = 0;
	//Assert
	if(sTime.Hours == RTCTestDataExpected[0])
	{
		result = "hours = correct";
	} else
	{
		result = "hours = incorrect";
		sprintf(shouldBe, "%d", RTCTestDataExpected[0]);
		sprintf(isCurrent, "%d", sTime.Hours);
		printWrongData(shouldBe, isCurrent);
	}
	printTestResult("RTC", "RTC set incorrect time", result);

	if(sTime.Minutes == RTCTestDataExpected[1])
	{
		result = "minutes = correct";
	} else
	{
		result = "minutes = incorrect";
		sprintf(shouldBe, "%d", RTCTestDataExpected[1]);
		sprintf(isCurrent, "%d", sTime.Minutes);
		printWrongData(shouldBe, isCurrent);
	}
	printTestResult("RTC", "RTC set incorrect time", result);

	if(sTime.Seconds == RTCTestDataExpected[2])
	{
		result = "seconds = correct";
	} else
	{
		result = " seconds = incorrect";
		sprintf(shouldBe, "%d", RTCTestDataExpected[2]);
		sprintf(isCurrent, "%d", sTime.Seconds);
		printWrongData(shouldBe, isCurrent);
	}
	printTestResult("RTC", "RTC set incorrect time", result);
}
#endif

void printTestResult(char* testObject, char* currentTest, char* result)
{
	char localOject[60] = "";
	char localTest[60] = "";
	char localResult[60] = "";
	sprintf(localOject, "%s", testObject);
	sprintf(localTest, "%s", currentTest);
	sprintf(localResult, "%s", result);
	HAL_UART_Transmit(&huart1, "This is the test of ", sizeof("This is the test of "), HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, localOject, sizeof(localOject), HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, " and the current test is ", sizeof(" and the current test = "), HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, localTest, sizeof(localTest), HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, "\n", sizeof("\n"), HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, "The result of this test is: ", sizeof("The result of this test is: "), HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, localResult, sizeof(localResult), HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, "\n", sizeof("\n"), HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, "----------------------", sizeof("----------------------"), HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, "\n", sizeof("\n"), HAL_MAX_DELAY);
}

void printWrongData(char shouldBe[90], char isCurrent[60])
{
	HAL_UART_Transmit(&huart1, "Data is currently: ", sizeof("Data is currently: "), HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, isCurrent, sizeof(isCurrent), HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, "\n", sizeof("\n"), HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, "Data should be: ", sizeof("Data should be: "), HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, shouldBe, sizeof(shouldBe), HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, "\n", sizeof("\n"), HAL_MAX_DELAY);
}
