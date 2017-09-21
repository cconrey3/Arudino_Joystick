/*
11/06/2015
This version of Joystick minimizes unnecessary matrix.writeScreen() calls via an if-statement that only allows 
clearing and writing a new screen if the new pixel position (~Pixel) is not equal to the reference pixel (~CurrentPixel).  
If not, the ~CurrentPixel variables are updated and the screen is written to.  
{Note, I'm going to start using "~VarName" to refer to both "xVarName" and "yVarName" together}
This version also has a built-in tolerance for ~Mapped of 2% (points [-10,10] within [-500,500]) so that if the Joystick is 
not moved significantly enough, ~Mapped is overwritten with 0 and no movement occurs at all. This fixes surprisingly well the drifting 
issue I've been running into in previous versions, and doesn't affect use too much. It is noticeable that there is a 
tolerance if the user moves very slowly, but in real use, moving that slowly shouldn't come up, and if it does, the tolerance levels and
~Sensitivity values can be adjusted to minimize it. Overall, I'm really happy with how well this version fixes major issues from before.
I think this code is good enough to start moving towards real games like Pong or Snake.

Previous Version: Joystick_calibrated
Changes from Previous Version:
-Fixed Flicker
-Inclusion of two new variables,
    yCurrentPixel
    xCurrentPixel 
-Fixed ghost drifting issue 
-Included commenting of significant or ambiguous code.
*/

#include "HT1632.h"
#define DATA 2
#define WR 3
#define CS 4
HT1632LEDMatrix matrix = HT1632LEDMatrix(DATA, WR, CS);
const int yPin = A0;
const int xPin = A1;
const int led = 13;  //Lights up during calibration
float yPosFloat = 8;   //starting x and y positions
float xPosFloat = 12; 
int yCurrentPixel = 8;  //starting pixel positions, for reference of change when yPosFloat and xPosFloat are updated and rounded.
int xCurrentPixel = 12;
boolean n = true;     //Initialize calibration, becomes false after calibration

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
  float xSensitivity = 0.0005;
  float ySensitivity = 0.0005;
  
  if(n){                             //Calibration. Runs first iteration of loop()
    xMeasure = calibrate(xPin);
    yMeasure = calibrate(yPin);
    n = false;
  }
  else if (!n){                      //Post-calibration.
    int yRaw = analogRead(yPin);
    int xRaw = analogRead(xPin);
  
    /*
    //For debugging, to read the inputs from Joystick.
    Serial.print("xMeasure: "); 
    Serial.print(xMeasure);
    Serial.print("yMeasure: ");
    Serial.println(yMeasure);
  
    Serial.print("yRaw: ");
    Serial.print(yRaw);
    Serial.print("xRaw: ");
    Serial.print(xRaw);
    */
  
    int yMapped = map(yRaw,0,2*yMeasure,500,-500);    //map joystick values (0 ~ 2 times the calibrated neutral value) to range (500 ~ -500) which is an arbitrary working range so that yMapped*ySensitivity has a reasonable change of position.
    int xMapped = map(xRaw,0,2*xMeasure,-500,500);
  
    if (abs(yMapped) <= 10){    //Built-in 2% tolerance, so there shouldn't be drifting if user is not actively moving joystick.
      yMapped = 0;
    }
    if (abs(xMapped) <= 10){
      xMapped = 0;
    }

    yPosFloat = yPosFloat + yMapped*ySensitivity;
    xPosFloat = xPosFloat + xMapped*xSensitivity;
    
    //For Debugging, to view current un-rounded y and x position float values.
    /*
    Serial.print("yPosFloat: ");
    Serial.print(yPosFloat);
    Serial.print("xPosFloat: ");
    Serial.println(xPosFloat);
    */
    
    yPosFloat = constrain(yPosFloat,0,15);
    xPosFloat = constrain(xPosFloat,0,23);
    
    int yPixel = yPosFloat;
    int xPixel = xPosFloat;
    
    if  ((yPixel != yCurrentPixel) | (xPixel != xCurrentPixel)) {    //Only updates LED matrix if there's a change in the pixel position
      matrix.clearScreen();
      matrix.setPixel(xPixel,yPixel);
      matrix.writeScreen();
      yCurrentPixel = yPixel;
      xCurrentPixel = xPixel;
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
