#ifndef __USART_main_h
#define __USART_main_h

#include "stm32f0xx.h"
#include <stdio.h>
#include <stdlib.h>

void USART_Setup(void);
void USART_int_Setup(void);
void USART_putc(char c);
void USART_Putstr(char *str);
void USART_Clearscreen(void);
char *USART_itoa(int16_t i, char *p);
void USART_print_number(int16_t number, char *str);

#endif //__USART_main_h
