#include "radioTest.h"

#if TEST_RADIO == 1

// Utility functions

// Print a result to USART
void radioTest::printResult(string test, string expected, string actual, string result)
{
	HAL_UART_Transmit(&huart1, (uint8_t *)"Test: ",sizeof("Test: "),HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, (uint8_t *)test.c_str(),sizeof(test),HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, (uint8_t *)"\n",sizeof("\n"),HAL_MAX_DELAY);

	HAL_UART_Transmit(&huart1, (uint8_t *)"Expected: ",sizeof("Expected: "),HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, (uint8_t *)expected.c_str(),sizeof(expected),HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, (uint8_t *)"\n",sizeof("\n"),HAL_MAX_DELAY);

	HAL_UART_Transmit(&huart1, (uint8_t *)"Actual: ",sizeof("Actual: "),HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, (uint8_t *)actual.c_str(),sizeof(actual),HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, (uint8_t *)"\n",sizeof("\n"),HAL_MAX_DELAY);

	HAL_UART_Transmit(&huart1, (uint8_t *)"Result: ",sizeof("Result: "),HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, (uint8_t *)result.c_str(),sizeof(result),HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, (uint8_t *)"\n\n",sizeof("\n\n"),HAL_MAX_DELAY);
}

// Test transmitter, receiver and API functions
void radioTest::testAll()
{
	testTransmitter();
	testReceiver();
}

// Test all transmitter functions
void radioTest::testTransmitter()
{
	transmitterFillBufferEmpty();
	transmitterFillBufferString();
	transmitterGetNextBitOnce();
	transmitterGetNextBitTwice();
	transmitterGetNextBitEmpty();
}

// Test all receiver functions
void radioTest::testReceiver()
{
	receiverAddToBuffer1Byte();
	receiverAddToBuffer2Byte();
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

	// Assert
	actual = t.buffer.empty();

	if(actual == expected)
	{
		result == "PASS: Buffer is empty";
	}
	else
	{
		result == "FAIL: Buffer is not empty";
	}

	printResult("fillBuffer with an empty queue", to_string(expected), to_string(actual), result);
}

void radioTest::transmitterFillBufferString()
{
	// Arrange
	string expected = "01100001"; // to_string of "a" (01100001) bitset object
	string actual;
	string result;
	transmitter t;
	t.messages.push_front("a"); // Add "a" to message queue, like send()

	// Act
	t.fillBuffer();

	// Assert
	actual = t.buffer.front().to_string();

	if(actual == expected)
	{
		result = "PASS: a is in the buffer";
	}
	else
	{
		result = "FAIL: a is not in the buffer";
	}

	printResult("fillBuffer with one string in the queue", expected, actual, result);
}

void radioTest::transmitterGetNextBitOnce()
{
	// Arrange
	uint8_t expected = 1;
	uint8_t actual;
	string result;
	transmitter t;
	t.messages.push_front("a"); // "a" (01100001) starts with 1 (right to left)
	t.fillBuffer();

	// Act
	actual = t.getNextBit();

	// Assert
	if(actual == expected)
	{
		result = "PASS: returned bit is 1";
	}
	else
	{
		result = "FAIL: returned bit is not 1";
	}

	printResult("getNextBit with 01100001 in the queue", to_string(expected), to_string(actual), result);
}

void radioTest::transmitterGetNextBitTwice()
{
	// Arrange
	uint8_t expected = 0;
	uint8_t actual;
	string result;
	transmitter t;
	t.messages.push_front("a"); // "a" (01100001), get the 2nd bit
	t.fillBuffer();

	// Act
	actual = t.getNextBit(); // First bit (1)
	actual = t.getNextBit(); // Second bit (0), overwrite the first one

	// Assert
	if(actual == expected)
	{
		result = "PASS: returned bit is 0";
	}
	else
	{
		result = "FAIL: returned bit is not 0";
	}

	printResult("getNextBit twice with 01100001 in the queue", to_string(expected), to_string(actual), result);
}

void radioTest::transmitterGetNextBitEmpty()
{
	// Arrange
	uint8_t expected = NO_NEW_BITS; // Macro for when we're out of bits
	uint8_t actual;
	string result;
	transmitter t;

	// Act
	actual = t.getNextBit(); // Don't fill the buffer, just call the function

	// Assert
	if(actual == expected)
	{
		result = "PASS: getNextBit returns NO_NEW_BITS";
	}
	else
	{
		result = "FAIL: getNextBit doesn't return NO_NEW_BITS";
	}



	printResult("getNextBit with an empty queue", to_string(expected), to_string(actual), result);
}

// Receiver class tests

void radioTest::receiverAddToBuffer1Byte()
{
	// Arrange
	string expected = "01100001"; // to_string of "a" bitset object
	string actual;
	string result;
	receiver r;

	// Act
	r.addToBuffer(1);
	r.addToBuffer(0);
	r.addToBuffer(0); // Bits are received
	r.addToBuffer(0); // from LSB to MSB
	r.addToBuffer(0);
	r.addToBuffer(1);
	r.addToBuffer(1);
	r.addToBuffer(0);

	// Assert
	actual = r.buffer.front().to_string();
	r.buffer.pop_front();

	if(actual == expected)
	{
		result = "PASS: 01100001 is in the buffer";
	}
	else
	{
		result = "FAIL: 01100001 is not in the buffer";
	}

	printResult("addToBuffer 8 times: 01100001", expected, actual, result);
}

void radioTest::receiverAddToBuffer2Byte()
{
	// Arrange
	string expected = "0110000101111010"; // "a" plus "z" bitset to_string
	string actual;
	string result;
	receiver r;

	// Act
	r.addToBuffer(1);
	r.addToBuffer(0);
	r.addToBuffer(0);
	r.addToBuffer(0);
	r.addToBuffer(0);
	r.addToBuffer(1);
	r.addToBuffer(1);
	r.addToBuffer(0);

	r.addToBuffer(0);
	r.addToBuffer(1);
	r.addToBuffer(0);
	r.addToBuffer(1);
	r.addToBuffer(1);
	r.addToBuffer(1);
	r.addToBuffer(1);
	r.addToBuffer(0);

	// Assert
	actual = r.buffer.front().to_string();
	r.buffer.pop_front();
	actual += r.buffer.front().to_string();
	r.buffer.pop_front();

	if(actual == expected)
	{
		result = "PASS: 01100001 01111010 is in the buffer";
	}
	else
	{
		result = "FAIL: 01100001 01111010 is not in the buffer";
	}

	printResult("addToBuffer 16 times: 01100001 01111010", expected, actual, result);
}

#endif // TEST_RADIO == 1
