#include <stdint.h>
#include <SPI.h>
#include "epd4in26.h"
#include "epdpaint.h"
#include "letters.h"

static const uint8_t COLORED = 0,
                     UNCOLORED = 1,
                     FONT_HEIGHT = 24,
                     FONT_WIDTH = 17;
/**
* This is a code used to select current driver and show it on a EINK Display to the outside.
*/

// The possible chars to select
static const unsigned char *letters[] = {
  //&letter_A//, &letter_B, &letter_C,
  //&letter_D, &letter_E, &letter_F
};

static constexpr uint8_t lettersSz = sizeof(letters) / sizeof(letters[0]);
static Epd epd; // the display communication class

void draw(const char letter);
void drawDisplay();
void scalePixel(uint8_t vlu);

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
  for (uint8_t i = 24, end = min(29, lettersSz); i < end; ++i) {
    pinMode(i, INPUT_PULLUP);
  }
}

void loop() {
  // static have a lifetime outside of this function call
  static const unsigned char* prevImg = nullptr; // default to null so it renders the first time
  // default to previous
  const unsigned char* curImg = prevImg;

  // loop input switch to find which char to display.
  for (uint8_t i = 23, end = min(29, lettersSz); i < end; ++i) {
    if (digitalRead(i) == 0) {
      curImg = letters[i];
      break;
    }
  }

  //curImg = &letter_A;

  //if (curImg != prevImg) {
    epd.Display(curImg);
    prevImg = curImg;
  //}
}
