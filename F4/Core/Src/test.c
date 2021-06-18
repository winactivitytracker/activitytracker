/*
 * test.c
 *
 *  Created on: 17 Jun 2021
 *      Author: THE SAPHIRE DRAGON
 */

#include "main.h"
#include "activity.h"
#include "test.h"
#include "sdcard.h"
#include "gps.h"

extern int16_t RTCMPUData[SELECT][BUFF_SIZE][DATA_ORDER];    //[which F0][buffer amount][0=hours,1=minutes,2=seconds,3=data]
extern uint8_t steps;
extern uint8_t prevSteps;
extern uint8_t buffer0Pointer;
extern uint8_t buffer1Pointer;
extern uint8_t buffer0TailPointer;
extern uint8_t buffer1TailPointer;
extern bool stepBlock;
extern Activity_T CurrentActivity;
extern GPS_t GPS;

void testAll()
{
	testActivity();
}

void testActivity()
{
	testTimeCheckFifo();
}

void testTimeCheckFifo()
{
	setEmptyBuffers();
	setBuffer0();
	setBuffer1();
	setBufferBoth();
	setBuffer1Late();
	setBuffer0Late();
	setBuffer1LateInPreviousMinute();
	setBuffer0LateInPreviousMinute();
	ActivitySetTest();
	activityToStringTest();
	totalActivtyTest();
	AcitivytLengthTest();
	AcitivytDailyLengthTest();
	setBuffer1Late30SecInPreviousMinute();
	setBuffer0Late30SecInPreviousMinute();
	setBufferStepWorthy();
}

//TEST1
//test what happens when the buffer is empty
void setEmptyBuffers()
{
	char* result;
	uint8_t oldBuffer0Pointer = 0, oldBuffer1Pointer = 0;
	buffer0Pointer = 0, buffer1Pointer = 0;
	RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_DATA] = BUFF_RESET;
	RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_HOURS] = BUFF_RESET;
	RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_MINUTES] = BUFF_RESET;
	RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_SECONDS] = BUFF_RESET;
	RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_DATA] = BUFF_RESET;
	RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_HOURS] = BUFF_RESET;
	RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_MINUTES] = BUFF_RESET;
	RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_SECONDS] = BUFF_RESET;
	dataTimeCheckFifo();

	if(oldBuffer0Pointer == buffer0Pointer && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_DATA] == BUFF_RESET && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_MINUTES] == BUFF_RESET && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_SECONDS] == BUFF_RESET)
	{
		result = "buffer 0 = correct";
	} else
	{
		result = "buffer 0 = incorrect";
	}

	if(oldBuffer1Pointer == buffer1Pointer && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_DATA] == BUFF_RESET && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_MINUTES] == BUFF_RESET && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_SECONDS] == BUFF_RESET)
	{
		result = "buffer 1 = correct";
	} else
	{
		result = "buffer 1 = incorrect";
	}
}

//TEST2
//test what happens when there is only data in the leg buffer
void setBuffer0()
{
	char* result;
	uint8_t oldBuffer0Pointer = 0, oldBuffer1Pointer = 0;
	buffer0Pointer = 0, buffer1Pointer = 0;
	RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_DATA] = -200;
	RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_HOURS] = 1;
	RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_MINUTES] = 1;
	RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_SECONDS] = 1;
	RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_DATA] = BUFF_RESET;
	RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_HOURS] = BUFF_RESET;
	RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_MINUTES] = BUFF_RESET;
	RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_SECONDS] = BUFF_RESET;
	dataTimeCheckFifo();

	if(oldBuffer0Pointer == buffer0Pointer && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_DATA] == -200 && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_MINUTES] == 1 && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_SECONDS] == 1)
	{
		result = "buffer 0 = correct";
	} else
	{
		result = "buffer 0 = incorrect";
	}

	if(oldBuffer1Pointer == buffer1Pointer && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_DATA] == BUFF_RESET && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_MINUTES] == BUFF_RESET && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_SECONDS] == BUFF_RESET)
	{
		result = "buffer 1 = correct";
	} else
	{
		result = "buffer 1 = incorrect";
	}
}

