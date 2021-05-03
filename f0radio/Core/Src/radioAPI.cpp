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

void radioInterrupt()
{
	return r.interrupt();
}
