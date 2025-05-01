# Racecar driver screen
This is simple ardino app displaying current driver in a racecar  
It's actually displays images of letters on an E-ink display.  
You can of course change those images to another one if you want, as long as it black and white and sized as screen size (800 * 480 pixels)  
See advanced section for how to do that.  

# Short video
https://youtu.be/RyOxi74VbmI  

# Hardware:  
The display used is an E-ink display like this to fix in the cars side window.  
https://www.electrokit.com/4.26e-papper-display-800x480px-svart/vit-spi

The display needs to be set up to communicat through SPI as decribed in 
https://www.waveshare.com/wiki/4.26inch_e-Paper_HAT_Manual#Working_With_Arduino  

## Pinout
Pinout E-paper to Arduino Uno R4 (tested with R4 wifi):  

|  e-paper   | Arduino UNO R4  |  
| ---------- | ----------------|  
| VCC        |  5V             |  
| GND        |  GND            |  
| DIN        |  D11            |  
| CLK        |  D13            |  
| CS         |  D10            |  
| DC         |  D9             |  
| RST        |  D8             |  
| BUSY       |  D7             |  
| PWR        |  D6             |  
  
  
## Inputs, driver selection
Needs a rotary switch to select driver, such as:  
https://www.electrokit.com/upload/product/41002/41002011/LorlinCK-4.pdf  
  
As software is is currently written it works in an inverted state, meaning if you drag a line low it will trigger. Inputs pin23 to pin28 on an Arduino UNO
  
### Screen burn in
To mitigate the risk of burn in, the highest available input, currently pin 4, should clear the screen. Be sure to do this if display is stored for a longer period.  
  
### Pinout inputs
To select a driver you need to ground its equivialent input. The highest input available clears the screen. 
  
Pinout of inputs is simple to change in sotfware, but default should be fine. They are:  
|  pin    |  image nr |  
| ------- | --------- |  
| A0 (14) | image_A   |  
| A1 (15) | image_B   |  
| A2 (16) | image_C   |  
| A3 (17) | image_D   |  
| A4 (18) | image_E   |  
| A6 (19) | image_F   |  
| 2       | image_G   |  
| 3       | mj_racing |  
| 4       | *clear*   |  

The images order and/or images are also easily changable in software if you want. See advanced how to do that.  

## Build software  
While your arduino is attached through USB:  
Open Arduino IDE in the sign folder, select correct bord and com port, press upload.  

# Advanced  
Here are some tips for how change the defaults.
Be sure to tak a copy before you start changing so you can easily restore the files if needed.  

## Change image order (or add new images) 
To add new images see section how to embedd images.  
   
In the file sign.ino change the items in the *images* array, be sure to include a nullptr i the last item. Also make sure each image name is prepended with the adress of operator (&) and separated by a commma.

```c++
static const struct vlu_img * images[] = {
  // Change these item...
  &image_A, &image_B, &image_C,
  &image_D, &image_E, &image_F,
  ...
  // Make sure the last one is a nullptr
  nullptr,
  // Make sure you don't exceed the number inputs. 
  // Currently you can display 9 images 
  // and one clear screen nullptr. 
}
```

## Change the input pins ordering  
To change the order and/or number of inputs, do this.  
In the file sign.ino change the the pin number in the *input_pins* array. 
```c++
static const uint8_t input_pins[] = {
  // change the order here
  // make sure these input don't go into the 
  // IO pins that controls the display.
  14,15,16,17,18,19,2,3,4,5
  // also make sure the number of inputs 
  // matches the number of images.
}
```

## Change font for character images.
You can change the fonts and generate new embedded images with some python scripts.  
See python section if you need help on setting up python environment.
  
The scripts are located in the *letter_images* folder. To generate new characters you edit at the top of *create_letter_images.py*.  
Change to your desired font and size. If you plan on a another screen size, you need to change that to.  
Although changing screen size also requires to use another *epdXinXX* *.cpp* and *.h* files also. That you have to figure out for your self...

It is always a good idea to use a monospaced font, meaning that each char takes up the same amount of screen space.  

When done, just run the script, it should (re-)create the *image_xx.png* files in this folder.  
You should now view those images so the fit and work as expected, before you embedd them.  

## Embedd images to the software.
In order for an image to be displayed from a micro controller, such as an Arduino, we need to embedd the data into the binary before we build.  

This software use a crude form of image compression So that many images can fit in memory.  

The algorithm is simply count how many bytes have the same color and store that along with what color. This saves a ton of space if many lines in the images is colored.  
But it does mean that we need to use a custom script to embed that data into our software.  

To create an embeddable *images.c* file we run the script *create_embedded_images.py*.  
All *.png* images in the folder *letter_images* are read and compressed into c style arrays that we later can use in our arduino sketch.  

All *.png* files means that you can embedd a black/white (2 color spaced) png file of the correct screen size (800 * 480) and it will get included into the *images.c* file during script execution.  

Make sure the filenames don't include non english characters. I do replace ' ' with '_' and 'åäö' with 'a' and 'o', but its better to only use english chars for filnames.  
The filenames later becomes an identifier in the c code so this is a **hard requirement**!

When that file has runned successfully it (re-)creates the files *images.c* and *images.h*.  
You need to copy these files into the sign folder, be sure that the old ones are removed before pasting those files.

To activate the new image, see section *Change image order* in this readme.

## Setting up python
You need to have python3 installed to use to recreate images. I used version 3.13 but it should work with at least 3.11.  

On Mac and Linux python is installed by default so it should work out of the box as long as the version is not ancient.  

On Windows you go to Windows store and search for python and install it.  

### Python dependencies
These scripts need python package Pillow to read, draw and save the images.  

Plese follow the manual for pillow if you need to install that:  
https://pillow.readthedocs.io/en/stable/installation/basic-installation.html#basic-installation

