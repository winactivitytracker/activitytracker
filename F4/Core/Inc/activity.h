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
	uint8_t 	ActivityStartTime;

	enum 		Activity{noMovement = 1, walking = 2, running = 3, unknown = 0};
	enum 		Activity currentActivity;
	enum 		Activity totalActivity;

}Activity_T;


void CalculateActivityAverage(uint8_t lastActiveMinute);
void getActivity();
char* activityToString(uint8_t activity);
void ActivityTotal();
void step(int16_t *legData[4], int16_t *armData[4]);
void dataTimeCheckFifo();
void moveBuffFifo(uint8_t buff);
void RadioToBuffer(unsigned int id, unsigned int hours, unsigned int minutes, unsigned int seconds, int gyroZ);

//defines
#define MAX_SECONDS 59
#define BUFF_LEG 0
#define BUFF_ARM 1
#define BUFF_ALL 2
#define BUFF_SIZE 20

#endif /* INC_ACTIVITY_H_ */