//TEST3
//test what happens when there is only data in the arm buffer
void setBuffer1()
{
	char* result;
	uint8_t oldBuffer0Pointer = 0, oldBuffer1Pointer = 0;
	buffer0Pointer = 0, buffer1Pointer = 0;
	RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_DATA] = BUFF_RESET;
	RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_HOURS] = BUFF_RESET;
	RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_MINUTES] = BUFF_RESET;
	RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_SECONDS] = BUFF_RESET;
	RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_DATA] = -200;
	RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_HOURS] = 1;
	RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_MINUTES] = 1;
	RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_SECONDS] = 1;
	dataTimeCheckFifo();

	if(oldBuffer0Pointer == buffer0Pointer && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_DATA] == BUFF_RESET && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_MINUTES] == BUFF_RESET && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_SECONDS] == BUFF_RESET)
	{
		result = "buffer 0 = correct";
	} else
	{
		result = "buffer 0 = incorrect";
	}

	if(oldBuffer1Pointer == buffer1Pointer && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_DATA] == -200 && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_MINUTES] == 1 && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_SECONDS] == 1)
	{
		result = "buffer 1 = correct";
	} else
	{
		result = "buffer 1 = incorrect";
	}
}

//TEST4
//test what happens when there is data in both buffers with the same seconds
void setBufferBoth()
{
	char* result;
	uint8_t oldBuffer0Pointer = 0, oldBuffer1Pointer = 0;
	buffer0Pointer = 0, buffer1Pointer = 0;
	RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_DATA] = -200;
	RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_HOURS] = 1;
	RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_MINUTES] = 1;
	RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_SECONDS] = 1;
	RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_DATA] = -200;
	RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_HOURS] = 1;
	RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_MINUTES] = 1;
	RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_SECONDS] = 1;
	dataTimeCheckFifo();

	if((oldBuffer0Pointer+1) == buffer0Pointer && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_DATA] == BUFF_RESET && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_MINUTES] == BUFF_RESET && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_SECONDS] == BUFF_RESET)
	{
		result = "buffer 0 = correct";
	} else
	{
		result = "buffer 0 = incorrect";
	}

	if((oldBuffer1Pointer+1) == buffer1Pointer && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_DATA] == BUFF_RESET && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_MINUTES] == BUFF_RESET && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_SECONDS] == BUFF_RESET)
	{
		result = "buffer 1 = correct";
	} else
	{
		result = "buffer 1 = incorrect";
	}
}

//TEST5
//test what happens when there is data in both buffers but the arm buffer is one second behind
void setBuffer1Late()
{
	char* result;
	uint8_t oldBuffer0Pointer = 0, oldBuffer1Pointer = 0;
	buffer0Pointer = 0, buffer1Pointer = 0;
	RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_DATA] = -200;
	RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_HOURS] = 1;
	RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_MINUTES] = 1;
	RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_SECONDS] = 2;
	RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_DATA] = -200;
	RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_HOURS] = 1;
	RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_MINUTES] = 1;
	RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_SECONDS] = 1;
	dataTimeCheckFifo();

	if(oldBuffer0Pointer == buffer0Pointer && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_DATA] == -200 && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_MINUTES] == 1 && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_SECONDS] == 2)
	{
		result = "buffer 0 = correct";
	} else
	{
		result = "buffer 0 = incorrect";
	}

	if((oldBuffer1Pointer+1) == buffer1Pointer && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_DATA] == BUFF_RESET && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_MINUTES] == BUFF_RESET && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_SECONDS] == BUFF_RESET)
	{
		result = "buffer 1 = correct";
	} else
	{
		result = "buffer 1 = incorrect";
	}
}

