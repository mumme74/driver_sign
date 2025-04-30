#include <stdint.h>
#include <SPI.h>
#include "epd4in26.h"
#include "epdpaint.h"
#include "letters.h"

/**
* This is a code used to select current driver and show it on a EINK Display to the outside.
*/

// The possible chars to select
static const struct vlu_img * letters[] = {
  &letter_A, &letter_B, &letter_C,
  &letter_D, &letter_E, &letter_F,
  &letter_G, &letter_H, &letter_I, 
  nullptr, // nullptr marks be the end 
  // comment out unneccesary if need to save space
  &letter_J, &letter_K, &letter_L,
  &letter_M, &letter_N, &letter_O,
  &letter_P, &letter_Q, &letter_R,
  &letter_S, &letter_T, &letter_U,
  &letter_V, &letter_W, &letter_X,
  &letter_Y, &letter_Z
};
static constexpr uint8_t letters_sz = 
  sizeof(letters) / sizeof(letters[0]);

static const uint8_t input_pins[] = {
  14,15,16,17,18,19,2,3,4,5
};
static constexpr uint8_t input_pins_sz =
  sizeof(input_pins) / sizeof(input_pins[0]);

static Epd epd; // the display communication class

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
  for (uint8_t i = 0, end = min(input_pins_sz, letters_sz); 
       i < end; ++i) 
  {
    pinMode(input_pins[i], INPUT_PULLUP);
  }
}

void loop() {
  // static have a lifetime outside of this function call
  static const struct vlu_img* prevImg = nullptr; // default to null so it renders the first time

  // loop input switch to find which char to display.
  for (uint8_t i = 0, end = min(input_pins_sz, letters_sz); 
       i < end; ++i) 
  {
    if (digitalRead(input_pins[i]) == 0) {
      if (prevImg != letters[i]) {
        Serial.print("Changed to: ");
        Serial.println(static_cast<char>('A' + i));
        prevImg = letters[i];
        draw(prevImg);
      }
      break;
    }
  }
}

void draw(const struct vlu_img* curImg) {
  if (curImg == nullptr) {
    epd.Clear();
    return;
  }

  epd.SendCommand(0x24);
  uint32_t pixelCnt = 0;
  uint8_t byt = 0, p = 0;
  for (uint16_t i = 0; i < curImg->n_segs; i++) {
    const auto* seg = &curImg->segs[i];
    for (uint16_t j = 0; j < seg->len; j++) {
      if (EPD_HEIGHT*EPD_WIDTH < ++pixelCnt) {
        Serial.println("To large image for display");
        break;
      }

      // rebuild the byte (8 pixels)
      byt |= seg->vlu << (7-p);
      if (8 == ++p) {
        epd.SendData(byt);
        byt = p = 0;
      }
    }
  }

  epd.TurnOnDisplay_Fast();
}
