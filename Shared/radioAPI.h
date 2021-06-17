// C API for radio and message functions.
// This makes it possible for functions in C (like main and the interrupt handler) to call C++ code.

#ifndef INC_RADIOAPI_H_
#define INC_RADIOAPI_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>

// Transmitter class public functions
void transmitterSend(char * message);
void transmitterSendBlocking(char * message);
void transmitterSendAck();
void transmitterTick();

// Receiver class public functions
void receiverEnable();
void receiverDisable();
bool receiverCheckMessage();
void receiverPopMessage(char *str);
bool receiverWaitForAck(uint16_t milliseconds);
void receiverTick();

// radioTest class functions
void radioTestAll();
void radioTestTransmitter();
void radioTestReceiver();

#ifdef __cplusplus
}
#endif

#endif /* INC_RADIOAPI_H_ */
