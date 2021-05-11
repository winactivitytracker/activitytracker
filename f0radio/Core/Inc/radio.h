// Header for the radio class

#ifndef RADIO_H_
#define RADIO_H_

#define NO_NEW_MESSAGE 3

using namespace std;

#include <string>
#include <vector>
#include "message.h"

class radio
{
private:
	uint8_t messagePointer;
	vector<message> messages;
public:
	void send(string msg);
	message getMessage();
	uint8_t getNextBit();
	void setMessage(message m);
	uint8_t nextMessage();
	void interrupt();
};

#endif /* RADIO_H_ */
