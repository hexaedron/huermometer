#include <ch32v003fun.h>
#include <cstring>
#include "funny_defs.h"
#include "funny_time.h"
#include "simpleTimer.h"
#include "adc.h"
#include "therm.h"

#define WS2812BSIMPLE_IMPLEMENTATION
#define LED_BRIGHTNESS 16
#define TEMP_HIGH 300
#define TEMP_LOW 170
#define NUM_LEDS 8

uint8_t map8(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max)
{
	return  (uint8_t)((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);
}

uint16_t tempLog[8] = {0};
void push_temp(int16_t newtemp)
{
	static uint8_t tempPointer = 0;
	tempLog[tempPointer] = newtemp;

	tempPointer++;
	if(tempPointer >= 8) tempPointer = 0;
}

int16_t filterTemp(void)
{
	int32_t sum = 0;
	for (uint8_t i = 0; i < 8; i++)
	{
		sum += tempLog[i];
	}

	return (int16_t)(sum >> 3);
}

#include "ws2812b_simple.h"
#include "color_utilities.h"

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
	uint8_t start = 0;
	simpleTimer32 tmr(125UL);
	int16_t temp = myTherm.getTemp();
	
	for (uint8_t i = 0; i < 8; i++)
	{
		push_temp(temp);
	}
	

	while(1)
	{
		if(tmr.ready())
		{
			push_temp(myTherm.getTemp());
		}

		memset(leds, 0, sizeof(leds));
		for (uint8_t i = (uint8_t)start; i < NUM_LEDS; i+=2)
		{
			hue =  map8(constrain(filterTemp(), TEMP_LOW, TEMP_HIGH), TEMP_LOW, TEMP_HIGH, 255, 0);
			rgb = EHSVtoHEX(hue, 255, LED_BRIGHTNESS);
			uint8_t* tmp = (uint8_t*)(&rgb);
			leds[i*3] = tmp[0];
			leds[i*3+1] = tmp[1];
			leds[i*3+2] = tmp[2];
		}
		WS2812BSimpleSend(GPIOC, 6, (uint8_t*)leds, sizeof(leds));
		start++; 
		if(start > 1 ) start = 0;
	}	
}