  int UD1 = A0;
  int UD2 = A1;


void setup() {
  
  Serial.begin(9600);

  pinMode(UD1,INPUT);
  pinMode(UD2,INPUT);
}

void loop() {
  Serial.println(analogRead(UD1));
  //Serial.println(analogRead(UD2));
}
