
int keyboardReadPin = 0;
int keyboardReading = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  keyboardReading = analogRead(keyboardReadPin);
  Serial.println(keyboardReading);
  delay(1000);
}
