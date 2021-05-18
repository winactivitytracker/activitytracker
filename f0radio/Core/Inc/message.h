// Header for the message class

#ifndef SRC_MESSAGE_H_
#define SRC_MESSAGE_H_

#define NO_NEW_BITS 2

using namespace std;

#include <string>
#include <list>

extern "C"
{
#include "main.h"
}

class message
{
private:
	string messageString;
	uint16_t bitPointer;
	uint8_t mirrorByte(uint8_t b);
public:
	message(string msg);
	uint8_t setNextBit();
};

#endif /* SRC_MESSAGE_H_ */