//TEST6
//test what happens when there is data in both buffers but the leg buffer is one second behind
void setBuffer0Late()
{
	char* result;
	uint8_t oldBuffer0Pointer = 0, oldBuffer1Pointer = 0;
	buffer0Pointer = 0, buffer1Pointer = 0;
	RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_DATA] = -200;
	RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_HOURS] = 1;
	RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_MINUTES] = 1;
	RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_SECONDS] = 1;
	RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_DATA] = -200;
	RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_HOURS] = 1;
	RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_MINUTES] = 1;
	RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_SECONDS] = 2;
	dataTimeCheckFifo();

	if((oldBuffer0Pointer+1) == buffer0Pointer && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_DATA] == BUFF_RESET && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_MINUTES] == BUFF_RESET && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_SECONDS] == BUFF_RESET)
	{
		result = "buffer 0 = correct";
	} else
	{
		result = "buffer 0 = incorrect";
	}

	if(oldBuffer1Pointer == buffer1Pointer && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_DATA] == -200 &&  RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_MINUTES] == 1 && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_SECONDS] == 2)
	{
		result = "buffer 1 = correct";
	} else
	{
		result = "buffer 1 = incorrect";
	}
}

//TEST7
//test what happens when there is data in both buffers but the arm buffer is one second behind in the previous buffer
void setBuffer1LateInPreviousMinute()
{
	char* result;
	uint8_t oldBuffer0Pointer = 0, oldBuffer1Pointer = 0;
	uint8_t test;
	buffer0Pointer = 0, buffer1Pointer = 0;
	RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_DATA] = -200;
	RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_HOURS] = 1;
	RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_MINUTES] = 2;
	RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_SECONDS] = 0;
	RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_DATA] = -200;
	RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_HOURS] = 1;
	RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_MINUTES] = 1;
	RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_SECONDS] = 59;
	dataTimeCheckFifo();

	if(oldBuffer0Pointer == buffer0Pointer && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_DATA] == -200 && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_MINUTES] == 2 && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_SECONDS] == 0)
	{
		result = "buffer 0 = correct";
	} else
	{
		result = "buffer 0 = incorrect";
	}

	if((oldBuffer1Pointer+1) == buffer1Pointer && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_DATA] == BUFF_RESET && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_MINUTES] == BUFF_RESET && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_SECONDS] == BUFF_RESET)
	{
		result = "buffer 1 = correct";
	} else
	{
		result = "buffer 1 = incorrect";
	}
}

//TEST8
//test what happens when there is data in both buffers but the leg buffer is one second behind in the previous buffer
void setBuffer0LateInPreviousMinute()
{
	char* result;
	uint8_t oldBuffer0Pointer = 0, oldBuffer1Pointer = 0;
	buffer0Pointer = 0, buffer1Pointer = 0;
	RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_DATA] = -200;
	RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_HOURS] = 1;
	RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_MINUTES] = 1;
	RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_SECONDS] = 59;
	RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_DATA] = -200;
	RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_HOURS] = 1;
	RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_MINUTES] = 2;
	RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_SECONDS] = 0;
	dataTimeCheckFifo();

	if((oldBuffer0Pointer+1) == buffer0Pointer && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_DATA] == BUFF_RESET && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_MINUTES] == BUFF_RESET && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_SECONDS] == BUFF_RESET)
	{
		result = "buffer 0 = correct";
	} else
	{
		result = "buffer 0 = incorrect";
	}

	if(oldBuffer1Pointer == buffer1Pointer && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_DATA] == -200 && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_MINUTES] == 2 && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_SECONDS] == 0)
	{
		result = "buffer 1 = correct";
	} else
	{
		result = "buffer 1 = incorrect";
	}
}

