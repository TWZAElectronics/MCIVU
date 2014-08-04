#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int keyboardReadPin = 0;
int keyboardReading = 0;

int numberOfPipelines = 5;
char DELIMETER = ':';

void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);
}


void loop() {
  keyboardReading = analogRead(keyboardReadPin);
  Serial.println(getKeyboardReading(keyboardReading));
  delay(1000);
}


String getKeyboardReading(int keyboardReading) {
  if (keyboardReading > 140 && keyboardReading < 145) {
    return "CMD_READY";
  } else if (keyboardReading > 507 && keyboardReading < 511) {
    return "CMD_MODE";
  } else if (keyboardReading > 336 && keyboardReading < 340) {
    return "CMD_DEPLOY";
  } else if (keyboardReading > 250 && keyboardReading < 255) {
    return "CMD_DEPLOY_TO";
  } else if (keyboardReading > 199 && keyboardReading < 204) {
    return "CMD_BACK";
  } else if (keyboardReading > 166 && keyboardReading < 170) {
    return "CMD_FORWARD";
  }
}


void serialEvent() {
  String data =(String) Serial.read();
  processCommand(data);
  delay(1000);
}


// DSP
void processCommand(String inputCommand) {
  if (inputCommand.startsWith("DSP")) {
    processDisplayCommand(inputCommand.substring(3));
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
        } else if (displaySubCommand.startsWith("SCR")) {
          scroll = getScroll(displaySubCommand);
          if (scroll == -1)
            return;
        } else if (displaySubCommand.startsWith("CMX")) {
          colourCommand = displaySubCommand;
        }

        displayCommand = displayCommand.substring(delimeterPosition+1, displayCommand.length());
    }
  } while(delimeterPosition >=0);

  displayText(displaySubCommand, lineNumber -1, scroll);
  setColour(colourCommand);
}


int getLineNumber(String lineNumberCommand) {
  if (lineNumberCommand.startsWith("LN")) {
    char lineNumber = lineNumberCommand.charAt(2);

    if (!isAlpha(lineNumber)) {
      return (int)lineNumber;
    }
  }

  return -1;
}


int getScroll(String scrollCommand) {
  if (scrollCommand.startsWith("SCR")) {
    char scrollValue = scrollCommand.charAt(2);

    if (!isAlpha(scrollValue)) {
      return (int)scrollValue;
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
    } else {
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

  if(scroll == 0) {
    lcd.print(text);
  } else {
    for (int positionCounter = 0; positionCounter < text.length(); positionCounter++) {
      lcd.scrollDisplayLeft();
      delay(150);
    }
  }
}

