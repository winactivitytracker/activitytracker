/*
 * transmitter.h
 *
 *  Created on: 20 May 2021
 *      Author: arteeh
 */

#ifndef SRC_TRANSMITTER_H_
#define SRC_TRANSMITTER_H_

#include <bitset>
#include <deque>
#include "message.h"
#include "main.h"

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
#define RED_HIGH		(GPIOA->BSRR = GPIO_BSRR_BS_2)
#define RED_LOW			(GPIOA->BSRR = GPIO_BSRR_BR_2)

class transmitter
{
private:

	deque<message> messages;
	deque<bitset<8>> buffer;
	uint8_t bufferPointer;

public:

	void fillBuffer();
	uint8_t getNextBit();
	void send(string msg);
	void tick();

};

#endif /* SRC_TRANSMITTER_H_ */
