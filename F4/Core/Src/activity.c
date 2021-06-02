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

Activity_T CurrentActivity;
extern GPS_t GPS;

void getActivity()
{
		if(0 < GPS.speed_km && GPS.speed_km < 2.5)
		{
			CurrentActivity.currentActivity = noMovement;	//nomovement
		}
		else if(GPS.speed_km >= 2.5 && GPS.speed_km < 7.0)
		{
			CurrentActivity.currentActivity = walking;	//walking
		}
		else if(GPS.speed_km >= 7.0 && GPS.speed_km < 15.0)
		{
			CurrentActivity.currentActivity = running;
		}
		else
		{
			CurrentActivity.currentActivity =  unknown;
		}
}

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
		default:
			string = "onbekend";
			break;
	}
	return string;
}

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
		default:
			CurrentActivity.activityTotal[unknown]++;
			break;
	}
}

void ActivityTotal()
{
	static float time;
	static uint8_t counter = 0, counterPM = 0, counterPauze = 0;
	static uint8_t trackActivity[4];
	char* SDString = "";

	if(time != GPS.utc_time)
	{
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
				default:
					trackActivity[unknown]++;
					break;
			}

			counter++;
		}
		else
		{
			uint8_t current = 0; //index = 0;

			for(int i = 0; i < 4; i++)
			{
				if(current <= trackActivity[i])
				{
					current = trackActivity[i];
					trackActivity[i] = 0;
					CurrentActivity.lastActiveMinute = i;
				}
			}

			if(counterPM < maxAcitivitySize)
			{
				if((CurrentActivity.lastActiveMinute == walking || CurrentActivity.lastActiveMinute == running))
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
					if(counterPauze < 2)
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
				char numbers[10];
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
