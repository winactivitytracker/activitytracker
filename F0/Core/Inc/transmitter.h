/*
 * transmitter.h
 *
 *  Created on: 20 May 2021
 *      Author: arteeh
 */

#ifndef SRC_TRANSMITTER_H_
#define SRC_TRANSMITTER_H_

using namespace std;

#include <bitset>
#include <deque>
#include <string>
#include "tim.h"
#include "main.h"

// F4
//#define TRANSMITTER_TIMER	&htim10
// F0
#define TRANSMITTER_TIMER	&htim15

#define NO_NEW_BITS			2

#define START_HIGH			0
#define START_LOW			1
#define MSG_HIGH			2
#define MSG_LOW				3
#define STOP_LOW			4
#define STOP_HIGH			5
#define IDLE				6

#define LENGTH_START		3
#define LENGTH_ZERO			1
#define LENGTH_ONE			2
#define LENGTH_STOP			4

class transmitter
{
private:

	deque<string> messages;
	deque<bitset<8>> buffer;
	uint8_t bitPointer;
	void enable();
	void disable();
	void fillBuffer();
	uint8_t getNextBit();
	void pin(uint8_t high);

public:

	void send(string message);
	void tick();

};

#endif /* SRC_TRANSMITTER_H_ */
