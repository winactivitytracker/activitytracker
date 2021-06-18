/*
 * sdCard.h
 *
 *  Created on: May 12, 2021
 *      Author: markv
 */

#ifndef INC_SDCARD_H_
#define INC_SDCARD_H_

#include "user_diskio_spi.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

extern FATFS FatFs; 	//Fatfs handle
extern FIL fil; 		//File handle
extern FRESULT fres; //Result after operations

bool initSdCard();
bool writeFile(char* fileName, char* string);
bool openFileRead(char* fileName);
char* readFile(char* fileName, uint16_t amountOfBytesToRead);
bool makeNewFile(char* fileName);
bool activityToSD(char* fileName, char* string);
void totalActivityToSD(char* fileName, char* firstString, char* secondString);
void writeStartToSD(char* fileName);



#endif /* INC_SDCARD_H_ */
