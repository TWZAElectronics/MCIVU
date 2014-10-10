#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int keyboardReadPin = 0;
char *keyboardReading;
int backlightPin = 13;

int numberOfPipelines = 5;
char DELIMETER = ':';

char *inData;

void setup() {
  lcd.begin(16, 2);
  pinMode(backlightPin, OUTPUT);
  digitalWrite(backlightPin, HIGH);
  Serial.begin(9600);
}

char *readSerialPort() {
  char inData[100];
  char inChar;
  byte index = 0;

  while(Serial.available() > 0) {
    if(index < 99) {
      inChar = Serial.read();
      inData[index] = inChar;
      index++;
      inData[index] = '\0';
    }
  }

  return inData;
}


char *getKeyboardReading(int keyboardReading) {
  if (keyboardReading > 140 && keyboardReading < 145) {
    return "CMD_READY";
  }
  else if (keyboardReading > 507 && keyboardReading < 511) {
    return "CMD_MODE";
  }
  else if (keyboardReading > 336 && keyboardReading < 340) {
    return "CMD_DEPLOY";
  }
  else if (keyboardReading > 250 && keyboardReading < 255) {
    return "CMD_DEPLOY_TO";
  }
  else if (keyboardReading > 199 && keyboardReading < 204) {
    return "CMD_BACK";
  }
  else if (keyboardReading > 166 && keyboardReading < 170) {
    return "CMD_FORWARD";
  }
  else {
    return "UNKNOWN";
  }
}


void sendCommand(String command) {
  Serial.println(command);
  delay(1500);
}

//void serialEvent() {
//  String data =(String) Serial.read();
//  processCommand(data);
//  delay(1000);
//}


// DSP
void processCommand(String inputCommand) {
  if (inputCommand.startsWith("DSP")) {
    processDisplayCommand(inputCommand.substring(4));
  }
}


// LN(1|2):SCR(0|1):CMX[A]|([R|G|B])*:TEXT
// Processing of display message stops immediately a protocol breach is detected
void processDisplayCommand(String displayCommand) {
  int lineNumber;
  int scroll;
  String displaySubCommand;
  String colourCommand;
  int delimeterPosition = -1;

  do {
    delimeterPosition = displayCommand.indexOf(DELIMETER);

    if (delimeterPosition != -1) {
      displaySubCommand = displayCommand.substring(0, delimeterPosition);

      if (displaySubCommand.startsWith("LN")) {
        lineNumber = getLineNumber(displaySubCommand);
        if (lineNumber == -1)
          return;
      }
      else if (displaySubCommand.startsWith("SCR")) {
        scroll = getScroll(displaySubCommand);
        if (scroll == -1)
          return;
      }
      else if (displaySubCommand.startsWith("CMX")) {
        colourCommand = displayCommand.substring(3, displayCommand.indexOf(DELIMETER));
      }

      displayCommand = displayCommand.substring(delimeterPosition+1, displayCommand.length());
    }
  }
  while(delimeterPosition >= 0);

  displayText(displayCommand, lineNumber -1, scroll);
  setColour(colourCommand);
}

int getLineNumber(String lineNumberCommand) {
  if (lineNumberCommand.startsWith("LN")) {
    char lineNumber = lineNumberCommand.charAt(2);

    if (!isAlpha(lineNumber)) {
      return ((int)lineNumber - 48);
    }
  }

  return -1;
}


int getScroll(String scrollCommand) {
  if (scrollCommand.startsWith("SCR")) {
    char scrollValue = scrollCommand.charAt(3);

    if (!isAlpha(scrollValue)) {
      return ((int)scrollValue - 48);
    }
  }

  return -1;
}


// colour value 'A' takes precedence over any other character in the matrix
void setColour(String colourMatrixCommand) {
  if (colourMatrixCommand.startsWith("CMX")) {
    String colourMatrix = colourMatrixCommand.substring(3);
    int colourMatrixLength = colourMatrix.length();

    lightsOut();

    if (colourMatrix.charAt(3) == 'A') {
      for (int i=0; i<numberOfPipelines; i+=1) {
        setColour('G', i);
      }
    }
    else {
      for (int i=0; i<colourMatrixLength; i+=1) {
        setColour(colourMatrix.charAt(i), i);
      }
    }
  }
}


// to be implemented
// sets the light at a given position to a given colour
void setColour(char colour, int position) {
  return;
}


// to be implemented
// its a dark dark place
void lightsOut() {
  return;
}


void displayText(String text, int lineNumber, int scroll) {
  lcd.setCursor(0, lineNumber);
  lcd.print(text);
  delay(2000);

  if(scroll == 1) {
    for (int positionCounter = 0; positionCounter < text.length(); positionCounter++) {
      lcd.scrollDisplayLeft();
      delay(500);
    }

    delay(1000);
  }

  lcd.setCursor(0, lineNumber);
  lcd.print(text);
}

void loop() {
  keyboardReading = getKeyboardReading(analogRead(keyboardReadPin));
  if(strcmp(keyboardReading, "CMD_READY") != 0 && strcmp(keyboardReading, "UNKNOWN") != 0) {
    sendCommand(keyboardReading);
    inData = readSerialPort();
    processCommand(inData);
  }
}
