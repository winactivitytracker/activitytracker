/*
 * radioAPI.h
 *
 *  Created on: Apr 30, 2021
 *      Author: arteeh
 */

#ifndef INC_RADIOAPI_H_
#define INC_RADIOAPI_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>

void radioSend(char * message);
bool radioReceive(char* *str);
void radioEnableReceiver();
void radioDisableReceiver();
void radioSendTick();
void radioReceiveTick();


#ifdef __cplusplus
}
#endif

#endif /* INC_RADIOAPI_H_ */
