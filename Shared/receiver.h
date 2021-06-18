#ifndef RECEIVER_H
#define RECEIVER_H

using namespace std;

#include <bitset>
#include <deque>
#include <string>
#include "tim.h"
#include "main.h"

#define START_HIGH		0
#define START_LOW		1
#define MSG_HIGH		2
#define MSG_LOW			3
#define STOP_LOW		4
#define STOP_HIGH		5
#define IDLE			6

// See "Radio bit lengtes.xlsx"
#define ZERO_MIN		80
#define ZERO_MAX		110
#define ONE_MIN			180
#define ONE_MAX			210
#define START_MIN		280
#define START_MAX		310
#define STOP_MIN		380
#define STOP_MAX		410

class receiver
{

private:

#ifndef TEST_RADIO
	deque<string> messages;
	deque<bitset<8>> buffer;
	uint16_t bitPointer;
	void clearBuffer();
	void addToBuffer(bool bit);
#endif

public:

	// These should normally be private and for internal use only.
	// We set them to public to unit test the specific components of the radio code
#ifdef TEST_RADIO
	deque<string> messages;
	deque<bitset<8>> buffer;
	int16_t bitPointer;
	void clearBuffer();
	void addToBuffer(bool bit);
#endif

	void enable();
	void disable();
	bool checkMessage();
	string popMessage();
	bool waitForAck(uint16_t milliseconds);
	void tick();

};

#endif // RECEIVER_H
