/*
 * receiver.h
 *
 *  Created on: 20 May 2021
 *      Author: arteeh
 */

#ifndef SRC_RECEIVER_H_
#define SRC_RECEIVER_H_

using namespace std;

#include <bitset>
#include <deque>
#include <string>
#include "tim.h"
#include "main.h"

#define START_HIGH		0
#define START_LOW		1
#define MSG_HIGH		2
#define MSG_LOW			3
#define STOP_LOW		4
#define STOP_HIGH		5
#define IDLE			6

// See "Radio bit lengtes.xlsx"
#define ZERO_MIN		14
#define ZERO_MAX		24
#define ONE_MIN			32
#define ONE_MAX			42
#define START_MIN		50
#define START_MAX		60
#define STOP_MIN		69
#define STOP_MAX		79

class receiver
{
private:

	deque<string> messages;
	deque<bitset<8>> buffer;
	uint16_t bitPointer;
	void clearBuffer();
	void addToBuffer(bool bit);

public:

	void enable();
	void disable();
	bool checkMessage();
	string popMessage();
	void tick();

};

#endif /* SRC_RECEIVER_H_ */
