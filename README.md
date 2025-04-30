## This is simple ardino app displaying current driver in a racecar  

# Usage:  
The display used is an E-ink display like this to fix in the cars side window.  
https://www.electrokit.com/4.26e-papper-display-800x480px-svart/vit-spi

The display needs to be set up to communicat through SPI as decribed in 
https://www.waveshare.com/wiki/3.52inch_e-Paper_HAT_Manual#Working_With_Arduino  

Pinout E-paper to Arduino Uno:  

|  e-paper   | Arduino UNO R4  |  
| ---------- | ----------------|  
| VCC        |  5V             |  
| GND        |  GND            |  
| DIN        |  D11            |  
| CLK        |  D13            |  
| CS         |  D10            |  
| DC         |  D9             |  
| RST        |  D8             |  
| BUSY       |  D9             |       


Needs a rotary switch to select driver, such as:  
https://www.electrokit.com/upload/product/41002/41002011/LorlinCK-4.pdf  

As software is is currently written it works in an inverted state, meaning if you drag a line low it will trigger. Inputs pin23 to pin28 on an Arduino UNO