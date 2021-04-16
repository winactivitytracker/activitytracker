#include <board.h>
#include <periph/gpio.h>
#include <xtimer.h>
#include <thread.h>

int main()
{
	board_init();
	
	/*
	gpio_init(GPIO_PIN(PORT_A, 2), GPIO_OUT);
	gpio_init(GPIO_PIN(PORT_B, 2), GPIO_OUT);
	
	gpio_set(GPIO_PIN(PORT_A, 2));
	gpio_set(GPIO_PIN(PORT_B, 2));
	
	while(1)
	{
		gpio_toggle(GPIO_PIN(PORT_A, 2));
		gpio_toggle(GPIO_PIN(PORT_B, 2));
			
		xtimer_sleep(1);
	}
	*/
	
	// GPIOA Periph clock enable
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN; 
	
	// GPIOB Periph clock enable
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN; 

	// A2 and B2 in output mode
	GPIOA->MODER |= (GPIO_MODER_MODER2_0);
	GPIOB->MODER |= (GPIO_MODER_MODER2_0);

	// Push pull mode selected
	GPIOA->OTYPER &= ~(GPIO_OTYPER_OT_2);
	GPIOB->OTYPER &= ~(GPIO_OTYPER_OT_2);
	
	// Maximum speed setting
	GPIOA->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR2);
	GPIOB->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR2);

	// Pull-up and pull-down resistors disabled
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR2);
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR2);

	while(1)
	{
		// Set PC9
		GPIOA->BSRR = GPIO_BSRR_BS_2;
		// Reset PC8
		GPIOB->BSRR = GPIO_BSRR_BR_2;
		
		// Delay ~ 1 sec.
		xtimer_sleep(1);	
		
		// Reset PC9
		GPIOA->BSRR = GPIO_BSRR_BR_2;
		// Set PC8
		GPIOB->BSRR = GPIO_BSRR_BS_2;
		
		// Delay ~ 1 sec.
		xtimer_sleep(1);
	}
	
	return 0;
}
