// Header for the message class

#ifndef SRC_MESSAGE_H_
#define SRC_MESSAGE_H_

#define NO_NEW_BITS 2

using namespace std;

#include <list>
#include <string>

extern "C"
{
#include "main.h"
}

class message
{
private:
	string messageString;
	uint16_t bitPointer;
	bool isComplete;
	uint8_t mirrorByte(uint8_t b);
public:
	message();
	message(string msg);
	string getMessageString();
	bool getIsComplete();
	void setIsComplete(bool i);
	uint8_t getNextBit();
	void setNextBit(uint8_t bit);
};

#endif /* SRC_MESSAGE_H_ */