//TEST9
//test what happens when there is data in both buffers but the arm buffer is 30 second behind in the previous buffer
void setBuffer1Late30SecInPreviousMinute()
{
	char* result;
	uint8_t oldBuffer0Pointer = 0, oldBuffer1Pointer = 0;
	buffer0Pointer = 0, buffer1Pointer = 0;
	RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_DATA] = -200;
	RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_HOURS] = 1;
	RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_MINUTES] = 2;
	RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_SECONDS] = 15;
	RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_DATA] = -200;
	RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_HOURS] = 1;
	RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_MINUTES] = 1;
	RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_SECONDS] = 45;
	dataTimeCheckFifo();

	if(oldBuffer0Pointer == buffer0Pointer && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_DATA] == -200 && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_MINUTES] == 2 && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_SECONDS] == 15)
	{
		result = "buffer 0 = correct";
	} else
	{
		result = "buffer 0 = incorrect";
	}

	if((oldBuffer1Pointer+1) == buffer1Pointer && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_DATA] == BUFF_RESET && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_MINUTES] == BUFF_RESET && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_SECONDS] == BUFF_RESET)
	{
		result = "buffer 1 = correct";
	} else
	{
		result = "buffer 1 = incorrect";
	}
}

//TEST10
//test what happens when there is data in both buffers but the leg buffer is 30 second behind in the previous buffer
void setBuffer0Late30SecInPreviousMinute()
{
	char* result;
	uint8_t oldBuffer0Pointer = 0, oldBuffer1Pointer = 0;
	buffer0Pointer = 0, buffer1Pointer = 0;
	RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_DATA] = -200;
	RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_HOURS] = 1;
	RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_MINUTES] = 1;
	RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_SECONDS] = 45;
	RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_DATA] = -200;
	RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_HOURS] = 1;
	RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_MINUTES] = 2;
	RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_SECONDS] = 15;
	dataTimeCheckFifo();

	if(oldBuffer0Pointer == buffer0Pointer && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_DATA] == BUFF_RESET && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_MINUTES] == BUFF_RESET && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_SECONDS] == BUFF_RESET)
	{
		result = "buffer 0 = correct";
	} else
	{
		result = "buffer 0 = incorrect";
	}

	if((oldBuffer1Pointer+1) == buffer1Pointer && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_DATA] == -200 && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_MINUTES] == 2 && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_SECONDS] == 15)
	{
		result = "buffer 1 = correct";
	} else
	{
		result = "buffer 1 = incorrect";
	}
}

//TEST11
//test what happens when there is step worthy data in both buffers
void setBufferStepWorthy()
{
	char* result;
	uint8_t oldBuffer0Pointer = 0, oldBuffer1Pointer = 0, oldSteps = 0;
	buffer0Pointer = 0, buffer1Pointer = 0, steps = 0;
	RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_DATA] = -2000;
	RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_HOURS] = 1;
	RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_MINUTES] = 1;
	RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_SECONDS] = 1;
	RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_DATA] = -1000;
	RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_HOURS] = 1;
	RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_MINUTES] = 1;
	RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_SECONDS] = 1;
	dataTimeCheckFifo();

	if((oldBuffer0Pointer+1) == buffer0Pointer && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_DATA] == BUFF_RESET && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_MINUTES] == BUFF_RESET && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_SECONDS] == BUFF_RESET)
	{
		result = "buffer 0 = correct";
	} else
	{
		result = "buffer 0 = incorrect";
	}

	if((oldBuffer1Pointer+1) == buffer1Pointer && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_DATA] == BUFF_RESET && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_MINUTES] == BUFF_RESET && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_SECONDS] == BUFF_RESET)
	{
		result = "buffer 1 = correct";
	} else
	{
		result = "buffer 1 = incorrect";
	}

	if((oldSteps+1) == steps && stepBlock)
	{
		result = "step = correct";
	} else
	{
		result = "step = incorrect";
	}
}

