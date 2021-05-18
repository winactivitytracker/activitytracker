// Class to control the radio, and more importantly the buffer of messages to be sent

#include "radio.h"

// The send function adds a message to the queue.
// The interrupt handler will notice and start sending this message bit by bit.
void radio::send(string msg)
{
	message m(msg);
	messages.push_back(m);
}

// Return the currently active message
message radio::getMessage()
{
	auto iterator = messages.begin();
	advance(iterator, messagePointer);
	return *iterator;
}

uint8_t radio::getNextBit()
{
	uint8_t retVal;

	message m = getMessage();
	retVal = m.setNextBit();
	setMessage(m);

	return retVal;
}

// Function to mock getNextBit(), to compare performance of this vs getNextBit()
uint8_t radio::getFakeBit()
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
void radio::setMessage(message m)
{
	auto iterator = messages.begin();
	advance(iterator, messagePointer);
	messages.insert(iterator,m);
}

// Move on to the next message.
// This is called when the previous message is fully sent.
uint8_t radio::nextMessage()
{
	if(messages.size() > (messagePointer + 1))
	{
		messagePointer = 0;
		return NO_NEW_MESSAGE;
	}
	else
	{
		messagePointer++;
		return 0;
	}
}

// Function to be called by the interrupt handler.
// Standard version with start and stop bits, using a state machine.
void radio::interrupt()
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
				holdFor = LENGTH_START;
				state = START_LOW;
				break;
			case START_LOW:
				SEND_LOW;
				holdFor = LENGTH_START;
				state = MSG_HIGH;
				break;
			case MSG_HIGH:
				switch(getNextBit())
				{
					case 0:
						SEND_HIGH;
						holdFor = LENGTH_ZERO;
						state = MSG_LOW;
						break;
					case 1:
						SEND_HIGH;
						holdFor = LENGTH_ONE;
						state = MSG_LOW;
						break;
					// When getNextBit() returns this, the message is done and the next one can be selected
					case NO_NEW_BITS:
						SEND_LOW;
						state = STOP_HIGH;
						break;
				}
				break;
			case MSG_LOW:
				SEND_LOW;
				state = MSG_HIGH;
				break;
			case STOP_HIGH:
				SEND_HIGH;
				holdFor = LENGTH_START;
				state = STOP_LOW;
				break;
			case STOP_LOW:
				SEND_LOW;
				holdFor = LENGTH_START;
				state = IDLE;
				break;
			case IDLE:
				if(nextMessage() != NO_NEW_MESSAGE)
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
void radio::interruptManchester()
{
	static bool clock = true;
	static uint8_t bit;

	if(clock)
	{
		//bit = getFakeBit();
		bit = getNextBit();
		clock = false;
	}
	else
	{
		clock = true;
	}

	switch(bit)
	{
		case 0:
			if(clock)	SEND_HIGH;
			else		SEND_LOW;
			break;
		case 1:
			if(clock)	SEND_LOW;
			else		SEND_HIGH;
			break;
		case NO_NEW_BITS:
			SEND_LOW;
			break;
	}
}

void radio::edge()
{
	// check if already counting
	if(isCounting)
	{
		// Stop the timer and check the counter
		stopCounter();
		isCounting = false;

		if((ZERO_MIN > counter) && (counter > ZERO_MAX))
		{
			// Add zero to buffer
		}
		else if((ONE_MIN > counter) && (counter > ONE_MAX))
		{
			// Add one to buffer
		}
	}
	else
	{
		// Start the counter's timer
		counter = 0;
		startCounter();
		isCounting = true;
	}
}

void radio::count()
{
	counter++;
}
