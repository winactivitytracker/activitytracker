/*
 * test.h
 *
 *  Created on: 17 Jun 2021
 *      Author: THE SAPHIRE DRAGON
 */

#ifndef INC_TEST_H_
#define INC_TEST_H_

void testAll();
void testActivity();
void testTimeCheckFifo();
void setEmptyBuffers();
void setBuffer0();
void setBuffer1();
void setBufferBoth();
void setBuffer1Late();
void setBuffer0Late();
void setBuffer1LateInPreviousMinute();
void setBuffer0LateInPreviousMinute();
void printTestResult(char* testObject, char* currentTest, char* result);

#endif /* INC_TEST_H_ */
