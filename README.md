## This is simple ardino app displaying current driver in a racecar  

# Usage:  
The display used is an BCD display like this to fix in the cars side window.  
https://www.electrokit.com/upload/product/41003/41003272/YSD-1600AR6F-89.pdf  

The display need an series 100 ohms resitor in series with an NPN dirver transistor for each BCD segment.  
The bas driver resitor can be something lika 1kohm and transitor a simple BC547C.  
Outputs goes to pin2-8 on an Arduino UNO.

Needs a rotary switch to select driver, such as:  
https://www.electrokit.com/upload/product/41002/41002011/LorlinCK-4.pdf  

As software is is currently written it works in an inverted state, meaning if you drag a line low it will trigger. Inputs pin23 to pin28 on an Arduino UNO