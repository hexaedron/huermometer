# huermometer

A small night lamp - thermometer.
It uses the NTC thermistor to measure temperature and 8LED WS2812 ring module from Aliexpress. It changes its light from purple (17°C and below) to red (30°C and above).
I've made it as an upgrade of a cheap glass ball souvenir.

## Usage
Just power it up from USB.

## BOM
* CH32V003F4P6
* 10K resistor
* NTC 3950 100K
* 8 LED WS2812 module from Aliexpress (i've used the ring shaped module, but a linear one is also an option. It depends on the form you want to get)

## Assembly
![photo](/pics/schematic.png)

## Build
You will need [ch32v00fun](https://github.com/cnlohr/ch32v003fun/) and [funny_libs](https://github.com/hexaedron/funny_libs) to build it. I prefer using official WCH toolchain. If you prefer using vanilla risc-v GCC, then remove `#define WCH_FAST_INTERRUPT_ENABLED` from `funny_defs.h`.

![photo](/pics/pic1.jpg)
![photo](/pics/pic2.jpg)
