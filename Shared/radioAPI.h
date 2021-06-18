// C API for radio and message functions.
// This makes it possible for functions in C (like main and the interrupt handler) to call C++ code.

#ifndef RADIOAPI_H
#define RADIOAPI_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include "main.h"

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

// Test class functions
void radioTestAll();

#ifdef __cplusplus
}
#endif

#endif // RADIOAPI_H
