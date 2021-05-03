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
	return messages.at(messagePointer);
}

uint8_t radio::getNextBit()
{
	uint8_t retVal;

	message m = getMessage();
	retVal = m.setNextBit();
	setMessage(m);

	return retVal;
}

// Replace the currently active message.
// This is for keeping the bitPointer variable updated.
void radio::setMessage(message m)
{
	messages.at(messagePointer) = m;
}

// Move on to the next message.
// This is called when the previous message is fully sent.
uint8_t radio::nextMessage()
{
	messagePointer++;

	if(messages.size() > messagePointer)
	{
		messagePointer = 0;
		return NO_NEW_MESSAGE;
	}
	else
	{
		return 0;
	}
}

// Function called by the interrupt
void radio::interrupt()
{
	// Toggle red LED to keep track of ticks
	HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);

	// Counter to keep track of how long a pin should be set (4 when bit is 1, 2 when it's 0)
	static uint8_t ticksToGo = 0;

	// When done waiting, move on to the next bit
	if(ticksToGo == 0)
	{
		// Get the next bit in the current message from the message list
		switch(getNextBit())
		{
			case 0:
				HAL_GPIO_WritePin(SEND_GPIO_Port, SEND_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
				ticksToGo = 2;
			case 1:
				HAL_GPIO_WritePin(SEND_GPIO_Port, SEND_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
				ticksToGo = 4;
			// When getNextBit() returns this, the message is done and the next one can be selected.
			// This is done in nextMessage()
			case NO_NEW_BITS:
				HAL_GPIO_WritePin(SEND_GPIO_Port, SEND_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
				if(nextMessage() == NO_NEW_MESSAGE)
				{
					// What to do when the buffer is empty?
				}
		}
	}
	else
	{
		ticksToGo--;
	}
}
