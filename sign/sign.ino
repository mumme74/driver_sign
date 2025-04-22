#include <stdint.h>

/**
* This is a code used to select current driver and show it on a BCD Display to the outside.
*  Pins 23 to 28 select driver, can only be low at a time
*  Pins 2-9 controls output segments in the display
*  Display segments: A=2,B=3,C=4,D=5,E=6,F=7,G=8
*        A
*    ---------
*   |         |
* F |         | B
*   |    G    |
*   + -------- 
*   |         |
* E |         | C
*   |    D    |
*    ---------
*/
enum Letters_e: uint8_t {
    letter_A,
    letter_B,
    letter_C,
    letter_D,
    letter_E,
    letter_F,
    _endletters,
};

static uint8_t letters[_endletters] = {
  0b01110111, // an A
  0b01111111, // a  B
  0b00111001, // a  C
  0b00111111, // a  D
  0b01111001, // an E
  0b01110001, // a  F 
};

void setup() {
  // set outputs as outputs
  // pin PD2-PD7 och PB0 (pin2-8)
  for (uint8_t i = 2; i < 9; i++)
    pinMode(i, OUTPUT);
  
  // set inputs as inputs with pull up
  // PC0-PC5 (23-28)
  for (uint8_t i = 23; i < 29; ++i)
    pinMode(i, INPUT_PULLUP);
}

void loop() {
  // defaulta till letter_A
  uint8_t outbits = letters[letter_A];

  if (digitalRead(24) == 0)
    outbits = letters[letter_B];
  else if (digitalRead(25) == 0)
    outbits = letters[letter_C];
  else if (digitalRead(26) == 0)
    outbits = letters[letter_D];
  else if (digitalRead(27) == 0)
    outbits = letters[letter_E];
  else if (digitalRead(28) == 0)
    outbits = letters[letter_F];

  // sets output bits for the 6 PDx ports, bit mainuplate to go from 0 bit => PD2 - PD7 
  PORTD = ((outbits << 2) & 0xFC) | (PORTD & 0x02);
  // sets output bits for the PB0 bit , bit manipulate to leave the rest intact
  PORTB = ((outbits & 0x80) >> 6) | (PORTB & 0xFC);
}
