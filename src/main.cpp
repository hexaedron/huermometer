#include <ch32v003fun.h>
#include "funny_defs.h"
#include "funny_time.h"
#include "simpleTimer.h"
#include "adc.h"
#include "therm.h"

#define WS2812BSIMPLE_IMPLEMENTATION
#define LED_BRIGHTNESS 8
#define TEMP_HIGH 300
#define TEMP_LOW 170

uint8_t map8(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max)
{
	return  (uint8_t)((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);
}

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

	adc_init(7<<(3*7));
	thermistor myTherm(PA1, resistorValues_t::resistor_10K);
	
	uint8_t leds[NUM_LEDS * 3] = {0};

	uint8_t hue = 0;
	uint32_t rgb;
	bool start = true;
	simpleTimer32 tmr(1000UL);
	int16_t temp = myTherm.getTemp();
	while(1)
	{
		if(tmr.ready())
		{
			temp = myTherm.getTemp();
		}

		for (uint8_t i = (uint8_t)start; i < NUM_LEDS; i+=2)
		{
			hue =  map8(constrain(temp, TEMP_LOW, TEMP_HIGH), TEMP_LOW, TEMP_HIGH, 255, 0);
			rgb = EHSVtoHEX(hue, 255, LED_BRIGHTNESS);
			uint8_t* tmp = (uint8_t*)(&rgb);
			leds[i*3] = tmp[0];
			leds[i*3+1] = tmp[1];
			leds[i*3+2] = tmp[2];
		}
		WS2812BSimpleSend(GPIOC, 6, (uint8_t*)leds, sizeof(leds));
		//Delay_Ms(50);
		start = !start;
	}	
}