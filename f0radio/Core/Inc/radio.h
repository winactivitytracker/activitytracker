// Header for the radio class

#ifndef RADIO_H_
#define RADIO_H_

#define NO_NEW_MESSAGE	3

#define START_HIGH		0
#define START_LOW		1
#define MSG_HIGH		2
#define MSG_LOW			3
#define STOP_LOW		4
#define STOP_HIGH		5
#define IDLE			6

#define LENGTH_START	5
#define LENGTH_ONE		3
#define LENGTH_ZERO		1

#define SEND_HIGH		(GPIOB->BSRR = GPIO_BSRR_BS_3)
#define SEND_LOW		(GPIOB->BSRR = GPIO_BSRR_BR_3)
#define RECEIVE			(GPIOB->IDR & GPIO_IDR_1)
#define RED_HIGH		(GPIOA->BSRR = GPIO_BSRR_BS_2)
#define RED_LOW			(GPIOA->BSRR = GPIO_BSRR_BR_2)
#define GREEN_HIGH		(GPIOB->BSRR = GPIO_BSRR_BS_2)
#define GREEN_LOW		(GPIOB->BSRR = GPIO_BSRR_BR_2)

#define START_MIN		400
#define START_MAX		600
#define ZERO_MIN		50
#define ZERO_MAX		100
#define ONE_MIN			250
#define ONE_MAX			300

using namespace std;

#include <string>
#include <list>
#include "message.h"
#include "main.h"

class radio
{
private:

	list<message> outboundMessages;
	uint8_t outboundMessagePointer;
	message inboundMessage;

public:
	// Transmitter functions
	uint8_t getNextOutboundBit();
	uint8_t getFakeOutboundBit();
	void setOutboundMessage(message m);
	uint8_t nextOutboundMessage();
	void send(string msg);
	void sendTick();
	void sendTickM();

	// Receiver functions
	message getInboundMessage();
	void receiveTick();
};

#endif /* RADIO_H_ */
