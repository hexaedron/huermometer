#include <ch32v003fun.h>
#include "funny_defs.h"
#include "funny_time.h"
#include "simpleTimer.h"


#include <stdbool.h>
#include <cstdlib>
#include <stdio.h>

int main()
 {
	SystemInit();
#ifdef WCH_FAST_INTERRUPT_ENABLED
	__set_INTSYSCR(0x3); // [Experimental] enable fast interrupt feature
#endif
	system_initSystick();

	funGpioInitAll();

	funPinMode(PC1, GPIO_Speed_10MHz | GPIO_CNF_OUT_PP);
	funPinMode(PD2, GPIO_Speed_In | GPIO_CNF_IN_FLOATING);
	funPinMode(PC2, GPIO_Speed_10MHz | GPIO_CNF_OUT_PP); // LED
	

	while (1)
	{
		
	}
	
}