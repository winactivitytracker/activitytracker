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
	testRTC();
}

void testMPU()
{

}

void testRadio()
{

}

void testRTC()
{
	setCorrectTime();
	setIncorrectTime();
}

void setCorrectTime()
{
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;	//this isn't used but should be called for correct updates.
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
	}
	printTestResult("RTC", "RTC set correct time", result);

	if(sTime.Minutes == RTCTestDataExpected[1])
	{
		result = "minutes = correct";
	} else
	{
		result = "minutes = incorrect";
	}
	printTestResult("RTC", "RTC set correct time", result);

	if(sTime.Seconds == RTCTestDataExpected[2])
	{
		result = "seconds = correct";
	} else
	{
		result = " seconds = incorrect";
	}
	printTestResult("RTC", "RTC set correct time", result);
}

void setIncorrectTime()
{
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;	//this isn't used but should be called for correct updates.
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
	}
	printTestResult("RTC", "RTC set incorrect time", result);

	if(sTime.Minutes == RTCTestDataExpected[1])
	{
		result = "minutes = correct";
	} else
	{
		result = "minutes = incorrect";
	}
	printTestResult("RTC", "RTC set incorrect time", result);

	if(sTime.Seconds == RTCTestDataExpected[2])
	{
		result = "seconds = correct";
	} else
	{
		result = " seconds = incorrect";
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
