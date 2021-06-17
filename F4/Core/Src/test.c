/*
 * test.c
 *
 *  Created on: 17 Jun 2021
 *      Author: THE SAPHIRE DRAGON
 */

#include "main.h"
#include "activity.h"
#include "test.h"

extern int16_t RTCMPUData[SELECT][BUFF_SIZE][DATA_ORDER];    //[which F0][buffer amount][0=hours,1=minutes,2=seconds,3=data]
extern uint8_t steps;
extern uint8_t prevSteps;
extern uint8_t buffer0Pointer;
extern uint8_t buffer1Pointer;
extern uint8_t buffer0TailPointer;
extern uint8_t buffer1TailPointer;
extern bool stepBlock;

void testAll()
{

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
	
	if(oldBuffer0Pointer == buffer0Pointer && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_DATA] == BUFF_RESET && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_MINUTES] == BUFF_RESET && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_SECONDS] == BUFF_RESET)
	{
		result = "buffer 0 = correct";
	} else
	{
		result = "buffer 0 = incorrect";
	}
	printTestResult("Activity", "Activity set empty buffers", result);
	
	if(oldBuffer1Pointer == buffer1Pointer && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_DATA] == BUFF_RESET && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_MINUTES] == BUFF_RESET && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_SECONDS] == BUFF_RESET)
	{
		result = "buffer 1 = correct";
	} else
	{
		result = "buffer 1 = incorrect";
	}
	printTestResult("Activity", "Activity set empty buffers", result);
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
	
	if(oldBuffer0Pointer == buffer0Pointer && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_DATA] == -200 && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_MINUTES] == 1 && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_SECONDS] == 1)
	{
		result = "buffer 0 = correct";
	} else
	{
		result = "buffer 0 = incorrect";
	}
	printTestResult("Activity", "Activity set buffer leg", result);
	
	if(oldBuffer1Pointer == buffer1Pointer && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_DATA] == BUFF_RESET && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_MINUTES] == BUFF_RESET && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_SECONDS] == BUFF_RESET)
	{
		result = "buffer 1 = correct";
	} else
	{
		result = "buffer 1 = incorrect";
	}
	printTestResult("Activity", "Activity set buffer leg", result);
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
	
	if(oldBuffer0Pointer == buffer0Pointer && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_DATA] == BUFF_RESET && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_MINUTES] == BUFF_RESET && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_SECONDS] == BUFF_RESET)
	{
		result = "buffer 0 = correct";
	} else
	{
		result = "buffer 0 = incorrect";
	}
	printTestResult("Activity", "Activity set buffer arm", result);
	
	if(oldBuffer1Pointer == buffer1Pointer && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_DATA] == -200 && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_MINUTES] == 1 && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_SECONDS] == 1)
	{
		result = "buffer 1 = correct";
	} else
	{
		result = "buffer 1 = incorrect";
	}
	printTestResult("Activity", "Activity set buffer arm", result);
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
	
	if((oldBuffer0Pointer+1) == buffer0Pointer && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_DATA] == BUFF_RESET && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_MINUTES] == BUFF_RESET && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_SECONDS] == BUFF_RESET)
	{
		result = "buffer 0 = correct";
	} else
	{
		result = "buffer 0 = incorrect";
	}
	printTestResult("Activity", "Activity set both buffers", result);
	
	if((oldBuffer1Pointer+1) == buffer1Pointer && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_DATA] == BUFF_RESET && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_MINUTES] == BUFF_RESET && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_SECONDS] == BUFF_RESET)
	{
		result = "buffer 1 = correct";
	} else
	{
		result = "buffer 1 = incorrect";
	}
	printTestResult("Activity", "Activity set both buffers", result);
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
	
	if(oldBuffer0Pointer == buffer0Pointer && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_DATA] == -200 && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_MINUTES] == 1 && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_SECONDS] == 2)
	{
		result = "buffer 0 = correct";
	} else
	{
		result = "buffer 0 = incorrect";
	}
	printTestResult("Activity", "Activity set both buffers, arm buffer is late", result);
	
	if((oldBuffer1Pointer+1) == buffer1Pointer && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_DATA] == BUFF_RESET && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_MINUTES] == BUFF_RESET && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_SECONDS] == BUFF_RESET)
	{
		result = "buffer 1 = correct";
	} else
	{
		result = "buffer 1 = incorrect";
	}
	printTestResult("Activity", "Activity set both buffers, arm buffer is late", result);
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
	
	if((oldBuffer0Pointer+1) == buffer0Pointer && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_DATA] == BUFF_RESET && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_MINUTES] == BUFF_RESET && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_SECONDS] == BUFF_RESET)
	{
		result = "buffer 0 = correct";
	} else
	{
		result = "buffer 0 = incorrect";
	}
	printTestResult("Activity", "Activity set both buffers, leg buffer is late", result);
	
	if(oldBuffer1Pointer == buffer1Pointer && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_DATA] == -200 && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_MINUTES] == BUFF_RESET && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_MINUTES] == 1 && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_SECONDS] == 2)
	{
		result = "buffer 1 = correct";
	} else
	{
		result = "buffer 1 = incorrect";
	}
	printTestResult("Activity", "Activity set both buffers, leg buffer is late", result);
}

