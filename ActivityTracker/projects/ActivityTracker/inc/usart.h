#ifndef USART_MAIN_H
#define USART_MAIN_H

#include "stm32f0xx.h"
#include <stdio.h>
#include <stdlib.h>

void USARTSetup(void);
void USARTIntSetup(void);
void USARTPutChar(char c);
void USARTPutStr(char *str);
void USARTClearScreen(void);
char *USARTIntToAscii(int16_t i, char *p);
void USARTPrintNumber(int16_t number, char *str);
void USARTPrintNumberFloat(float number, char *str);

#endif //USART_MAIN_H
