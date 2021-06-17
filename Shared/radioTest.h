#ifndef RADIOTEST_H
#define RADIOTEST_H

#include "usart.h"
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
	void testAll();
	void testTransmitter();
	void testReceiver();
	void testAPI();

	// Transmitter class tests
	void transmitterFillBufferEmpty();
	void transmitterFillBufferString();

	// Receiver class tests


	// API function tests

};


#endif // RADIOTEST_H
