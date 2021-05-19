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

void radioSend(char * message, uint8_t length);
void radioSendTick();
void radioReceiveTick();
void radioEcho();

#ifdef __cplusplus
}
#endif

#endif /* INC_RADIOAPI_H_ */