void ActivitySetTest()
{
	int count = 0;

	//act
	CalculateActivityAverage(walking);
	CalculateActivityAverage(noMovement);
	CalculateActivityAverage(running);
	CalculateActivityAverage(unknown);
	CalculateActivityAverage(unknownIndoor);

	//assert
	if(CurrentActivity.activityTotal[walking] == 1)
	{
		count++;
	}
	if(CurrentActivity.activityTotal[noMovement] == 1)
	{
		count++;
	}
	if(CurrentActivity.activityTotal[running] == 1)
	{
		count++;
	}
	if(CurrentActivity.activityTotal[unknown] == 1)
	{
		count++;
	}
	if(CurrentActivity.activityTotal[unknownIndoor] == 1)
	{
		count++;
	}

	assert(count == 5);

	//clear the buffer because its dummy data
	for (int i = 0; i < sizeof(CurrentActivity.activityTotal); i++)
	{
		CurrentActivity.activityTotal[i] = 0;
		count--;
	}

	if(CurrentActivity.activityTotal[walking] == 0)
	{
		count++;
	}
	if(CurrentActivity.activityTotal[noMovement] == 0)
	{
		count++;
	}
	if(CurrentActivity.activityTotal[running] == 0)
	{
		count++;
	}
	if(CurrentActivity.activityTotal[unknown] == 0)
	{
		count++;
	}
	if(CurrentActivity.activityTotal[unknownIndoor] == 0)
	{
		count++;
	}

	//check if the buffer is cleared
	assert(count == 5);

}

void activityToStringTest()
{

	int count = 0;
	//act
	char* walkingString = activityToString(walking);
	char* runningString = activityToString(running);
	char* NMString = activityToString(noMovement);
	char* unknownString = activityToString(unknown);
	char* unknownIndoorString = activityToString(unknownIndoor);



	//assert
	if(!strcmp(walkingString, "Wandelen"))
	{
		count++;
	}

	if(!strcmp(runningString, "Hardlopen"))
	{
		count++;
	}

	if(!strcmp(NMString, "Geen beweging"))
	{
		count++;
	}

	if(!strcmp(unknownString, "Onbekend"))
	{
		count++;
	}

	if(!strcmp(unknownIndoorString, "Indoor Activiteit"))
	{
		count++;
	}

	assert(count == 5);
}

void totalActivtyTest()
{
	//act
	//set running as the most common activity
	for (int i = 0; i < 5; ++i) {
		CalculateActivityAverage(walking);
	}
	for (int i = 0; i < 7; ++i) {
			CalculateActivityAverage(running);
	}
	for (int i = 0; i < 2; ++i) {
				CalculateActivityAverage(noMovement);
	}

	ActivityTotal();

	//assert
	//passed if running is total activity
	assert(CurrentActivity.totalActivity == running);

}

void AcitivytLengthTest()
{
		int AL = 5;
		//act
		//set a activity with 5 minutes of walking
		for (int i = 0; i < AL; ++i) {
			CalculateActivityAverage(walking);
		}


		//assert
		//passed if the current activity length is 5

		assert(CurrentActivity.length == AL);

		ActivityTotal();

		//passed if the activity length was 5
		assert(CurrentActivity.previousLength == AL);

}

void AcitivytDailyLengthTest()
{
		int ALW = 5;
		int ALR = 7;
		int ALTotal = ALW + ALR;
		GPS.utc_time = 123500.0; //timer dummy value just for testing
		CurrentActivity.activeDailyMinutes = 0; //set nul so the dummy values are the only used values
		//act
		//set a activity with 5 minutes of walking
		for (int i = 0; i < ALW; ++i) {
			CalculateActivityAverage(walking);
		}

		ActivityTotal();

		//set a activity with 7 minutes of walking
		for (int i = 0; i < ALR; ++i) {
			CalculateActivityAverage(running);
		}

		ActivityTotal();

		//assert
		//passed if the total daily activy minutes are 12 (5+7)

		assert(CurrentActivity.activeDailyMinutes == ALTotal);


		//if its 12pm the timer has activeDailyminutes are reset
		GPS.utc_time = 0;
		getTime();

		assert(CurrentActivity.activeDailyMinutes == 0);

}
