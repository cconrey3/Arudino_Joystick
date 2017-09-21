#include "HT1632.h"

#define DATA 2
#define WR 3
#define CS 4
#define t 50

HT1632LEDMatrix matrix = HT1632LEDMatrix(DATA, WR, CS);

int UD = A0;
int LR = A1;
  
void setup() {
  Serial.begin(9600);
  matrix.begin(HT1632_COMMON_16NMOS);
  
  pinMode(UD,INPUT);
  pinMode(LR,INPUT);
  
  delay(100);
  matrix.clearScreen();
  delay(1000);
}

void loop() {
  
  //Original Sketch
  matrix.clearScreen();
  int UDLevel = analogRead(UD);
  int LRLevel = analogRead(LR);
  
  int UDPixel = map(UDLevel,1023,0,0,15);
  int LRPixel = map(LRLevel, 0,1023,0,23);
  
  matrix.setPixel(LRPixel,UDPixel);
  matrix.writeScreen();
  
  /* 11/10/2015
  int LRPixel = 1;
  int UDPixel[5] = {3, 4, 5, 6, 7};
  
  for(int i = 0;i < 4;i++){
  matrix.setPixel(LRPixel,UDPixel[i]);
  }
  matrix.writeScreen();
  */
}
