// Header for the radio class

#ifndef RADIO_H_
#define RADIO_H_

#define NO_NEW_MESSAGE 3

#define START_HIGH 0
#define START_LOW 1
#define MSG_HIGH 2
#define MSG_LOW 3
#define STOP_LOW 4
#define STOP_HIGH 5
#define IDLE 6

#define LENGTH_START 5
#define LENGTH_ONE 3
#define LENGTH_ZERO 1

#define SEND_HIGH (GPIOB->BSRR = GPIO_BSRR_BS_3)
#define SEND_LOW (GPIOB->BSRR = GPIO_BSRR_BR_3)

#define ZERO_MIN 1
#define ZERO_MAX 5
#define ONE_MIN 5
#define ONE_MAX 10

using namespace std;

#include <string>
#include <list>
#include "message.h"
#include "main.h"

class radio
{
private:
	uint8_t messagePointer;
	list<message> messages;
	uint16_t counter;
	bool isCounting;
public:
	void send(string msg);
	message getMessage();
	uint8_t getNextBit();
	uint8_t getFakeBit();
	void setMessage(message m);
	uint8_t nextMessage();
	void interrupt();
	void interruptManchester();
	void edge();
	void count();
};

#endif /* RADIO_H_ */
