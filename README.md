## This is simple ardino app displaying current driver in a racecar  

# Usage:  
The display used is an E-ink display like this to fix in the cars side window.  
https://www.electrokit.com/3.52e-papper-display-360x240px-svart/vit-for-raspberry-pi?gad_source=1&gbraid=0AAAAAD_OrGNEUcbfV6Jq9f9js4fKQiOy5&gclid=EAIaIQobChMI26n-j57-jAMVQRiiAx2IVCUJEAQYAiABEgJEj_D_BwE

The display needs to be set up to communicat through SPI as decribed in 
https://www.waveshare.com/wiki/3.52inch_e-Paper_HAT_Manual#Working_With_Arduino  

Pinout E-paper to Arduino Uno:  

|  e-paper   | Arduino UNO  |  
| ---------- | ------------ |  
| VCC        |  5V          |  
| GND        |  GND         |  
| DIN        |  D11         |  
| CLK        |  D13         |  
| CS         |  D10         |  
| DC         |  D9          |  
| RST        |  D8          |  
| BUSY       |  D9          |       


Needs a rotary switch to select driver, such as:  
https://www.electrokit.com/upload/product/41002/41002011/LorlinCK-4.pdf  

As software is is currently written it works in an inverted state, meaning if you drag a line low it will trigger. Inputs pin23 to pin28 on an Arduino UNO