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
