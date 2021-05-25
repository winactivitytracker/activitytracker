/*
 * transmitter.cpp
 *
 *  Created on: 20 May 2021
 *      Author: arteeh
 */

#include "transmitter.h"


void transmitter::enable()
{
	HAL_TIM_Base_Start_IT(&htim15);
}

void transmitter::disable()
{
	HAL_TIM_Base_Stop_IT(&htim15);
}

// Pop the next message from the outbound queue
// and add it to the outbound buffer
void transmitter::fillBuffer()
{
	string m = messages.front();
	messages.pop_front();

	for(uint8_t i = 0; i < m.length(); i++)
	{
		bitset<8> c = m[i];
		buffer.push_back(c);
	}
	bitPointer = 0;
}

// Get the next bit to send
uint8_t transmitter::getNextBit()
{
	if(buffer.empty())
	{
		return NO_NEW_BITS;
	}

	if(bitPointer > 7)
	{
		buffer.pop_front();
		bitPointer = 0;

		if(buffer.empty())
		{
			return NO_NEW_BITS;
		}
	}

	uint8_t retVal = buffer.front()[bitPointer];

	bitPointer++;

	return retVal;
}

// The send function adds a message to the queue.
// The interrupt handler will notice and start sending this message bit by bit.
void transmitter::send(string message)
{
	enable();
	messages.push_back(message);
}

// Function to be called by the interrupt handler.
// Standard version with start and stop bits, using a state machine.
void transmitter::tick()
{
	// Counter to keep track of how long a pin should be set or unset
	static uint8_t holdFor = 1;

	// State machine variable to keep track of message progress across interrupts
	static uint8_t state = START_HIGH;

	// When done waiting, move on to the next instruction
	if(holdFor <= 1)
	{
		switch(state)
		{
			case START_HIGH:
				SEND_HIGH;
				RED_HIGH;
				holdFor = LENGTH_START;
				state = START_LOW;
				break;
			case START_LOW:
				SEND_LOW;
				RED_LOW;
				holdFor = LENGTH_START;
				state = MSG_HIGH;
				// Fill the buffer with the message to be sent
				fillBuffer();
				break;
			case MSG_HIGH:
				switch(getNextBit())
				{
					case 0:
						SEND_HIGH;
						RED_HIGH;
						holdFor = LENGTH_ZERO;
						state = MSG_LOW;
						break;
					case 1:
						SEND_HIGH;
						RED_HIGH;
						holdFor = LENGTH_ONE;
						state = MSG_LOW;
						break;
					// When getNextBit() returns this, the message is done and the next one can be selected
					case NO_NEW_BITS:
						SEND_LOW;
						RED_LOW;
						state = STOP_LOW;
						break;
				}
				break;
			case MSG_LOW:
				SEND_LOW;
				RED_LOW;
				state = MSG_HIGH;
				break;
			case STOP_LOW:
				SEND_LOW;
				RED_LOW;
				holdFor = LENGTH_STOP;
				state = STOP_HIGH;
				break;
			case STOP_HIGH:
				SEND_HIGH;
				RED_HIGH;
				holdFor = LENGTH_STOP;
				state = IDLE;
				break;
			case IDLE:
				SEND_LOW;
				RED_LOW;
				if(messages.empty())
				{
					disable();
				}
				else
				{
					holdFor = 10;
					bitPointer = 0;
					state = START_HIGH;
				}
				break;
		}
	}
	else
	{
		holdFor--;
	}
}
