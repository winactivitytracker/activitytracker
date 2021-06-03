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
	// Stop listening
	r.disable();

	// Convert C char pointer to C++ string
	string messageString(message);

	t.send(messageString);

	// Start listening again
	r.enable();
}

void transmitterSendBlocking(char * message)
{
	// Stop listening
	r.disable();

	// Convert C char pointer to C++ string
	string messageString(message);

	t.sendBlocking(messageString);

	// Start listening again
	r.enable();
}

void transmitterSendAck()
{
	// Stop listening
	r.disable();

	t.sendAck();

	// Start listening again
	r.enable();
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
void receiverPopMessage(char *str)
{
	// Get the filled up message object
	string message = r.popMessage();

	char * tmp = const_cast<char*>(message.c_str());

	// Here be dragons
	//str = const_cast<char*>(message.c_str());

	strncpy(str,tmp,60);
}

bool receiverWaitForAck(uint16_t milliseconds)
{
	return r.waitForAck(milliseconds);
}

void receiverTick()
{
	return r.tick();
}
