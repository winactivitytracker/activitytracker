#ifndef TRANSMITTER_H
#define TRANSMITTER_H

using namespace std;

#include <bitset>
#include <deque>
#include <string>
#include "tim.h"
#include "main.h"

// Normally 0, set to 1 for unit tests
#define TEST_TRANSMITTER 1

#define NO_NEW_BITS		2

#define START_HIGH		0
#define START_LOW		1
#define MSG_HIGH		2
#define MSG_LOW			3
#define STOP_LOW		4
#define STOP_HIGH		5
#define IDLE			6

#define LENGTH_START	3
#define LENGTH_ZERO		1
#define LENGTH_ONE		2
#define LENGTH_STOP		4

class transmitter
{

private:

#ifndef TEST_TRANSMITTER
	bool messageSent = false;
	deque<string> messages;
	deque<bitset<8>> buffer;
	uint8_t bitPointer;
	void enable();
	void disable();
	void fillBuffer();
	uint8_t getNextBit();
	void pin(uint8_t high);
#endif

public:

	// These should normally be private and for internal use only.
	// We set them to public to unit test the specific components of the radio code
#ifdef TEST_TRANSMITTER
	bool messageSent = false;
	deque<string> messages;
	deque<bitset<8>> buffer;
	uint8_t bitPointer;
	void enable();
	void disable();
	void fillBuffer();
	uint8_t getNextBit();
	void pin(uint8_t high);
#endif

	void send(string message);
	void sendBlocking(string message);
	void sendAck();
	void tick();

};

#endif // TRANSMITTER_H
