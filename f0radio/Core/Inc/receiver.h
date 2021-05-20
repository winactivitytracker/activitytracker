/*
 * receiver.h
 *
 *  Created on: 20 May 2021
 *      Author: arteeh
 */

#ifndef SRC_RECEIVER_H_
#define SRC_RECEIVER_H_

#include <bitset>
#include <deque>
#include "message.h"
#include "main.h"

#define START_MIN		400
#define START_MAX		600
#define ZERO_MIN		50
#define ZERO_MAX		100
#define ONE_MIN			250
#define ONE_MAX			300

#define RECEIVE			(GPIOB->IDR & GPIO_IDR_1)
#define GREEN_HIGH		(GPIOB->BSRR = GPIO_BSRR_BS_2)
#define GREEN_LOW		(GPIOB->BSRR = GPIO_BSRR_BR_2)

class receiver
{
private:

	deque<message> messages;
	bitset<1000> buffer;
	uint16_t bufferPointer;

public:

	void clearBuffer();
	bool checkMessage();
	message popMessage();
	void tick();

};

#endif /* SRC_RECEIVER_H_ */
