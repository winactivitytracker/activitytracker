/*
 * RATmain.h
 *
 *  Created on: 18 Jun 2021
 *      Author: markv
 */

#ifndef INC_RATMAIN_H_
#define INC_RATMAIN_H_

#include "mpu.h"
#include "radioAPI.h"
#include "radioHandler.h"
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

// F0 Radio timers
#define SEND_TIMER		&htim15
#define RECEIVE_TIMER	&htim16

// 1 for wrist, 0 for ankle
#define IDENTIFIER		0
#define TEST			1
#define TEST_MPU		0
#define TEST_RTC 		1
#define TEST_RADIO 		0
#define DO_NOT_ENCRYPT	1

void RATmain();

#endif /* INC_RATMAIN_H_ */
