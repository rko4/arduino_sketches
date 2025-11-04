const unsigned blueLedPin = 10;
const unsigned greenLedPin = 9;
const unsigned redLedPin = 11;
// DEBUG
//bool light = true;

void setup() {
  // put your setup code here, to run once:
//  Serial.begin(9600);
  
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);
//  pinMode(13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(redLedPin, 0);
  analogWrite(greenLedPin, 0);
  analogWrite(blueLedPin, 16);

  // DEBUG
  /*
  delay(500);
  
  digitalWrite(13, light);
  light = !light;
  */
//  Serial.println(light);
}
