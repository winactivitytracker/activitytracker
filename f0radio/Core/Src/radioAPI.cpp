// C API for radio and message functions.
// This makes it possible for functions in C (like main and the interrupt handler) to call C++ code.

using namespace std;

#include "transmitter.h"
#include "receiver.h"
#include "radioAPI.h"

transmitter t;
receiver r;

void radioSend(char * message)
{
	// Convert C char pointer to C++ string
	string messageString(message);

	return t.send(messageString);
}

void radioSendTick()
{
	return t.tick();
}

void radioReceiveTick()
{
	return r.tick();
}

void radioEcho()
{
	if(r.checkMessage())
	{
		// Get the filled up message object
		message m = r.popMessage();

		// Get the received string from the message
		string s = m.getMessageString();

		t.send(s);
	}
}
