#ifndef RADIOTEST_H
#define RADIOTEST_H

#include "usart.h"
#include "transmitter.h"
#include "receiver.h"

class radioTest
{

private:

	void printResult(string test, string expected, string actual, string result);
	void printResult(string test, bool expected, bool actual, string result);

public:

	// Utility functions
	void testAll();
	void testTransmitter();
	void testReceiver();
	void testAPI();

	// Transmitter class tests
	void transmitterFillBufferEmpty();
	void transmitterFillBuffer1String();
	void transmitterFillBuffer2String();

	// Receiver class tests


	// API function tests


};

#endif // RADIOTEST_H
