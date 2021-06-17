#include "main.h"

#if TEST_RADIO

#include <radioTest.h>

// Print a result string to USART
void radioTest::printResult(string test, string expected, string actual, string result)
{
	HAL_UART_Transmit(&huart1, (uint8_t *)"Test: ",				sizeof("Test: "),		HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, (uint8_t *)test.c_str(),			sizeof(test),			HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, (uint8_t *)"\n",					sizeof("\n"),			HAL_MAX_DELAY);

	HAL_UART_Transmit(&huart1, (uint8_t *)"Expected: ",			sizeof("Expected: "),	HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, (uint8_t *)expected.c_str(),		sizeof(expected),		HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, (uint8_t *)"\n",					sizeof("\n"),			HAL_MAX_DELAY);

	HAL_UART_Transmit(&huart1, (uint8_t *)"Actual: ",			sizeof("Actual: "),		HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, (uint8_t *)actual.c_str(),		sizeof(actual),			HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, (uint8_t *)"\n",					sizeof("\n"),			HAL_MAX_DELAY);

	HAL_UART_Transmit(&huart1, (uint8_t *)"Result: ",			sizeof("Result: "),		HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, (uint8_t *)result.c_str(),		sizeof(result),			HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, (uint8_t *)"\n\n",				sizeof("\n\n"),			HAL_MAX_DELAY);
}

// Same function, but for bools
void radioTest::printResult(string test, bool expected, bool actual, string result)
{
	string e = "false";
	string a = "false";

	if(expected)
	{
		e = "true";
	}

	if(actual)
	{
		a = "true";
	}

	printResult(test, e, a, result);
}

// Utility functions

// Test transmitter, receiver and API functions
void radioTest::testAll()
{
	testTransmitter();
	testReceiver();
	testAPI();
}

// Test all transmitter functions
void radioTest::testTransmitter()
{
	//transmitterFillBufferEmpty();
	//transmitterFillBufferString();
}

// Test all receiver functions
void radioTest::testReceiver()
{

}

// Test API functions
void radioTest::testAPI()
{

}



// Transmitter class tests

// call fillBuffer when the message queue is empty
void radioTest::transmitterFillBufferEmpty()
{
	// Arrange
	bool expected = true;
	bool actual;
	string result;

	transmitter t;

	// Act
	t.fillBuffer();
	actual = t.buffer.empty();

	// Assert
	if(actual == expected)
	{
		result == "PASS: Buffer is empty";
	}
	else
	{
		result == "FAIL: Buffer is not empty";
	}

	printResult("fillBuffer with an empty queue", expected, actual, result);
}

void radioTest::transmitterFillBufferString()
{
	// Arrange
	bitset<8> a("a");			// "a" (01100001) bitset object
	deque<bitset<8>> expected;
	expected.push_front(a);		// Fill the expected buffer with "a"
	deque<bitset<8>> actual;
	string result;
	transmitter t;

	// Act
	t.messages.push_front("a");	// Add "a" to message queue, like send()
	t.fillBuffer();
	actual = t.buffer;

	// Assert
	if(actual.front() == expected.front())
	{
		result = "PASS: a is in the buffer";
	}
	else
	{
		result = "FAIL: a is not in the buffer";
	}

	printResult("fillBuffer with one string in the queue", expected.front().to_string(), actual.front().to_string(), result);
}

// Receiver class tests



// API function tests



#endif
