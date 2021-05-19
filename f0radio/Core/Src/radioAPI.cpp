// C API for radio and message functions.
// This makes it possible for functions in C (like main and the interrupt handler) to call C++ code.

using namespace std;

#include "radio.h"
#include "radioAPI.h"

radio r;

void radioSend(char * message, uint8_t length)
{
	// Convert C char pointer to C++ string
	string messageString(message, message + length);

	return r.send(messageString);
}

void radioSendTick()
{
	// Use this version if you don't want Manchester encoding
	return r.sendTick();
	//return r.sendTickM();
}

void radioReceiveTick()
{
	return r.receiveTick();
}

void radioEcho()
{
	// FIXME: There is no string in this message
	message m = r.getInboundMessage();
	if(m.getIsComplete() == true)
	{
		string s = m.getMessageString();
		char c[s.size() + 1];
		s.copy(c,s.size()+1);
		radioSend(c,m.getMessageString().length());
	}
}
