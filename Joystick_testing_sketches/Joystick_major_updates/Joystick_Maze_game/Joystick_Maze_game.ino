/*
9/27/2017
This version of Joystick made minor changes for comprehension, such as variable name choice and true/false choice for booleans. 
Other major changes includeintroduceing a vertical wall and code in the loop method that checks to see if the cursor clips the wall; 
if it does, the screen blinks, then resets, using a new reset method called "death()"


Previous Version: Joystick_fixFlicker
Changes from Previous Version:
-New Variables calibrated (replacing n), clipping, xClipping (currently unused, but there for later use), yWall, xWall (for later use)
-improved clarity
-new method, "death()"

  SETUP:
  -LED Matrix has 4 required wires for single LED matrix use: VCC+5(RED, pin 1), GND(BLK, pin 3), DATA(BLUE, pin 4), Chip Select(WHITE, pin 3), READ/WRITE(YELLOW, pin7)
  -PINS on LED Matrix:
  (red strip at top)
  1  10
  2  9
  3  8
  4  7
  5  6
*/

#include "HT1632.h"
#define DATA 2
#define WR 3
#define CS 4
HT1632LEDMatrix matrix = HT1632LEDMatrix(DATA, WR, CS);
const int yPin = A0;
const int xPin = A1;
const int led = 13;  //Lights up during calibration
float yPosFloat = 0;   //starting x and y positions
float xPosFloat = 0; 
int yCurrentPixel = 0;  //starting pixel positions, for reference of change when yPosFloat and xPosFloat are updated and rounded.
int xCurrentPixel = 0;
int yPixel;
int xPixel;
const int yWall_one[] = {0,1,3,4,5,6,7,8};
const int xWall_one = 3;
const int yWall_two[] = {3,4,5,6,7,8,9,10,11,12,13,14,15};
const int xWall_two = 8;
const int yWall_three = 3;
const int xWall_three[] = {9,10,11,12,13,14,15,16,17,18};
const int yWall_four[] = {4,5,6,7,8,9,10,11,12};
const int xWall_four = 18;
const int yWall_five[] = {7,8,9,10,11,12,13,14,15};
const int xWall_five = 12;
const int yFinishLine = 14;
const int xFinishLine[] = {9,10,11};

boolean clipping = false;
boolean success = false;
boolean xClipping = false;
boolean calibrated = false;     //Initialize calibration, becomes true after calibration

void setup() {
  Serial.begin(9600);
  matrix.begin(HT1632_COMMON_16NMOS);
  pinMode(yPin,INPUT);
  pinMode(xPin, INPUT);
  
  
  /*
  use to check if Matrix is connected properly
  matrix.fillScreen();
  */
  /*
  for(int i = 0; i < (sizeof(yWall_two)/sizeof(int)); i++){
  Serial.print(yWall_two[i]);
  Serial.print("\n");
  }
  */
  
  delay(100);
  matrix.clearScreen();
  matrix.setPixel(xCurrentPixel,yCurrentPixel);
  for(int i = 0; i < (sizeof(yWall_one)/sizeof(int)); i++){
   matrix.setPixel(xWall_one,yWall_one[i]); 
  }
  for(int i = 0; i < (sizeof(yWall_two)/sizeof(int)); i++){
   matrix.setPixel(xWall_two,yWall_two[i]); 
  }
  for(int i = 0; i < (sizeof(xWall_three)/sizeof(int)); i++){
   matrix.setPixel(xWall_three[i],yWall_three); 
  }
  for(int i = 0; i < (sizeof(yWall_four)/sizeof(int)); i++){
   matrix.setPixel(xWall_four,yWall_four[i]); 
  }
    for(int i = 0; i < (sizeof(yWall_five)/sizeof(int)); i++){
   matrix.setPixel(xWall_five,yWall_five[i]); 
  }
  for(int i = 0; i < (sizeof(xFinishLine)/sizeof(int)); i++){
   matrix.setPixel(xFinishLine[i],yFinishLine); 
  }
  matrix.writeScreen();
}

void loop() { 
  static int xMeasure;
  static int yMeasure;
  float xSensitivity = 0.0003;
  float ySensitivity = 0.0003;
  
  if(!calibrated){                            //Calibration. Runs first iteration of loop()
    xMeasure = calibrate(xPin);
    yMeasure = calibrate(yPin);
    calibrated = true;
  }
  else if (calibrated){                      //Post-calibration.
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
    
    yPixel = yPosFloat;
    xPixel = xPosFloat;
    
    if  ((yPixel != yCurrentPixel) | (xPixel != xCurrentPixel)) {    //Only updates LED matrix if there's a change in the pixel position
      for(int i = 0; i < (sizeof(yWall_one)/sizeof(int)); i++){
        if ((yPixel == yWall_one[i]) & (xPixel == xWall_one)){
          clipping = true;
        }
      }
      for(int i = 0; i < (sizeof(yWall_two)/sizeof(int)); i++){
        if ((yPixel == yWall_two[i]) & (xPixel == xWall_two)){
          clipping = true;
        }
      }
      for(int i = 0; i < (sizeof(xWall_three)/sizeof(int)); i++){
        if ((yPixel == yWall_three) & (xPixel == xWall_three[i])){
          clipping = true;
        }
      }
      for(int i = 0; i < (sizeof(yWall_four)/sizeof(int)); i++){
        if ((yPixel == yWall_four[i]) & (xPixel == xWall_four)){
          clipping = true;
        }
      }
      for(int i = 0; i < (sizeof(yWall_five)/sizeof(int)); i++){
        if ((yPixel == yWall_five[i]) & (xPixel == xWall_five)){
          clipping = true;
        }
      }
      for(int i = 0; i < (sizeof(xFinishLine)/sizeof(int)); i++){
        if ((yPixel == yFinishLine) & (xPixel == xFinishLine[i])){
          success = true;
        }
      }
      if(clipping){
      death();
      }
      else if(success){
       gloriousDeath(); 
      }
      else{
      matrix.clrPixel(xCurrentPixel,yCurrentPixel);
      matrix.setPixel(xPixel,yPixel);
      matrix.writeScreen();
      yCurrentPixel = yPixel;
      xCurrentPixel = xPixel;
    }
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

void death() {
  matrix.blink(true);
  delay(2000);
  matrix.blink(false);
  matrix.clrPixel(xCurrentPixel,yCurrentPixel);
  yPosFloat = 0;
  xPosFloat = 0; 
  yCurrentPixel = 0;
  xCurrentPixel = 0;
  yPixel = 0;
  xPixel = 0;
  matrix.setPixel(xCurrentPixel,yCurrentPixel);
  matrix.writeScreen();
  clipping = false;
}

void gloriousDeath() {
  matrix.fillScreen();
  matrix.blink(true);
  delay(2000);
  matrix.blink(false);
  matrix.clearScreen();
  
  matrix.setTextSize(1);
  matrix.setTextColor(1);
  
  matrix.setCursor(3,0);
  matrix.print("YOU");
  matrix.setCursor(3,8);
  matrix.print("WIN");
  matrix.writeScreen();
  delay(1000);
  
  success = false;
  
 death(); 
}

