/*
08/31/2015
This version of Joystick includes a calibration function that measures the neutral position of the joystick and uses the neutral x and y value as
the base position for reference when yRaw and xRaw are calculated. This is intended to limit the amount of drift noticed in earlier versions of Joystick.
This calibration works pretty well, but there is still some drift at times. I've also noticed flickering in the LED pretty consistently, but about as much as
I noticed in Joystick_velocity, the version before Joystick_calibrated.

Previous Version: Joystick_velocity
Changes from previous version:
-included int calibrate(int pin) procedure
-included three important new variables,
  static int xMeasure
  static int yMeasure
  boolean n
  These are related to calibrate(pin). xMeasure and yMeasure are the readings of the joystick at a neutral position at the beginning of the function
  They are used in an edited form of yMapped and xMapped that now map from (0,2*xMeasure) or (0,2*yMeasure) instead of (0,1000). This means that a reading 
  of the new neutral/reference position will map x/yMapped to 0 movement. boolean n is used to distinguish the first iteration of the loop, where x and y 
  are calibrated, and every other iteration, where the joystick program is run.
-variable cleanup and inclusion of Serial Monitor debugging lines

*/

#include "HT1632.h"

#define DATA 2
#define WR 3
#define CS 4

HT1632LEDMatrix matrix = HT1632LEDMatrix(DATA, WR, CS);

const int yPin = A0;
const int xPin = A1;
const int led = 13; //Lights up when undergoing calibration.
float yPosFloat = 8;
float xPosFloat = 12; 
boolean n = true;  

void setup() {
  Serial.begin(9600);
  
  matrix.begin(HT1632_COMMON_16NMOS);
  
  pinMode(yPin,INPUT);
  pinMode(xPin, INPUT);
  
  matrix.clearScreen();
  delay(100);
}

  
void loop() { 
  static int xMeasure;
  static int yMeasure;
  float xSensitivity = 0.001;
  float ySensitivity = 0.001;
  
  if(n){
    xMeasure = calibrate(xPin);
    yMeasure = calibrate(yPin);
    n = false;
  }
  else if (!n){
  matrix.clearScreen();
  
  int yRaw = analogRead(yPin);
  int xRaw = analogRead(xPin);
  
  //Serial.print("xMeasure: ");
  //Serial.print(xMeasure);
  //Serial.print("yMeasure: ");
  //Serial.println(yMeasure);
  
  //Serial.print("yRaw: ");
  //Serial.print(yRaw);
  //Serial.print("xRaw: ");
  //Serial.print(xRaw);

  
  int yMapped = map(yRaw,0,2*yMeasure,500,-500);
  int xMapped = map(xRaw,0,2*xMeasure,-500,500);

  yPosFloat = yPosFloat + yMapped*ySensitivity;
  xPosFloat = xPosFloat + xMapped*xSensitivity;
  //Serial.print("yPosFloat: ");
  //Serial.print(yPosFloat);
  //Serial.print("xPosFloat: ");
  //Serial.println(xPosFloat);
  
  yPosFloat = constrain(yPosFloat,0,15);
  xPosFloat = constrain(xPosFloat,0,23);
  
  int yPixel = yPosFloat;
  int xPixel = xPosFloat;
  
  matrix.setPixel(xPixel,yPixel);
  matrix.writeScreen();
  }
}

int calibrate(int pin) {  
  const int testNum = 25;
  int measure = 0;
  
  digitalWrite(led,HIGH);
  
  for(int count = 0;count<testNum;count++){
    measure += analogRead(pin);  
  }
  measure = measure/testNum;
  digitalWrite(led,LOW);
  return measure;  
}
