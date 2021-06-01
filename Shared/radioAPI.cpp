// C API for radio and message functions.
// This makes it possible for functions in C (like main and the interrupt handler) to call C++ code.

using namespace std;

#include <cstring>

#include "transmitter.h"
#include "receiver.h"
#include "radioAPI.h"

transmitter t;
receiver r;

// Transmitter class public functions

void transmitterSend(char * message)
{
	// Convert C char pointer to C++ string
	string messageString(message);

	return t.send(messageString);
}

void transmitterSendBlocking(char * message)
{
	// Convert C char pointer to C++ string
	string messageString(message);

	return t.sendBlocking(messageString);
}

void transmitterSendAck()
{
	return t.sendAck();
}

void transmitterTick()
{
	return t.tick();
}

// Receiver class public functions

void receiverEnable()
{
	return r.enable();
}

void receiverDisable()
{
	return r.disable();
}

bool receiverCheckMessage()
{
	return r.checkMessage();
}

// Get a message from the message queue.
// str should be a char*, and it must be initialized
void receiverPopMessage(char* *str)
{
	// Get the filled up message object
	string message = r.popMessage();

	// Here be dragons
	*str = const_cast<char*>(message.c_str());
}

bool receiverWaitForAck(uint16_t milliseconds)
{
	return r.waitForAck(milliseconds);
}

void receiverTick()
{
	return r.tick();
}