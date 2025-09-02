#include <Deneyap_OLED.h>
#include <Deneyap_KumandaKolu.h>

OLED ekran1;
Joystick kumanda1;
int numbers1[12];
bool visible1[12];
int selectedIndex1 = 0;

OLED ekran2;
Joystick kumanda2;
int numbers2[12];
bool visible2[12];
int selectedIndex2 = 0;

bool blinkState1 = true;
bool blinkState2 = true;
unsigned long lastBlinkTime1 = 0;
unsigned long lastBlinkTime2 = 0;
const int blinkInterval = 300;

int silinenSayac1 = 0;
int silinenSayac2 = 0;

bool gameEnded1 = false;
bool gameEnded2 = false;

unsigned long gameStartTime1 = 0;
unsigned long gameStartTime2 = 0;

unsigned long lastMoveTime1 = 0;
unsigned long lastMoveTime2 = 0;
const unsigned long moveDelay = 200; 

const int totalRows = 3;  
const int totalCols = 4;   

void setup() {
  Serial.begin(115200);

  ekran1.begin(0x7A);
  kumanda1.begin(0x1A);
  ekran2.begin(0x78);
  kumanda2.begin(0x1B);

  for (int i = 0; i < 12; i++) {
    numbers1[i] = i + 1;
    visible1[i] = true;
    numbers2[i] = i + 1;
    visible2[i] = true;
  }

  drawAllNumbers1();
  drawAllNumbers2();

  gameStartTime1 = millis();
  gameStartTime2 = millis();
}

void loop() {

  if (!gameEnded1) {
    handleJoystickAndDisplay(kumanda1, ekran1, numbers1, visible1, selectedIndex1, blinkState1, lastBlinkTime1, silinenSayac1, gameEnded1, gameStartTime1, 1, lastMoveTime1);
  }
  if (!gameEnded2) {
    handleJoystickAndDisplay(kumanda2, ekran2, numbers2, visible2, selectedIndex2, blinkState2, lastBlinkTime2, silinenSayac2, gameEnded2, gameStartTime2, 2, lastMoveTime2);
  }
}

void handleJoystickAndDisplay(Joystick &joystick, OLED &oled, int numbers[], bool visible[], int &selectedIndex,
                              bool &blinkState, unsigned long &lastBlinkTime,
                              int &silinenSayac, bool &gameEnded, unsigned long &gameStartTime, int playerNum,
                              unsigned long &lastMoveTime) {
  int xVal = joystick.xRead();
  int yVal = joystick.yRead();
  bool buton = joystick.swRead();

  int dx = 0, dy = 0;

  if (xVal > 700) dx = 1;
  else if (xVal < 300) dx = -1;

  if (yVal > 700) dy = -1;
  else if (yVal < 300) dy = 1;

  if (millis() - lastMoveTime > moveDelay) {
    int oldIndex = selectedIndex;

    if (dx == 1 && (selectedIndex % totalCols) < (totalCols - 1)) selectedIndex++;
    else if (dx == -1 && (selectedIndex % totalCols) > 0) selectedIndex--;

    if (dy == 1 && selectedIndex + totalCols <= (totalCols * totalRows - 1)) selectedIndex += totalCols;
    else if (dy == -1 && selectedIndex - totalCols >= 0) selectedIndex -= totalCols;

    if (oldIndex != selectedIndex) {
      drawNumberAt(oled, numbers, visible, oldIndex, true);
      blinkState = true;
      drawNumberAt(oled, numbers, visible, selectedIndex, blinkState);
      lastMoveTime = millis();
    }
  }

  if (buton && visible[selectedIndex]) {
    visible[selectedIndex] = false;
    drawNumberAt(oled, numbers, visible, selectedIndex, true);

    silinenSayac++;
    if (silinenSayac >= totalCols * totalRows) {
      gameEnded = true;
      unsigned long elapsedMillis = millis() - gameStartTime;
      handleGameEnd(elapsedMillis, playerNum);
    }
  }

  if (millis() - lastBlinkTime > blinkInterval) {
    blinkState = !blinkState;
    lastBlinkTime = millis();
    drawNumberAt(oled, numbers, visible, selectedIndex, blinkState);
  }
}

void drawNumberAt(OLED &oled, int numbers[], bool visible[], int index, bool show) {
  int numbersPerRow = totalCols; 
  int numbersPerCol = totalRows; 

  int charWidth = 3; 
  int startCol = 2;
  int startRow = 2;

  int row = index / totalCols;
  int col = index % totalCols;

  int oledRow = startRow + row * 2;

  oled.setTextXY(oledRow, startCol + col * charWidth);

  if (visible[index] && show) {
    char buf[4];
    sprintf(buf, "%2d ", numbers[index]);
    oled.putString(buf);
  }
  else if (!visible[index]) {
    if (show) {
      oled.putString(".. ");
    } else {
      oled.putString("   ");
    }
  }
  else {
    oled.putString("   ");
  }
}

void handleGameEnd(unsigned long elapsedMillis, int winningPlayer) {
  int totalSeconds = elapsedMillis / 1000;
  int minutes = totalSeconds / 60;
  int seconds = totalSeconds % 60;

  if (winningPlayer == 1) {
    displayWinMessage(ekran1, minutes, seconds);
    ekran2.clearDisplay();
  } else if (winningPlayer == 2) {
    displayWinMessage(ekran2, minutes, seconds);
    ekran1.clearDisplay();
  }

  delay(5000);

  ekran1.clearDisplay();
  ekran2.clearDisplay();

  resetGame(numbers1, visible1, selectedIndex1, silinenSayac1, ekran1, gameEnded1);
  resetGame(numbers2, visible2, selectedIndex2, silinenSayac2, ekran2, gameEnded2);

  gameStartTime1 = millis();
  gameStartTime2 = millis();
}

void resetGame(int numbers[], bool visible[], int &selectedIndex, int &silinenSayac, OLED &oled, bool &gameEnded) {
  for (int i = 0; i < 12; i++) {
    visible[i] = true;
  }
  silinenSayac = 0;
  selectedIndex = 0;
  gameEnded = false;
  oled.clearDisplay();
  for (int i = 0; i < 12; i++) {
    drawNumberAt(oled, numbers, visible, i, true);
  }
}

void drawAllNumbers1() {
  ekran1.clearDisplay();
  for (int i = 0; i < totalCols * totalRows; i++) {
    drawNumberAt(ekran1, numbers1, visible1, i, true);
  }
}

void drawAllNumbers2() {
  ekran2.clearDisplay();
  for (int i = 0; i < totalCols * totalRows; i++) {
    drawNumberAt(ekran2, numbers2, visible2, i, true);
  }
}

void displayWinMessage(OLED &oled, int minutes, int seconds) {
  oled.clearDisplay();
  oled.setTextXY(2, 5);
  oled.putString("TEBRIKLER");
  oled.setTextXY(3, 4);
  oled.putString("KAZANDINIZ!");

  oled.setTextXY(5, 4);
  char buf[20];
  sprintf(buf, "Sure: %02d:%02d", minutes, seconds);
  oled.putString(buf);
}