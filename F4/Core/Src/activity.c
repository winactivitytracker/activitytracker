/*
 * activity.c
 *
 *  Created on: 28 mei 2021
 *      Author: joel-
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "gps.h"
#include "activity.h"
#include "sdCard.h"

#define maxAcitivitySize 0Xff
#define amountOfActivites 	5
#define maxActivityPauze 	2
#define printSize			10

Activity_T CurrentActivity;
extern GPS_t GPS;

int16_t RTCMPUData[SELECT][BUFF_SIZE][DATA_ORDER];    //[which F0][buffer amount][0=hours,1=minutes,2=seconds,3=data]
uint8_t steps;
uint8_t prevSteps;
uint8_t buffer0Pointer = 0;
uint8_t buffer1Pointer = 0;
uint8_t buffer0TailPointer = 0;
uint8_t buffer1TailPointer = 0;
bool stepBlock = false;

//get activity based on gps speed
void getActivity()
{
		if(0 < GPS.speed_km && GPS.speed_km < 2.5 && prevSteps < 35)
		{
			CurrentActivity.currentActivity = noMovement;	//nomovement
		}
		else if(GPS.speed_km >= 2.5 && GPS.speed_km < 7.0 && prevSteps >= 35)
		{
			CurrentActivity.currentActivity = walking;	//walking
		}
		else if(GPS.speed_km >= 7.0 && GPS.speed_km < 15.0 && prevSteps >= 35)
		{
			CurrentActivity.currentActivity = running;
		}
		else if(GPS.speed_km == 0 && prevSteps > 35)
		{
			CurrentActivity.currentActivity = unknownIndoor;
		}
		else
		{
			CurrentActivity.currentActivity =  unknown;
		}
}

//return activity name
char* activityToString(uint8_t activity)
{
	char* string = "";

	switch (activity) {
		case noMovement:
			string = "Geen beweging";
			break;
		case walking:
			string = "Wandelen";
			break;
		case running:
			string = "Hardlopen";
			break;
		case unknownIndoor:
			string = "Indoor Activiteit";
			break;
		default:
			string = "onbekend";
			break;
	}
	return string;
}

//adding a activity after every occurrence
void CalculateActivityAverage(uint8_t lastActiveMinute)
{
	switch (lastActiveMinute) {
		case noMovement:
			CurrentActivity.activityTotal[noMovement]++;
			break;
		case walking:
			CurrentActivity.activityTotal[walking]++;
			break;
		case running:
			CurrentActivity.activityTotal[running]++;
			break;
		case unknownIndoor:
			CurrentActivity.activityTotal[unknownIndoor]++;
			break;
		default:
			CurrentActivity.activityTotal[unknown]++;
			break;
	}
}

//calculate the total activity and send it to the SD
void ActivityTotal()
{
	static float time = 0.0;
	static uint8_t counter = 0, counterPM = 0, counterPauze = 0;
	static uint8_t trackActivity[amountOfActivites];
	char* SDString = "";

	if(time != GPS.utc_time)
	{
		if(time == 0.0)
		{
			CurrentActivity.ActivityStartTime = GPS.utc_time;
		}
		time = GPS.utc_time;

		if(counter < 60)	//one minute
		{
			switch (CurrentActivity.currentActivity) {
				case noMovement:
					trackActivity[noMovement]++;
					break;
				case walking:
					trackActivity[walking]++;
					break;
				case running:
					trackActivity[running]++;
					break;
				case unknownIndoor:
					trackActivity[unknownIndoor]++;
					break;
				default:
					trackActivity[unknown]++;
					break;
			}

			counter++;
		}
		else
		{
			uint8_t current = 0;

			for(int i = 0; i < amountOfActivites; i++)
			{
				if(current <= trackActivity[i])
				{
					current = trackActivity[i];
					trackActivity[i] = 0;
					CurrentActivity.lastActiveMinute = i;
				}
			}

			//if the minute has ended add the most common activity of that minute to the total activity

			if(counterPM < maxAcitivitySize)
			{
				if(CurrentActivity.lastActiveMinute == walking || CurrentActivity.lastActiveMinute == running || CurrentActivity.lastActiveMinute == unknownIndoor) //activity measured
				{
					if(counterPM == 0)
					{
						writeStartToSD("MinActi.txt");

					}
					CalculateActivityAverage(CurrentActivity.lastActiveMinute);
					CurrentActivity.length++;
					CurrentActivity.activeDailyMinutes++;
					SDString = activityToString(CurrentActivity.lastActiveMinute);
					activityToSD("MinActi.txt", SDString);

					counterPM++;
				}
				else if(counterPM != 0 && (CurrentActivity.lastActiveMinute == noMovement || CurrentActivity.lastActiveMinute == unknown))
				{
					//if there is a pauze in the activity end the activity after three minutes without movement
					if(counterPauze < maxActivityPauze)
					{
						CalculateActivityAverage(CurrentActivity.lastActiveMinute);
						counterPauze++;
						counterPM++;
					}
					else
					{
						counterPM = maxAcitivitySize;
						counterPauze = 0;
					}
				}
			}
			else
			{
				//end of activity
				//calculate the avarage for total activity
				//and write total activity to sd

				current = 0;
				for(int i = 0; i < sizeof(CurrentActivity.activityTotal); i++)
				{
					if(current <= CurrentActivity.activityTotal[i])
					{
						current = CurrentActivity.activityTotal[i];
						CurrentActivity.activityTotal[i] = 0;
						CurrentActivity.totalActivity = i;
					}
				}
				char numbers[printSize];

				sprintf(numbers, "%d", CurrentActivity.length);
				SDString = activityToString(CurrentActivity.totalActivity);
				totalActivityToSD("MinActi.txt", numbers, SDString);

				//lengte van de activiteit toevoegen
				CurrentActivity.previousLength = CurrentActivity.length;
				CurrentActivity.length = 0;
				counterPM = 0;
				counterPauze = 0;
			}


			counter = 0;
		}

	}
}

//this function will clear the data and move the pointer
void moveBuffFifo(uint8_t buff)
{
    //move everything in the first buffer (leg buffer) one down because of invalid data
    if(buff == BUFF_LEG)
    {
        //set data to -1 so the buffer knows that location is not in use
        for(uint8_t moveData = 0; moveData < DATA_ORDER; moveData++)
        {
	    //reset data
            RTCMPUData[BUFF_LEG][buffer0Pointer][moveData] = BUFF_RESET;
        }
	//move the pointer
        buffer0Pointer++;
    } else if (buff == BUFF_ARM)    //move everything in the second (arm buffer) one down because of invalid data
    {
        //set data to -1 so the buffer knows that location is not in use
        for(uint8_t moveData = 0; moveData < DATA_ORDER; moveData++)
        {
	    //reset data
            RTCMPUData[BUFF_ARM][buffer1Pointer][moveData] = BUFF_RESET;
        }
	//move the pointer
        buffer1Pointer++;
    } else    //move everything in the buffers one down because the data has been checked
    {
        //set data to -1 so the buffer knows that location is not in use
        for(uint8_t moveData = 0; moveData < DATA_ORDER; moveData++)
        {
	    //reset data
            RTCMPUData[BUFF_LEG][buffer0Pointer][moveData] = BUFF_RESET;
            RTCMPUData[BUFF_ARM][buffer1Pointer][moveData] = BUFF_RESET;
        }
	//move the pointer
        buffer0Pointer++;
        buffer1Pointer++;
    }
    //reset the pointer because it reached the end of the buffer
    if(buffer0Pointer == BUFF_SIZE)
    {
        buffer0Pointer = 0;
    }
    if(buffer1Pointer == BUFF_SIZE)
    {
        buffer1Pointer = 0;
    }
}

//check if there is data to use for checking steps
void dataTimeCheckFifo()
{
    //checks if both buffers have data
    while(RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_DATA] != BUFF_RESET && RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_DATA] != BUFF_RESET)
    {
	//check if the seconds of the data of both buffers is equal, gives true if it isn't equal
        if(RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_SECONDS] != RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_SECONDS])
        {
	    //checks if which of the 2 buffers is the oldest
            if(RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_SECONDS] == MAX_SECONDS && RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_SECONDS] != 58)
            {
                //delete RTCMPUData[0] because it's 59 while RTCMPUData[1] is already in the next min
                moveBuffFifo(BUFF_LEG);
            } else if (RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_SECONDS] == MAX_SECONDS && RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_SECONDS] != 58)
            {
                //delete RTCMPUData[1] because it's 59 while RTCMPUData[0] is already in the next min
                moveBuffFifo(BUFF_ARM);
            } else if (RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_SECONDS] <= RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_SECONDS])
            {
                //delete RTCMPUData[0] because it's an older value then RTCMPUData[1]
                moveBuffFifo(BUFF_LEG);
            } else
            {
                //delete RTCMPUData[1] because it's an older value then RTCMPUData[0]
                moveBuffFifo(BUFF_ARM);
            }
        } else
        {    //the seconds of both data are the same
            step(RTCMPUData[BUFF_LEG][buffer0Pointer], RTCMPUData[BUFF_ARM][buffer1Pointer]);
            moveBuffFifo(BUFF_ALL);
        }
    }
}

//step function, counts if there was a step, should be in the while loop
void step(int16_t legData[4], int16_t armData[4])
{
	static float time = 0.0;
	static int counter = 0;
    //checks if one second has passed
	if(time != GPS.utc_time)
	{
		time = GPS.utc_time;

		if(counter < 60)
		{
			counter++;
		}
		else	//one minute
		{
			prevSteps = steps;
			steps = 0;
			counter = 0;
		}

    }

    //the data of the arm x3 because it is less strong
    int16_t armData3 = 3 * armData[BUFF_DATA];

    //checks if the data is actually a step
    //if((*legData[BUFF_DATA] + armData3) < STEP_LIMIT && !stepBlock)
    if((legData[BUFF_DATA] + armData3) < STEP_LIMIT && !stepBlock)
    {
        //add 1 to the counter
        steps++;
        CurrentActivity.totalDailySteps++;
        stepBlock = true;
    }

    //makes sure that the step has ended before recognizing a new one
    if((legData[BUFF_DATA] + armData3) >= STEP_RELEASE){
        stepBlock = false;
    }

    //HAL_Delay(250);
}

void RadioToBuffer(unsigned int id, unsigned int hours, unsigned int minutes, unsigned int seconds, int gyroZ)
{
	if(id)
	{
		RTCMPUData[id][buffer1TailPointer][BUFF_HOURS] = hours;
		RTCMPUData[id][buffer1TailPointer][BUFF_MINUTES] = minutes;
		RTCMPUData[id][buffer1TailPointer][BUFF_SECONDS] = seconds;
		RTCMPUData[id][buffer1TailPointer][BUFF_DATA] = gyroZ;
		buffer1TailPointer++;
	}
	else
	{
		RTCMPUData[id][buffer0TailPointer][BUFF_HOURS] = hours;
		RTCMPUData[id][buffer0TailPointer][BUFF_MINUTES] = minutes;
		RTCMPUData[id][buffer0TailPointer][BUFF_SECONDS] = seconds;
		RTCMPUData[id][buffer0TailPointer][BUFF_DATA] = gyroZ;
		buffer0TailPointer++;
	}

	if(buffer1TailPointer == BUFF_SIZE)
	{
		buffer1TailPointer = 0;
	}
	if(buffer0TailPointer == BUFF_SIZE)
	{
		buffer0TailPointer = 0;
	}
}
