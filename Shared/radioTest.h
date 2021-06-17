#if TEST_RADIO == 1

#ifndef RADIOTEST_H
#define RADIOTEST_H

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

	void printResult(string test, string expected, string actual, string result);
	void printResult(string test, bool expected, bool actual, string result);

public:

	// Utility functions
	void testAll(transmitter t, receiver r);
	void testTransmitter(transmitter t);
	void testReceiver(receiver r);
	void testAPI();

	// Transmitter class tests
	void transmitterFillBufferEmpty(transmitter t);
	void transmitterFillBufferString(transmitter t);

	// Receiver class tests


	// API function tests

};


#endif // RADIOTEST_H

#endif // TEST_RADIO == 1
