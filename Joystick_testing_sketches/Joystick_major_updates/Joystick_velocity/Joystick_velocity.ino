/* This sketch uses a Parallax brand two-pot joystick ranging values from 0-1000, and an Adafruit HT1632 Led Matrix.

  This sketch uses the joystick to control the velocity of a pixel moving up and down the left border of the matrix/screen.
  
  Unlike LEDMatrix_Playtesting, which uses the joystick values to directly update the position (all the way up means the pixel is necessarily at the top of the screen,
  all the way down means the pixel is at the bottom of screen, and neutral means center of screen), now the joystick changes the relative motion of the pixel,
  that is, all up means full speed upward, regardless of position on the screen; all down means full speed downward; and neutral
  means the pixel stays in it's same position, whether at the top, bottom, or anywhere inbetwen.
*/

#include "HT1632.h"

#define DATA 2
#define WR 3
#define CS 4
#define t 50

HT1632LEDMatrix matrix = HT1632LEDMatrix(DATA, WR, CS);

const int yPin = A0;
const int xPin = A1;
float yPosFloat = 8;
float xPosFloat = 12; 

void setup() {
  Serial.begin(9600);
  matrix.begin(HT1632_COMMON_16NMOS);
  
  pinMode(yPin,INPUT);
  pinMode(xPin, INPUT);
  
  matrix.clearScreen();
  delay(100);
}

void loop() {
  matrix.clearScreen();
  
  int yRaw = analogRead(yPin);
  Serial.println(yRaw);
  int xRaw = analogRead(xPin);
  int yMapped = map(yRaw,-1,1000,500,-500);
  int xMapped = map(xRaw,0,1000,-500,500);

  yPosFloat = yPosFloat + yMapped*0.001;
  xPosFloat = xPosFloat + xMapped*0.001;
  yPosFloat = constrain(yPosFloat,0,15);
  xPosFloat = constrain(xPosFloat,0,23);
  
  int yPixel = yPosFloat;
  int xPixel = xPosFloat;
  
  matrix.setPixel(xPixel,yPixel);
  matrix.writeScreen();
}
