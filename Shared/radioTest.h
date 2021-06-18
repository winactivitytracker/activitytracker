#ifndef RADIOTEST_H
#define RADIOTEST_H

#include "main.h"
#include "usart.h"
#include "transmitter.h"
#include "receiver.h"
#include "radioAPI.h"

#include <string>
#include <bitset>
#include <deque>

using namespace std;

class radioTest
{

private:

	// Utility functions
	void printResult(string test, string expected, string actual, string result);

public:

	// Utility functions
	void testAll();
	void testTransmitter();
	void testReceiver();
	void testAPI();

	// Transmitter class tests
	void transmitterFillBufferEmpty();
	void transmitterFillBufferString();
	void transmitterGetNextBitOnce();
	void transmitterGetNextBitTwice();
	void transmitterGetNextBitEmpty();

	// Receiver class tests
	void receiverAddToBuffer1Byte();
	void receiverAddToBuffer2Byte();

	// API function tests

};

#endif // RADIOTEST_H
