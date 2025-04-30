#include <stdint.h>
#include <SPI.h>
#include "epd4in26.h"
#include "epdpaint.h"
#include "fonts.h"

static const uint8_t COLORED = 0,
                     UNCOLORED = 1,
                     FONT_HEIGHT = 24,
                     FONT_WIDTH = 17;
/**
* This is a code used to select current driver and show it on a EINK Display to the outside.
*/

// The possible chars to select
static constexpr char letters[] = "ABCDEF";
static constexpr uint8_t lettersCnt = sizeof(letters) / sizeof(letters[0]);
static uint8_t charCanvas[(24/8)*17] = {0}; // draw char canvas memory
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

  delay(2000);

  // set inputs with pullup
  for (uint8_t i = 24, end = min(29, lettersCnt); i < end; ++i) {
    pinMode(i, INPUT_PULLUP);
  }
}

void loop() {
  // static have a lifetime outside of this function call
  static char prevChar = '/0'; // default to null so it renders the first time
  // default to previous
  char curChar = prevChar;

  // loop input switch to find which char to display.
  for (uint8_t i = 23, end = min(29, lettersCnt); i < end; ++i) {
    if (digitalRead(i) == 0) {
      curChar = letters[i];
      break;
    }
  }

  curChar = 'A';

  //if (curChar != prevChar) {
    draw(curChar);
    prevChar = curChar;
  //}
}

void draw(const char letter) {
  Paint paint(charCanvas, Font24.Width, Font24.Height); // width should be the multiple of 8   

  //paint.SetRotate(ROTATE_90);
  paint.Clear(UNCOLORED);

  Serial.print("Drawing letter: '"); Serial.print(letter); Serial.print("'\r\n ");
  paint.DrawCharAt(0, 0, letter, &Font24, COLORED);
  drawDisplay();
  epd.TurnOnDisplay();
}

void drawDisplay() {
  Serial.println("-------------");
  for (int y = 0; y < Font24.Height; ++y) {
    for (int x = 0; x <= Font24.Width/8; ++x) {

      char buf[10] = {0};
      snprintf(buf,10, "0x%x ", charCanvas[y+x]);
      Serial.print(buf);
    }
    Serial.print('\n');
  }
  Serial.println("-------------");
  
  epd.SendCommand(0x24);

  for (int y = 0; y < Font24.Height; ++y) {
    uint8_t byt = 0, p = 0;
    for (int x = 0; x < Font24.Width; ++x) {
      byt |= charCanvas[x/8 + y] << (8-p);
      if (8 == ++p) {
        --p;
        char buf[10] = {0};
        snprintf(buf,10, "0x%x ", byt);
        Serial.print(buf);
        for (; p < 0xff; p--) {
          uint8_t vlu = (byt & (1 << p)) >> p;
          scalePixel(vlu);
        }
        byt = p = 0;
      }
    }
    Serial.print('\n');
  }

  epd.TurnOnDisplay_Fast();
}

void scalePixel(uint8_t vlu) {
  // fonts are 17pixels wide, 800 is the width so nearest is scalefactor 28
  const int xScaleFactor = EPD_WIDTH / Font24.Width / 8,
            yScaleFactor = EPD_HEIGHT / Font24.Height / 8;
  
  for (int i = 0; i < yScaleFactor ; ++i) {
    uint8_t byt = 0, p = 0;
    for (int j = 0; j < xScaleFactor; ++j) {
      byt |= 1 << p;
      if (8 == ++p) {
        epd.SendData(byt);
        byt = p = 0;
      }
    }
    if (p < 8)
      epd.SendData(byt);
  }
}
