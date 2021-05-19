// Class to control the radio, and more importantly the buffer of messages to be sent

#include "radio.h"

uint8_t radio::getNextOutboundBit()
{
	uint8_t retVal;

	auto iterator = outboundMessages.begin();
	advance(iterator, outboundMessagePointer);

	message m = *iterator;
	retVal = m.getNextBit();
	setOutboundMessage(m);

	return retVal;
}

// Function to mock getNextBit(), to compare performance of this vs getNextBit()
uint8_t radio::getFakeOutboundBit()
{
	static bool one = false;
	static uint8_t counter = 0;
	static uint8_t max = 208;

	if(counter >= max) return NO_NEW_BITS;
	counter++;

	if(one)
	{
		one = false;
		return 1;
	}
	else
	{
		one = true;
		return 0;
	}
}

// Replace the currently active message.
// This is for keeping the bitPointer variable updated.
void radio::setOutboundMessage(message m)
{
	auto iterator = outboundMessages.begin();
	advance(iterator, outboundMessagePointer);
	outboundMessages.insert(iterator,m);
}

// Move on to the next message.
// This is called when the previous message is fully sent.
uint8_t radio::nextOutboundMessage()
{
	if(outboundMessages.size() > (outboundMessagePointer + 1))
	{
		outboundMessagePointer = 0;
		return NO_NEW_MESSAGE;
	}
	else
	{
		outboundMessagePointer++;
		return 0;
	}
}

// The send function adds a message to the queue.
// The interrupt handler will notice and start sending this message bit by bit.
void radio::send(string msg)
{
	message m(msg);
	outboundMessages.push_back(m);
}

// Function to be called by the interrupt handler.
// Standard version with start and stop bits, using a state machine.
void radio::sendTick()
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
				break;
			case MSG_HIGH:
				switch(getNextOutboundBit())
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
				if(nextOutboundMessage() != NO_NEW_MESSAGE)
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

// Function to be called by the interrupt handler.
// Simple implementation of manchester encoding.
void radio::sendTickM()
{
	static bool clock = true;
	static uint8_t bit;

	if(clock)
	{
		//bit = getFakeOutboundBit();
		bit = getNextOutboundBit();
		clock = false;
	}
	else
	{
		clock = true;
	}

	switch(bit)
	{
		case 0:
			if(clock)
			{
				SEND_HIGH;
				RED_HIGH;
			}
			else
			{
				SEND_LOW;
				RED_LOW;
			}
			break;
		case 1:
			if(clock)
			{
				SEND_LOW;
				RED_LOW;
			}
			else
			{
				SEND_HIGH;
				RED_HIGH;
			}
			break;
		case NO_NEW_BITS:
			SEND_LOW;
			break;
	}
}

// Receiver functions

message radio::getInboundMessage()
{
	return inboundMessage;
}

void radio::receiveTick()
{
	static bool started = false;
	static bool isCounting = false;
	static uint16_t counter = 0;
	bool on = RECEIVE;

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

				if(started)
				{
					inboundMessage.setIsComplete(true);
					stopCounter();
				}
				else
				{
					started = true;
				}
			}
			else if((ZERO_MIN < counter) && (counter < ZERO_MAX))
			{
				// Stop the timer
				isCounting = false;
				counter = 0;

				// Add zero to buffer
				inboundMessage.setNextBit(0);
			}
			else if((ONE_MIN < counter) && (counter < ONE_MAX))
			{
				// Stop the timer
				isCounting = false;
				counter = 0;

				// Add one to buffer
				inboundMessage.setNextBit(1);
			}
		}
	}
}
