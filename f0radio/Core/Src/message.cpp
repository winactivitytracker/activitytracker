/*
 * message.cpp
 *
 *  Created on: Apr 30, 2021
 *      Author: arteeh
 */

#include "message.h"

// Function to mirror the bits in a byte.
uint8_t message::mirrorByte(uint8_t b)
{
	b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
	b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
	b = (b & 0xAA) >> 1 | (b & 0x55) << 1;

	return b;
}

// Class constructor
message::message()
{
	messageString = "";
	bitPointer = 0;
}

// Class constructor
message::message(string msg)
{
	messageString = msg;
	bitPointer = 0;
}

string message::getMessageString()
{
	return messageString;
}

// Return the currently selected bit in the message,
// then move the pointer to the next one
uint8_t message::getNextBit()
{
	if((bitPointer / 8) > messageString.length())
	{
		return NO_NEW_BITS;
	}

	char ch = messageString[bitPointer/8];

	// When we need the 19th bit, we actually need the 3rd bit from ch
	// So 19 % 8 gives us 3
	uint8_t bitInChar = bitPointer % 8;

	// Mirror our byte, otherwise it will be sent from MSB to LSB
	//uint8_t mirrored = mirrorByte(ch);

	// Get 0 or 1 with a bitwise operation,
	// using our mirrored byte and the bit we want (eg. 3)
	uint8_t bit = (ch >> bitInChar) & 1;

	// Move to the next bit
	bitPointer++;

	return bit;
}

// FIXME: Something goes wrong here
// Write a bit to a messageString
void message::setNextBit(uint8_t bit)
{
	uint8_t charPointer = bitPointer / 8;
	uint8_t c = messageString[charPointer];
	if(bit) c |= (1UL << (7 - (bitPointer % 8)));
	if(charPointer > messageString.length())
	{
		messageString += c;
	}
	else
	{
		messageString[charPointer] = c;
	}

	bitPointer++;
}
