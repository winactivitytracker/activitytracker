/*
 * transmitter.cpp
 *
 *  Created on: 20 May 2021
 *      Author: arteeh
 */

#include "transmitter.h"

// Pop the next message from the outbound queue
// and add it to the outbound buffer
void transmitter::fillBuffer()
{
	message m = messages.front();
	messages.pop_front();

	string s = m.getMessageString();

	for(uint8_t i = 0; i < s.length(); i++)
	{
		bitset<8> c = s[i];
		buffer.push_back(c);
	}
	bufferPointer = 0;
}

uint8_t transmitter::getNextBit()
{
	if(buffer.empty())
	{
		return NO_NEW_BITS;
	}

	if(bufferPointer > 7)
	{
		buffer.pop_front();
		bufferPointer = 0;

		if(buffer.empty())
		{
			return NO_NEW_BITS;
		}
	}

	uint8_t retVal = buffer.front()[bufferPointer];

	bufferPointer++;

	return retVal;
}

// The send function adds a message to the queue.
// The interrupt handler will notice and start sending this message bit by bit.
void transmitter::send(string msg)
{
	message m(msg);
	messages.push_back(m);
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
						state = STOP_HIGH;
						break;
				}
				break;
			case MSG_LOW:
				SEND_LOW;
				RED_LOW;
				state = MSG_HIGH;
				break;
			case STOP_HIGH:
				SEND_HIGH;
				RED_HIGH;
				holdFor = LENGTH_START;
				state = STOP_LOW;
				break;
			case STOP_LOW:
				SEND_LOW;
				RED_LOW;
				holdFor = LENGTH_START;
				state = IDLE;
				break;
			case IDLE:
				if(!messages.empty())
				{
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
