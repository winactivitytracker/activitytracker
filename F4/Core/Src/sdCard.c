/*
 * sdCard.c
 *
 *  Created on: May 12, 2021
 *      Author: markv
 */

#include "sdCard.h"

FATFS FatFs; 	//Fatfs handle
FIL fil; 		//File handle
FRESULT fres; //Result after operations

bool initSdCard()
{
	fres = f_mount(&FatFs, "", 1); //1=mount now
	if (fres != FR_OK)
	{
		return false;
	} else
	{
		return true;
	}
}


bool writeFile(char* fileName, char* string)
{
	char* internalString = "";
	if(openFileRead(fileName))
	{
		f_close(&fil);
	} else
	{
		makeNewFile(fileName);
	}

	fres = f_open(&fil, fileName, FA_WRITE | FA_OPEN_EXISTING | FA_OPEN_APPEND);
	if(fres == FR_OK) {
		//Copy in a string
		uint16_t stringLength = strlen(string);
		strncpy((char*)internalString, string, stringLength);
		UINT bytesWrote;
		fres = f_write(&fil, internalString, stringLength, &bytesWrote);
		if(fres == FR_OK) {
			f_close(&fil);
			return true;
		} else {
			f_close(&fil);
			return false;
		}
	} else {
		return false;
	}
}

bool openFileRead(char* fileName)
{
	fres = f_open(&fil, fileName, FA_READ);
	if (fres != FR_OK) {
		return false;
	} else
	{
		return true;
	}
}


char* readFile(char* fileName, uint16_t amountOfBytesToRead)
{
	if(openFileRead(fileName))
	{
		char* readBuff = "";

		TCHAR* rres = f_gets((TCHAR*)readBuff, amountOfBytesToRead, &fil);
		if(rres != 0) {
			f_close(&fil);
			return readBuff;
		} else {
			f_close(&fil);
			return "reading error";
		}
	} else
	{
		f_close(&fil);
		return "file error";
	}
}

bool makeNewFile(char* fileName)
{
	//Make a new file with the "fileName", make sure ".txt" is included in the fileName
	fres = f_open(&fil, fileName, FA_WRITE | FA_OPEN_ALWAYS | FA_CREATE_NEW);
	if(fres == FR_OK) {
		f_close(&fil);
		return true;
	} else {
		return false;
	}
}
