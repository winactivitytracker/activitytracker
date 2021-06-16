/*
 * test.h
 *
 *  Created on: 16 Jun 2021
 *      Author: markv
 */

#ifndef INC_TEST_H_
#define INC_TEST_H_

void testAll();
void testMPU();
void testRadio();
void testRTC();
void setCorrectTime();
void setIncorrectTime();
void printTestResult(char* testObject, char* currentTest, char* result);


#endif /* INC_TEST_H_ */
