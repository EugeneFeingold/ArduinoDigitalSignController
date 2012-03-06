#include <SoftwareSerial.h>

#include "SignController.h"
#include "SignOptions.h"

//SoftwareSerial mySerial(2,3);


int counter = 0;

const int messageSize = 200;
char message[messageSize];

SignController signController = SignController(2, 3, true);
SignOptions options = SignOptions();

void setup() {
  Serial.begin(57600);

  clearMessage();

  options.scrollMode = ScrollModes::MOVE_TO_LEFT_LOOP;
  options.scrollRate = 0x58;
  options.scrollFreeze = 0;
  
  signController.sendMessage("", &options);

}


void loop() {

  byte b;
  
  if (Serial.available() > 0) {
    b = Serial.read();
    
    Serial.println(counter);
    if (b == ';') {
      message[counter] = ' ';
      signController.sendMessage(message, &options);
      counter = 0;
      
      Serial.println(message);
      
      clearMessage();
      
    } else {
      message[counter++] = b;
    }
  }
  
}


void clearMessage() {
  for (int i = 0; i < messageSize; i++)
     message[i] = 0x00;
}




