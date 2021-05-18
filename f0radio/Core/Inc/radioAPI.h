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

void radioInterrupt();
void radioSend(char * message, uint8_t length);
void radioEdge();
void radioCount();

#ifdef __cplusplus
}
#endif

#endif /* INC_RADIOAPI_H_ */
