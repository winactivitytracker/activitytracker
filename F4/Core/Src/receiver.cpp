/*
 * receiver.cpp
 *
 *  Created on: 20 May 2021
 *      Author: arteeh
 */

#include "receiver.h"

// Turn on the timer to start listening for messages
void receiver::enable()
{
	HAL_TIM_Base_Start_IT(RECEIVER_TIMER);
}

// Turn off the timer to stop listening for messages
void receiver::disable()
{
	HAL_TIM_Base_Stop_IT(RECEIVER_TIMER);
}

// Get the contents of the inbound buffer
// and put it in a message in the inbound queue
void receiver::clearBuffer()
{
	string m = "";

	while(!buffer.empty())
	{
		bitset<8> c = buffer.front();
		buffer.pop_front();

		m += (char) c.to_ulong();
	}

	messages.push_back(m);

	bitPointer = 0;
}

// Add a single bit to the buffer
void receiver::addToBuffer(bool bit)
{
	bitset<8> c;
	if(bitPointer > 7 || buffer.empty())
	{
		// Make a new char and set the first bit
		c.set(0, bit);
		buffer.push_back(c);
		bitPointer = 1;
	}
	else
	{
		// Grab the existing char and set the bit on it
		c = buffer.back();
		// 7- if the sent bytes are mirrored
		//c.set(7-bitPointer, bit);
		c.set(bitPointer, bit);
		buffer.pop_back();
		buffer.push_back(c);
		bitPointer++;
	}

	string s = buffer.back().to_string();
}

bool receiver::checkMessage()
{
	// If messages are empty, return false. If full, return true
	return !(messages.empty());
}

string receiver::popMessage()
{
	string retVal = messages.front();
	messages.pop_front();
	return retVal;
}

void receiver::tick()
{
	bool on = HAL_GPIO_ReadPin(RADIO_RECEIVE_GPIO_Port,RADIO_RECEIVE_Pin);
	static bool started = false;
	static bool isCounting = false;
	static uint16_t counter = 0;

	if(on && isCounting)
	{
		counter++;
		// For seeing how many counts a bit is
		//HAL_GPIO_TogglePin(GREEN_GPIO_Port,GREEN_Pin);
	}
	else if(on && !isCounting)
	{
		counter = 0;
		isCounting = true;
	}
	else if(!on && isCounting)
	{
		if(!started)
		{
			if((START_MIN < counter) && (counter < START_MAX))
			{
				HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,GPIO_PIN_SET);
				// Remember that there has been a start bit
				started = true;
			}
		}
		else
		{
			if((ZERO_MIN < counter) && (counter < ZERO_MAX))
			{
				// Add zero to buffer
				addToBuffer(false);
			}
			else if((ONE_MIN < counter) && (counter < ONE_MAX))
			{
				// Add one to buffer
				addToBuffer(true);
			}
			else if((STOP_MIN < counter) && (counter < STOP_MAX))
			{
				// Forget the start bit so we can wait
				// for the next message
				started = false;

				// If a second high point the length of a
				// start bit is found (the stop bit), put
				// the buffer in the message queue
				clearBuffer();

				HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,GPIO_PIN_RESET);
			}
		}

		// Stop the timer
		counter = 0;
		isCounting = false;
	}
}
