/*
 * sdCard.c
 *
 *  Created on: May 12, 2021
 *      Author: markv
 */

#include "sdCard.h"
#include "gps.h"

FATFS FatFs; 	//Fatfs handle
FIL fil; 		//File handle
FRESULT fres; //Result after operations

//check if the SD card is insterted, if it is mount the card right now and return true.
//If it isn't, return false.
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

//Write something to the SD card. If the write was successfull, return true.
//If the write was successfull, return true.
bool writeFile(char* fileName, char* string)
{
	if(openFileRead(fileName))
	{
		f_close(&fil);
	} else
	{
		makeNewFile(fileName);
	}

	fres = f_open(&fil, fileName, FA_WRITE | FA_OPEN_EXISTING | FA_OPEN_APPEND);
	if(fres == FR_OK) {
		uint16_t stringLength = strlen(string);
	    UINT bytesWrote;
	    fres = f_write(&fil, string, stringLength, &bytesWrote);
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

//open a file for reading.
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

//read a set amount of bytes from a file
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

//Make a new file with the "fileName", make sure ".txt" is included in the fileName
bool makeNewFile(char* fileName)
{
	fres = f_open(&fil, fileName, FA_WRITE | FA_OPEN_ALWAYS | FA_CREATE_NEW);
	if(fres == FR_OK) {
		f_close(&fil);
		return true;
	} else {
	   	return false;
	}
}

//write an activity to the SD card. this function will also grab time data from the GPS module
bool activityToSD(char* fileName, char* string)
{
	char *sTime;
	sTime = getTime();
	writeFile(fileName, sTime);
	writeFile(fileName, " : ");
	if(writeFile(fileName, string))
	{
		free(sTime);
		writeFile(fileName, "\n");
		return true;
	} else
	{
		free(sTime);
		return false;
	}

}

//write the total activity to the SD card
void totalActivityToSD(char* fileName, char* firstString, char* secondString)
{
	char *sTime;
	sTime = getTime();
	writeFile(fileName, "Einde activiteit, activteit gedaan voor ");
	writeFile(fileName, firstString);
	writeFile(fileName, " minuten.\nActiviteit beëindigt op: ");
	writeFile(fileName, sTime);
	free(sTime);
	writeFile(fileName, ". Activiteit was: ");
	writeFile(fileName, secondString);
	writeFile(fileName, "\n");

}

//write the start of the activty to the SD card
void writeStartToSD(char* fileName)
{
	char *sTime;
	sTime = getTime();
	writeFile(fileName, "Nieuwe activiteit gestart op: ");
	writeFile(fileName, sTime);
	free(sTime);
	writeFile(fileName, "\n");
}
