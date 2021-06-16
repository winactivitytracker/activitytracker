/*
 * radioHandler.c
 *
 *  Created on: 16 Jun 2021
 *      Author: markv
 */

#include "main.h"
#include "radioHandler.h"
#include "mpu.h"
#include "radioAPI.h"

// Send the ID, time and Gyro Z-axis over
// radio according to Pure ALOHA protocol
void sendGyroZ()
{
	uint8_t timeoutCounter = MAXTIMEOUT;

	// Get the time
	RTC_TimeTypeDef currTime;
	RTC_DateTypeDef currDate;
	HAL_RTC_GetTime(&hrtc, &currTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &currDate, RTC_FORMAT_BIN);

	// Get the gyro Z axis
	int16_t gXRaw, gYRaw, gZRaw;
	uint8_t sec, min, hour;
	MPU6050ReadGyro(&gXRaw,&gYRaw,&gZRaw);

	// Create the message string
	// Example message: "z,1,11,59,59,-1000"
	// Make the string 60 to be sure
	char MPUString[60] = "";

	// Put the time values in uint8_t variables to ensure
	// they're always this format
	hour = currTime.Hours;
	min = currTime.Minutes;
	sec = currTime.Seconds;

	// Construct the string
	sprintf(MPUString,
		"z,%u,%u,%u,%u,%d",
		IDENTIFIER,
		hour,
		min,
		sec,
		gZRaw
	);

	// Send the initial message
	transmitterSendBlocking(MPUString);

	// Wait for an ACK
	while(!receiverWaitForAck((200)) && timeoutCounter != 0)
	{
		// Wait a random amount of time until resend
		alohaWait();

		// Resend
		transmitterSendBlocking(MPUString);

		timeoutCounter--;
	}

	// Wait, for debugging purposes
	HAL_Delay(250);
}

// Wait a random amount of time between 200 and 500 ms
void alohaWait()
{
	uint16_t r = 200 + (rand() % 300);
	HAL_Delay(r);
}

// Request the time from the main unit and handle the response
bool askTime()
{
	uint8_t timeOutCounter = 20;

	// Construct the time request string
	char timeAskString[4] = "";
	sprintf(timeAskString,
		"t,%u",
		IDENTIFIER
	);

	// Send the data
	transmitterSendBlocking(timeAskString);

	// ALOHA: wait for ack, resend after random time
	while(!receiverWaitForAck(300))
	{
		alohaWait();
		transmitterSendBlocking(timeAskString);
	}

	unsigned int inputArray[3];
	char incoming[100];

	// Keep requesting until we have a time or if there's a timeout
	for(;;)
	{
		// See if there's a message
		if(receiverCheckMessage())
		{
			// place it it incoming
			receiverPopMessage(incoming);

			// If message starts with tt, it's a time message
			if(strncmp(incoming, "tt,", 3) == 0)
			{
				// Read the data and add it to the RTC clock
				sscanf(incoming, "tt,%u:%u:%u", &inputArray[0], &inputArray[1], &inputArray[2]);
				RTC_SetTime(inputArray[0], inputArray[1], inputArray[2]);

				// Acknowledge the time message
				transmitterSendAck();

				return true;
			}
		}
		// Exit the for loop if there's a timeout
		if(timeOutCounter == 0)
		{
			break;
		}

		timeOutCounter--;
		HAL_Delay(50);
	}
	return false;
}

