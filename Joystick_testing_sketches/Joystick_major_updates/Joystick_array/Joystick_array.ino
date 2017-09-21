/*
11/10/2015
This version of Joystick is the first step towards a pong-like game. This version is changed a lot in that the two-dimensional movement of the joystick was removed, and now there is only a 
single shape that moves up and down. This version removes all code relating to xPositions or measures of an x-value. This version adds in a for loop at the point where the
pixel position and writeScreen are updated. Instead of writing a single pixel, the sktech now writes 4 pixels in a column based on yPixel. This code looks a lot less clean now
and is very raw at the moment, but with further updates, which will introduce a ball and bounce system based on the sketch bouncyBall, this should get cleaned up. The problem 
of drifting seems to be back now, which is unfortunate; I'm looking into better ways to fix that.

Previous Version: Joystick_fixFlicker
Changes from Previous Version:
-removal of x-related measures and variables
-addition of column-bar in place of singular pixel

*/

#include "HT1632.h"
#define DATA 2
#define WR 3
#define CS 4
HT1632LEDMatrix matrix = HT1632LEDMatrix(DATA, WR, CS);
const int yPin = A0;
const int led = 13;
float yPosFloat = 5;   //starting x and y positions
int yCurrentPixel = 5;  //starting pixel positions, for reference of change when yPosFloat and xPosFloat are updated and rounded.
int xPixel = 0;
boolean n = true;     //Initialize calibration, becomes false after calibration
int tolerance = 7;

void setup() {
  Serial.begin(9600);
  matrix.begin(HT1632_COMMON_16NMOS);
  pinMode(yPin,INPUT);
  matrix.clearScreen();
  delay(100);
}

void loop() { 
  static int yMeasure;
  float ySensitivity = 0.0005;
  
  if(n){                             //Calibration. Runs first iteration of loop()
    yMeasure = calibrate(yPin);
    n = false;
  }
  else if (!n){                      //Post-calibration.
    int yRaw = analogRead(yPin);
  
    /*
    //For debugging, to read the inputs from Joystick.
    Serial.print("yMeasure: ");
    Serial.println(yMeasure);
  
    Serial.print("yRaw: ");
    Serial.print(yRaw);
    */
  
    int yMapped = map(yRaw,0,2*yMeasure,500,-500);    //map joystick values (0 ~ 2 times the calibrated neutral value) to range (500 ~ -500) which is an arbitrary working range so that yMapped*ySensitivity has a reasonable change of position.
  
    if (abs(yMapped) <= tolerance){    //Built-in 2% tolerance, so there shouldn't be drifting if user is not actively moving joystick.
      yMapped = 0;
    }

    yPosFloat = yPosFloat + yMapped*ySensitivity;
    
    //For Debugging, to view current un-rounded y and x position float values.
    /*
    Serial.print("yPosFloat: ");
    Serial.println(yPosFloat);
    */
    
    yPosFloat = constrain(yPosFloat,0,12);
    
    int yPixel = yPosFloat;
    
    if  (yPixel != yCurrentPixel){    //Only updates LED matrix if there's a change in the pixel position
      matrix.clearScreen();
      
      for(int i = yPixel; i < yPixel + 4;i++){ //New for loop to set 4 pixel positions for the bar.
      matrix.setPixel(xPixel,i);
      }
      matrix.writeScreen();
      yCurrentPixel = yPixel;
    }
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
