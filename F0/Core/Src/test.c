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

void testAll()
{
	testMPU();
	testRTC();
}

void testMPU()
{
	setCorrectMPU();
	setIncorrectMPU();
}

void testRadio()
{

}

void testRTC()
{
	setCorrectTime();
	setIncorrectTime();
}

void setCorrectMPU()
{
	//Arrange
	uint8_t setAccel = MPU_A8G, setGyro = MPU_G1000G;
	uint8_t accelTest, gyroTest;
	char shouldBe[30], isCurrent[30];
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
	char shouldBe[30], isCurrent[30];
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

void setCorrectTime()
{
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;	//this isn't used but should be called for correct updates.
	char shouldBe[30], isCurrent[30];
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
	char shouldBe[30], isCurrent[30];
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

void printTestResult(char* testObject, char* currentTest, char* result)
{
	char localOject[30] = "";
	char localTest[30] = "";
	char localResult[30] = "";
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

	HAL_Delay(500);
}

void printWrongData(char shouldBe[30], char isCurrent[30])
{
	HAL_UART_Transmit(&huart1, "Data is currently: ", sizeof("Data is currently: "), HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, isCurrent, sizeof(isCurrent), HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, "\n", sizeof("\n"), HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, "Data should be: ", sizeof("Data should be: "), HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, shouldBe, sizeof(shouldBe), HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, "\n", sizeof("\n"), HAL_MAX_DELAY);

	HAL_Delay(500);
}