//TEST7
//test what happens when there is data in both buffers but the arm buffer is one second behind in the previous buffer
void setBuffer1LateInPreviousMinute()
{
	char* result;
	uint8_t oldBuffer0Pointer = 0, oldBuffer1Pointer = 0;
	buffer0Pointer = 0, buffer1Pointer = 0;
	RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_DATA] = -200;
	RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_HOURS] = 1;
	RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_MINUTES] = 2;
	RTCMPUData[BUFF_LEG][buffer0Pointer][BUFF_SECONDS] = 0;
	RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_DATA] = -200;
	RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_HOURS] = 1;
	RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_MINUTES] = 1;
	RTCMPUData[BUFF_ARM][buffer1Pointer][BUFF_SECONDS] = 59;
	
	if(oldBuffer0Pointer == buffer0Pointer && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_DATA] == -200 && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_MINUTES] == 2 && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_SECONDS] == 0)
	{
		result = "buffer 0 = correct";
	} else
	{
		result = "buffer 0 = incorrect";
	}
	printTestResult("Activity", "Activity set both buffers, arm buffer is late and still in the previous minute", result);
	
	if((oldBuffer1Pointer+1) == buffer1Pointer && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_DATA] == BUFF_RESET && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_MINUTES] == BUFF_RESET && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_SECONDS] == BUFF_RESET)
	{
		result = "buffer 1 = correct";
	} else
	{
		result = "buffer 1 = incorrect";
	}
	printTestResult("Activity", "Activity set both buffers, arm buffer is late and still in the previous minute", result);
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
	
	if((oldBuffer0Pointer+1) == buffer0Pointer && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_DATA] == BUFF_RESET && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_MINUTES] == BUFF_RESET && RTCMPUData[BUFF_LEG][oldBuffer0Pointer][BUFF_SECONDS] == BUFF_RESET)
	{
		result = "buffer 0 = correct";
	} else
	{
		result = "buffer 0 = incorrect";
	}
	printTestResult("Activity", "Activity set both buffers, leg buffer is late and still in the previous minute", result);
	
	if(oldBuffer1Pointer == buffer1Pointer && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_DATA] == -200 && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_MINUTES] == 2 && RTCMPUData[BUFF_ARM][oldBuffer1Pointer][BUFF_SECONDS] == 0)
	{
		result = "buffer 1 = correct";
	} else
	{
		result = "buffer 1 = incorrect";
	}
	printTestResult("Activity", "Activity set both buffers, leg buffer is late and still in the previous minute", result);
}

void printTestResult(char* testObject, char* currentTest, char* result)
{
	char localOject[30] = "";
	char localTest[30] = "";
	char localResult[30] = "";
	sprintf(localOject, "%s", testObject);
	sprintf(localTest, "%s", currentTest);
	sprintf(localResult, "%s", result);
//	HAL_UART_Transmit(&huart1, "This is the test of ", sizeof("This is the test of "), HAL_MAX_DELAY);
//	HAL_UART_Transmit(&huart1, localOject, sizeof(localOject), HAL_MAX_DELAY);
//	HAL_UART_Transmit(&huart1, " and the current test is ", sizeof(" and the current test = "), HAL_MAX_DELAY);
//	HAL_UART_Transmit(&huart1, localTest, sizeof(localTest), HAL_MAX_DELAY);
//	HAL_UART_Transmit(&huart1, "\n", sizeof("\n"), HAL_MAX_DELAY);
//	HAL_UART_Transmit(&huart1, "The result of this test is: ", sizeof("The result of this test is: "), HAL_MAX_DELAY);
//	HAL_UART_Transmit(&huart1, localResult, sizeof(localResult), HAL_MAX_DELAY);
//	HAL_UART_Transmit(&huart1, "\n", sizeof("\n"), HAL_MAX_DELAY);

	writeFile("test", "This is the test of ");
	writeFile("test", localOject);
	writeFile("test", " and the current test is ");
	writeFile("test", localTest);
	writeFile("test", "\n");
	writeFile("test", "The result of this test is: ");
	writeFile("test", localResult);
	writeFile("test", "\n");
	
	HAL_Delay(500);
}

