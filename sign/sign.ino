#include <stdint.h>
#include <SPI.h>
#include "EPD_3in52.h"
#include "epdpaint.h"
#include "fonts.h"

static const uint8_t COLORED = 0,
                     UNCOLORED = 1;
/**
* This is a code used to select current driver and show it on a EINK Display to the outside.
*/

// The possible chars to select
static constexpr char letters[] = "ABCDEF";
static constexpr uint8_t lettersCnt = sizeof(letters) / sizeof(letters[0]);
static uint8_t charCanvas[(24*17)/8] = {0}; // draw char canvas memory
static Epd epd; // the display communication class

void draw(const char letter);
void scaleUpAndSend();

void setup() {
   // put your setup code here, to run once:
   Serial.begin(115200);
   if (epd.Init() != 0) {
       Serial.print("e-Paper init failed");
       return;
   }
   Serial.print("3.52inch e-paper demo\r\n ");
   Serial.print("e-Paper Clear\r\n ");

   epd.display_NUM(EPD_3IN52_WHITE);
   epd.lut_GC();
   epd.refresh();

   epd.SendCommand(0x50);
   epd.SendData(0x17);

   delay(2000);
}

void loop() {
  // static have a lifetime outside of this function call
  static char prevChar = '/0'; // default to null so it renders the first time
  // default to previous
  char curChar = prevChar;

  // loop input switch to find which char to display.
  for (uint8_t i = 24, end = min(29, lettersCnt); i < end; ++i) {
    if (digitalRead(i) == 0) {
      curChar = letters[i];
      break;
    }
  }

  if (curChar != prevChar) {
    draw(curChar);
    prevChar = curChar;
  }
}

void draw(const char letter) {
  Paint paint(charCanvas, Font24.Width, Font24.Height); // width should be the multiple of 8   

  paint.SetRotate(ROTATE_90);
  paint.Clear(UNCOLORED);

  Serial.print("Drawing letter: '"); Serial.print(letter); Serial.print("'\r\n ");
  paint.DrawCharAt(0, 0, letter, &Font24, COLORED);
  scaleUpAndSend();
  epd.lut_GC();
  epd.refresh();
  // delay(2000);
}

void scaleUpAndSend() {
  epd.SendCommand(0x13);

  // fonts are 17pixels wide, 240 is the width so nearest is scalefactor 14
  for (int x = 0; x < 24; ++x) {
    for (int y = 0; y < 17; ++y) {
      for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 14; ++j) {
          epd.SendData(charCanvas[x * 24 + y]);
        }
      }
    }
  }
}
