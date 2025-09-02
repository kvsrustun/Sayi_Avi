#include <Deneyap_5x7LedEkran.h>        

DotMatrix LEDMatris1;
DotMatrix LEDMatris2;

#define BUTTON_PIN D2
bool buttonPressed = false;
bool lastButtonReading = HIGH;
int currentNumber1 = -1;
int currentNumber2 = -1;

static const byte digits[6][7] = {
  {0b00100, 0b01100, 0b00100, 0b00100, 0b00100, 0b00100, 0b01110}, 
  {0b01110, 0b10001, 0b00001, 0b00010, 0b00100, 0b01000, 0b11111}, 
  {0b11110, 0b00001, 0b00001, 0b01110, 0b00001, 0b00001, 0b11110}, 
  {0b00010, 0b00110, 0b01010, 0b10010, 0b11111, 0b00010, 0b00010}, 
  {0b11111, 0b10000, 0b11110, 0b00001, 0b00001, 0b10001, 0b01110}, 
  {0b00110, 0b01000, 0b10000, 0b11110, 0b10001, 0b10001, 0b01110}  
};

void displayNumberOnDotMatrix(DotMatrix &ledMatris, int number) {
  if (number < 1 || number > 6) {
    clearDotMatrix(ledMatris);
    return;
  }
  int idx = number - 1;
  ledMatris.dotrow1((digits[idx][0] & 0x10) ? 1 : 0, (digits[idx][0] & 0x08) ? 1 : 0, (digits[idx][0] & 0x04) ? 1 : 0, (digits[idx][0] & 0x02) ? 1 : 0, (digits[idx][0] & 0x01) ? 1 : 0);
  ledMatris.dotrow2((digits[idx][1] & 0x10) ? 1 : 0, (digits[idx][1] & 0x08) ? 1 : 0, (digits[idx][1] & 0x04) ? 1 : 0, (digits[idx][1] & 0x02) ? 1 : 0, (digits[idx][1] & 0x01) ? 1 : 0);
  ledMatris.dotrow3((digits[idx][2] & 0x10) ? 1 : 0, (digits[idx][2] & 0x08) ? 1 : 0, (digits[idx][2] & 0x04) ? 1 : 0, (digits[idx][2] & 0x02) ? 1 : 0, (digits[idx][2] & 0x01) ? 1 : 0);
  ledMatris.dotrow4((digits[idx][3] & 0x10) ? 1 : 0, (digits[idx][3] & 0x08) ? 1 : 0, (digits[idx][3] & 0x04) ? 1 : 0, (digits[idx][3] & 0x02) ? 1 : 0, (digits[idx][3] & 0x01) ? 1 : 0);
  ledMatris.dotrow5((digits[idx][4] & 0x10) ? 1 : 0, (digits[idx][4] & 0x08) ? 1 : 0, (digits[idx][4] & 0x04) ? 1 : 0, (digits[idx][4] & 0x02) ? 1 : 0, (digits[idx][4] & 0x01) ? 1 : 0);
  ledMatris.dotrow6((digits[idx][5] & 0x10) ? 1 : 0, (digits[idx][5] & 0x08) ? 1 : 0, (digits[idx][5] & 0x04) ? 1 : 0, (digits[idx][5] & 0x02) ? 1 : 0, (digits[idx][5] & 0x01) ? 1 : 0);
  ledMatris.dotrow7((digits[idx][6] & 0x10) ? 1 : 0, (digits[idx][6] & 0x08) ? 1 : 0, (digits[idx][6] & 0x04) ? 1 : 0, (digits[idx][6] & 0x02) ? 1 : 0, (digits[idx][6] & 0x01) ? 1 : 0);
}

void clearDotMatrix(DotMatrix &ledMatris) {
  ledMatris.dotrow1(0,0,0,0,0);
  ledMatris.dotrow2(0,0,0,0,0);
  ledMatris.dotrow3(0,0,0,0,0);
  ledMatris.dotrow4(0,0,0,0,0);
  ledMatris.dotrow5(0,0,0,0,0);
  ledMatris.dotrow6(0,0,0,0,0);
  ledMatris.dotrow7(0,0,0,0,0);
}


void setup() {
  LEDMatris1.begin(0x0A);
  LEDMatris2.begin(0x4A);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  randomSeed(analogRead(0));

  clearDotMatrix(LEDMatris1);
  clearDotMatrix(LEDMatris2);
}

void loop() {
   bool reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonReading) {
    buttonPressed = reading;

    if (buttonPressed == LOW) {  
      currentNumber1 = random(1, 7);
      currentNumber2 = random(1, 7);
    }
  }
  lastButtonReading = reading;

  if (currentNumber1 >= 1 && currentNumber1 <= 6) {
    displayNumberOnDotMatrix(LEDMatris1, currentNumber1);
  } else {
    clearDotMatrix(LEDMatris1);
  }

  if (currentNumber2 >= 1 && currentNumber2 <= 6) {
    displayNumberOnDotMatrix(LEDMatris2, currentNumber2);
  } else {
    clearDotMatrix(LEDMatris2);
  }
}