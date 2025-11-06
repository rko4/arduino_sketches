const unsigned blueLedPin = 10;
const unsigned greenLedPin = 9;
const unsigned redLedPin = 11;

void setup() {
  Serial.begin(9600);
  
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);
}

void loop() {
  int redLevel = analogRead(A0) / 4;
  int greenLevel = analogRead(A2) / 4;
  int blueLevel = analogRead(A5) / 4;

  analogWrite(redLedPin, redLevel);
  analogWrite(greenLedPin, greenLevel);
  analogWrite(blueLedPin, blueLevel);
}
