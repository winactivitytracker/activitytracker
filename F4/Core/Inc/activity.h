/*
 * activity.h
 *
 *  Created on: 28 mei 2021
 *      Author: joel-
 */

#ifndef INC_ACTIVITY_H_
#define INC_ACTIVITY_H_

typedef struct{
	uint8_t		length;
	uint8_t		activeDailyMinutes;
	uint8_t		lastActiveMinute;
	uint8_t 	activityTotal[4];
	uint8_t 	previousLength;

	enum 		Activity{noMovement = 1, walking = 2, running = 3, unknown = 0};
	enum 		Activity currentActivity;
	enum 		Activity totalActivity;

}Activity_T;

void CalculateActivityAverage(uint8_t lastActiveMinute);
void getActivity();
char* activityToString(uint8_t activity);
void ActivityTotal();

#endif /* INC_ACTIVITY_H_ */


