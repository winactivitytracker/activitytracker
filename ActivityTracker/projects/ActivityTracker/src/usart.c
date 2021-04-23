#include "usart.h"

void USARTSetup(void)
{
	GPIO_InitTypeDef gpioA;
	USART_InitTypeDef usart;

	// Initialize USART1
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	// Setup Tx- and Rx pin
	gpioA.GPIO_Mode			= GPIO_Mode_AF;
	gpioA.GPIO_Pin			= GPIO_Pin_9 | GPIO_Pin_10;
	gpioA.GPIO_Speed		= GPIO_Speed_50MHz;

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);

	GPIO_Init(GPIOA, &gpioA);

	USART_StructInit(&usart);
	
	usart.USART_BaudRate	= 115200;
	
	USART_Init(USART1, &usart);

	USART_Cmd(USART1, ENABLE);
}

void USARTIntSetup(void)
{
	USART1->CR1 |= USART_CR1_RXNEIE;

	NVIC_EnableIRQ(USART1_IRQn);
	NVIC_SetPriority(USART1_IRQn, 0);
	NVIC_ClearPendingIRQ(USART1_IRQn);
}

/**
  * @brief	This function sends a string of characters through USART1.
  *				 If a LINEFEED character ('\n') is detected, the functions also
  *				 sends a CONTROL ('\r') character.
  * @param	str: NULL ('\0') terminated string
  * @retval None
  */
void USARTPutStr(char *str)
{
	while(*str)
	{
		if(*str == '\n')
		{
			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){;}
			USART_SendData(USART1, '\r');
		}
	
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){;}
		USART_SendData(USART1, *str++);
	}
}

void USARTPutChar(char c)
{
		// Wait for Transmit data register empty
		while((USART1->ISR & USART_ISR_TXE) == 0) ;

		// Transmit data by writing to TDR, clears TXE flag	
		USART1->TDR = c;
}

/**
  * @brief	This function implements the following VT100 terminal commands
  *				 * Clear screan
  *				 * Cursor home
  * @param	None
  * @retval None
  */
void USARTClearScreen(void)
{
		char cmd1[5] = {0x1B, '[', '2', 'J', '\0'}; // Clear screen
		char cmd2[4] = {0x1B, '[', 'f', '\0'}; // Cursor home
	
		USARTPutStr(cmd1);
		USARTPutStr(cmd2);
}

/**
  * @brief	This function converts an integer to an ASCII string of characters
  *				 The result is always a four digit number, so leading zero's are
  *				 inserted if neccesary. The string is terminated with a NULL ('\0')
  *				 character.
  * @param	i: the number to convert. Negative numbers are also allowed.
  *				 p: pointer to the destination buffer
  * @retval pointer to the start of the destination buffer
  */
char *USARTIntToAscii(int16_t i, char *p)
{
	int16_t t1, t2;
	char h[10];

	t1 = t2 = 0;
	if (i < 0)
	{
		p[t2] = '-';
		t2++;
		i = -i;
	}

	if(i<1000){ p[t2++]='0'; }
	if(i< 100){ p[t2++]='0'; }
	if(i<  10){ p[t2++]='0'; }
	
	do
	{
		h[t1] = i % 10 + 48;
		t1++;
		i = i / 10;
	}
	while (i > 0);

	while (t1 > 0)
	{
		p[t2++] = h[--t1];
	}

	p[t2] = '\0';

	return(p);
}

void USARTPrintNumber(int16_t number, char *str)
{
	char numbers[255];
	uint16_t x = 0;
	USARTPutStr(str);
	sprintf(numbers, "%d", number);
	while(numbers[x] != '\0')
	{
		USARTPutChar(numbers[x]);
		x++;
	}
}

void USARTPrintNumberFloat(float number, char *str)
{
	char numbers[255];
	uint16_t x = 0;
	USARTPutStr(str);
	sprintf(numbers, "%f", number);
	while(numbers[x] != '\0')
	{
		USARTPutChar(numbers[x]);
		x++;
	}
}
