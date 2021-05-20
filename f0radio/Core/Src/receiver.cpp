/*
 * receiver.cpp
 *
 *  Created on: 20 May 2021
 *      Author: arteeh
 */

#include "receiver.h"

// Get the contents of the inbound buffer
// and put it in a message in the inbound queue
void receiver::clearBuffer()
{
	message m(buffer.to_string());
	bitset<1000> b;
	buffer = b;
	bufferPointer = 0;
	messages.push_back(m);
}

bool receiver::checkMessage()
{
	if(messages.empty())
	{
		return false;
	}
	else
	{
		return true;
	}
}

message receiver::popMessage()
{
	message retVal = messages.front();
	messages.pop_front();
	return retVal;
}

void receiver::tick()
{
	static bool started = false;
	static bool isCounting = false;
	static uint16_t counter = 0;
	bool on = RECEIVE;
	/*
	bool on;
	if(HAL_GPIO_ReadPin(RECEIVE_GPIO_Port,RECEIVE_Pin) == GPIO_PIN_SET)
	{
		on = true;
	}
	else
	{
		on = false;
	};

	HAL_GPIO_TogglePin(GREEN_GPIO_Port,GREEN_Pin);
	*/

	if(on)
	{
		if(isCounting)
		{
			counter++;
		}
		else
		{
			counter = 0;
			isCounting = true;
		}
	}
	else
	{
		if(isCounting)
		{
			if((START_MIN < counter) && (counter < START_MAX))
			{
				// Stop the timer
				isCounting = false;
				counter = 0;

				if(!started)
				{
					// Remember that there has been a start bit
					started = true;
				}
				else
				{
					// If a second high point the length of a
					// start bit is found (the stop bit), put
					// the buffer in the message queue
					clearBuffer();
				}
			}
			else if((ZERO_MIN < counter) && (counter < ZERO_MAX))
			{
				// Stop the timer
				isCounting = false;
				counter = 0;

				// Add zero to buffer
				buffer.set(bufferPointer,0);
				bufferPointer++;
			}
			else if((ONE_MIN < counter) && (counter < ONE_MAX))
			{
				// Stop the timer
				isCounting = false;
				counter = 0;

				// Add one to buffer
				buffer.set(bufferPointer,1);
				bufferPointer++;
			}
		}
	}
}
