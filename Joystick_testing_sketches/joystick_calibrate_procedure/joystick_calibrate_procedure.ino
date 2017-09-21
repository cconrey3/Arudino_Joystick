const int yPin = A0;
const int xPin = A1;

int xMeasure = calibrate(xPin);
int yMeasure = calibrate(yPin);
  
void setup() {
  Serial.begin(9600); 
}

void loop() {

  Serial.println(xMeasure);
  Serial.println(yMeasure);

}

int calibrate(int pin) {
  
  const int testNum = 25;
  int measure = 0;
  int i = 0;
  
  pinMode(pin,INPUT);  
  do{
    measure += analogRead(pin); 
    i++;  
    //delay(100); 
  }while (i < testNum);
  measure = measure/testNum;
  return measure;  
}
