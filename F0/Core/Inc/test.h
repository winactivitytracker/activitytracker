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
void setCorrectMPU();
void setIncorrectMPU();
void setCorrectTime();
void setIncorrectTime();
void printTestResult(char* testObject, char* currentTest, char* result);
void printWrongData(char shouldBe[30], char isCurrent[30]);


#endif /* INC_TEST_H_ */
