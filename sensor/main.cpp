#include <board.h>
#include <periph/gpio.h>
#include <xtimer.h>
#include <thread.h>

static char stack[THREAD_STACKSIZE_MAIN];

int main()
{
	board_init();
	
	kernel_pid_t pid;
	pid = thread_create(stack,		/* stack array pointer */
		sizeof(stack),			/* stack size */
		THREAD_PRIORITY_MAIN - 1,	/* thread priority*/
		flag,				/* thread configuration flag, usually*/
		thread_handler,			/* thread handler function */
		NULL,				/* argument of thread_handler function*/
		"thread name"
	);
	
	LED0_ON;
	LED1_OFF;
	
	while(1)
	{
		LED0_TOGGLE;
		LED1_TOGGLE;
		xtimer_sleep(1);
	}
	
	
	return 0;
}
