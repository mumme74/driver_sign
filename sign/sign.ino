#include <stdint.h>
#include <SPI.h>
#include "epd4in26.h"
#include "images.h"

/**
* This is a code used to select current driver and show it on a EINK Display to the outside.
*/

// The possible chars to select
static const struct vlu_img * images[] = {
  &image_A, &image_B, &image_C,
  &image_D, &image_E, &image_F,
  &image_G, &image_H, &nj_racing, 
  nullptr, // nullptr marks be the end 
  // comment out unneccesary if need to save space
  // using the compression method the whole alphabet fits into memory
  // on an R4
  // &image_J, &image_K, &image_L,
  // &image_M, &image_N, &image_O,
  // &image_P, &image_Q, &image_R,
  // &image_S, &image_T, &image_U,
  // &image_V, &image_W, &image_X,
  // &image_Y, &image_Z
};
static constexpr uint8_t images_sz = 
  sizeof(images) / sizeof(images[0]);

// input pins mapping, 
// pos0 = images[0], pos1 = image[1] and so on
static const uint8_t input_pins[] = {
  14,15,16,17,18,19,2,3,4,5
};
static constexpr uint8_t input_pins_sz =
  sizeof(input_pins) / sizeof(input_pins[0]);

// the display communication class
static Epd epd; 

// forward declare
void draw(const struct vlu_img* curImg);

void setup() {
   // put your setup code here, to run once:
  Serial.begin(115200);
  if (epd.Init() != 0) {
      Serial.print("e-Paper init failed");
      return;
  }
  Serial.print("4.26inch e-paper sign\r\n ");
  Serial.print("e-Paper Clear\r\n ");

  epd.Clear();

  // set inputs with pullup
  for (uint8_t i = 0, end = min(input_pins_sz, images_sz); 
       i < end; ++i) 
  {
    pinMode(input_pins[i], INPUT_PULLUP);
  }
}

void loop() {
  // static have a lifetime outside of this function call
  static const struct vlu_img* prevImg = nullptr; // default to null so it renders the first time

  // loop input switch to find which char to display.
  for (uint8_t i = 0, end = min(input_pins_sz, images_sz); 
       i < end; ++i) 
  {
    if (digitalRead(input_pins[i]) == 0) {
      if (prevImg != images[i]) {
        Serial.print("Changed to: ");
        Serial.println(static_cast<char>('A' + i));
        prevImg = images[i];
        draw(prevImg);
      }
      break;
    }
  }
}

void send_byte(uint8_t data) {
  epd.SendData(data);
}

// uncompress and draw at the same time.
void draw(const struct vlu_img* curImg) {
  if (curImg == nullptr) {
    epd.Clear();
    return;
  }

  epd.SendCommand(0x24);
  int res = images_render(
              curImg, send_byte, EPD_HEIGHT*EPD_WIDTH);
  if (!res)
    Serial.println("Error: Failed to render, maybe image to large for the screen?");

  epd.TurnOnDisplay_Fast();
}
