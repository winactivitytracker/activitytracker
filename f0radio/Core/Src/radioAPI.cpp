// C API for radio and message functions.
// This makes it possible for functions in C (like main and the interrupt handler) to call C++ code.

using namespace std;

#include <cstring>

#include "transmitter.h"
#include "receiver.h"
#include "radioAPI.h"

transmitter t;
receiver r;

void radioSend(char * message)
{
	// Convert C char pointer to C++ string
	string messageString(message);

	t.send(messageString);
}

// Get a message from the message queue.
// str should be a char*, and it must be initialized
bool radioReceive(char* *str)
{
	if(r.checkMessage())
	{
		// Get the filled up message object
		string message = r.popMessage();

		// Here be dragons
		*str = const_cast<char*>(message.c_str());

		return true;
	}
	else
	{
		// There is no message in the queue
		return false;
	}
}

void radioEnableReceiver()
{
	r.enable();
}

void radioDisableReceiver()
{
	r.disable();
}

void radioSendTick()
{
	t.tick();
}

void radioReceiveTick()
{
	r.tick();
}
