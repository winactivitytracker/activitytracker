#ifndef __main_h
#define __main_h

#include "stm32f0xx_i2c.h"
#include "I2C_main.h"
#include "MPU_6050_main.h"
#include "USART_main.h"
#include "MAX30102_main.h"
#include "stdbool.h"

#define ONE_TENTHOUSEND_SECOND  		(SystemCoreClock/8/10000)
#define ONE_THOUSEND_SECOND  				(SystemCoreClock/8/1000)
#define ONE_HUNDERDTH_SECOND  			(SystemCoreClock/8/100)
#define ONE_FIFTYTH_SECOND  				(SystemCoreClock/8/50)
#define ONE_TENTH_SECOND  					(SystemCoreClock/8/10)
#define HALF_SECOND   							(SystemCoreClock/8/5)
#define ONE_SECOND         					(SystemCoreClock/8)

#endif //__main_h
