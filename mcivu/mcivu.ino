
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

String getCommand(keyboardReading) {
  if (keyboardReading > 140 && keyboardReading < 145) {
    return 'CMD_READY';
  } else if (keyboardReading > 507 && keyboardReading < 511) {
    return 'CMD_MODE';
  } else if (keyboardReading > 336 && keyboardReading < 340) {
    return 'CMD_DEPLOY';
  } else if (keyboardReading > 250 && keyboardReading < 255) {
    return 'CMD_DEPLOY_TO';
  } else if (keyboardReading > 199 && keyboardReading < 204) {
    return 'CMD_BACK';
  } else if (keyboardReading > 166 && keyboardReading < 170) {
    return 'CMD_FORWARD';
  }
}
