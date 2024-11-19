#include <ch32v003fun.h>
#include "funny_defs.h"
#include "funny_time.h"
#include "simpleTimer.h"

#define WS2812BSIMPLE_IMPLEMENTATION
//#define WSRBG //For WS2816C's.
#define WSGRB // For SK6805-EC15

//#include "ws2812b_dma_spi_led_driver.h"
#include "ws2812b_simple.h"

#include "color_utilities.h"

#define NUM_LEDS 8

int main()
 {
	SystemInit();
#ifdef WCH_FAST_INTERRUPT_ENABLED
	__set_INTSYSCR(0x3); // [Experimental] enable fast interrupt feature
#endif
	system_initSystick();

	funGpioInitD();
	
	uint8_t leds[NUM_LEDS * 3] = {0};

	uint8_t hue = 0;
	uint32_t rgb;
	while(1)
	{
		for (uint8_t i = 0; i < NUM_LEDS; i++)
		{
			//uint8_t* tmp = (uint8_t*)(&ledsEHS[i]);
			rgb = EHSVtoHEX(hue, 255, 16);
			uint8_t* tmp = (uint8_t*)(&rgb);
			leds[i*3] = tmp[0];
			leds[i*3+1] = tmp[1];
			leds[i*3+2] = tmp[2];
		}
		WS2812BSimpleSend(GPIOC, 6, (uint8_t*)leds, sizeof(leds));
		Delay_Ms(50);
		hue--;
	}	
}