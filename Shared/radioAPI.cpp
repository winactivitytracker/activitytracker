// C API for radio and message functions.
// This makes it possible for functions in C (like main and the interrupt handler) to call C++ code.

#include "radioAPI.h"

using namespace std;

#include <cstring>
#include "transmitter.h"
#include "receiver.h"
#if TEST_RADIO == 1
#include "radioTest.h"
radioTest rt;
#endif

transmitter t;
receiver r;
<<<<<<< HEAD
#if TEST_RADIO == 1
radioTest rt;
#endif
=======

>>>>>>> 36d49780ab4084e9eb71f82970dec73d73be7aab

// Transmitter class public functions

void transmitterSend(char * message)
{
	// Convert C char pointer to C++ string
	string messageString(message);

	t.send(messageString);
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
	t.tick();
}

// Receiver class public functions

void receiverEnable()
{
	r.enable();
}

void receiverDisable()
{
	r.disable();
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
	r.tick();
}

#if TEST_RADIO == 1

void radioTestAll()
{
	//rt.testAll(t,r);
}

#endif // TEST_RADIO == 1
