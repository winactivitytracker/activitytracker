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
message::message(string msg)
{
	messageString = msg;
	bitPointer = 0;

	/*
	// Turn the message into a char *
	char const * charArray = msg.c_str();

	// Manchester encode the message
	// i: the bit in the charArray
	// j: the bit in the bitArray
	uint64_t i = 0;
	uint64_t j = 0;
	while(i < (msg.length() * 8))
	{
		bool bit = (charArray[(i/8)] >> i) & 1;
		if(bit == 1)
		{
			bits.push_back(false);
			bits.push_back(true);
		}
		else
		{
			bits.push_back(true);
			bits.push_back(false);
		}

		i++;
		j += 2;
	}
	*/
}

/*
uint8_t message::setNextBit()
{
	if(bits.empty())
	{
		return NO_NEW_BITS;
	}

	auto front = bits.begin();
	std::advance(front, bitPointer);

	if(front == bits.end())
	{
		return NO_NEW_BITS;
	}
	else
	{
		bitPointer++;
		return *front;
	}
}
*/

// Return the currently selected bit in the message,
// then move the pointer to the next one
uint8_t message::setNextBit()
{
	// Turn the message into a char *
	char const * charList = messageString.c_str();

	// Get the correct character from the string using the bitpointer
	// For example: bit 19 from "hey" means the 3rd bit from 'y' is needed
	// so 19 / 8 is 2, and 'y' is char 2 of the string (starts at 0)
	uint8_t charPointer = bitPointer / 8;

	// Then we can get the char we need using our charPointer
	char ch = charList[charPointer];

	// When we need the 19th bit, we actually need the 3rd bit from ch
	// So 19 % 8 gives us 3
	uint8_t bitInChar = bitPointer % 8;

	// Mirror our byte, otherwise it will be sent from MSB to LSB
	uint8_t mirrored = mirrorByte(ch);

	// Get 0 or 1 with a bitwise operation,
	// using our mirrored byte and the bit we want (eg. 3)
	uint8_t bit = (mirrored >> bitInChar) & 1;

	// Check if the pointer exceeds the message length
	if((bitPointer / 8) > messageString.length())
	{
		return NO_NEW_BITS;
	}
	else
	{
		// Move to the next bit
		bitPointer++;

		return bit;
	}
}
