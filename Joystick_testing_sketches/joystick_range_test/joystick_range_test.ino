const int yPin = A0;
const int xPin = A1;
void setup() {
  Serial.begin(9600);
    
  pinMode(yPin,INPUT);
  pinMode(xPin, INPUT);
}

void loop() {
  int x = analogRead(xPin);
  int y = analogRead(yPin);
  Serial.print(x);
  Serial.print("   ");
  Serial.println(y);

}
