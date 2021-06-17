/*
 * test.h
 *
 *  Created on: 16 Jun 2021
 *      Author: markv
 */

#ifndef INC_TEST_H_
#define INC_TEST_H_

#define MAXACCELORGYRO	(2000)
#define MINACCELORGYRO	(-2000)

void testAll();
void testMPU();
void testRadio();
void testRTC();
void setCorrectMPU();
void setIncorrectMPU();
void testGyro();
void testAccel();
void setCorrectTime();
void setIncorrectTime();
void printTestResult(char* testObject, char* currentTest, char* result);
void printWrongData(char shouldBe[90], char isCurrent[60]);


#endif /* INC_TEST_H_ */
