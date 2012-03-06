#include <SoftwareSerial.h>

#include "SignController.h"
#include "SignOptions.h"


const int inputBufferSize = 200;

char inputBuffer[inputBufferSize];
int counter = 0;

SignController signController = SignController(2, 3, true);
SignOptions options = SignOptions();

void setup() {
  Serial.begin(57600);

  clearBuffer();

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
    if (b == '`') {
      inputBuffer[counter] = ' '; //terminator
      processBuffer();
      clearBuffer();
      counter = 0;
    } 
    else {
      inputBuffer[counter++] = b;
    }
  }
}


void processBuffer() {
  String command = String(inputBuffer).substring(0, 4);
  String message = String(inputBuffer).substring(5);

  if (command == "MSG ")     //set message
    return signController.sendMessage(inputBuffer, &options);
  if (command == "MOSM")     //message option scroll mode
    return (void)(options.scrollMode = message[0]);
  if (command == "MOSR")     //message option scroll rate
    return (void)(options.scrollRate = 0x58);  //TODO: convert from char[] (hex) to number
  
  Serial.println("Error: Unknown command.");


}


void clearBuffer() {
  for (int i = 0; i < inputBufferSize; i++)
    inputBuffer[i] = 0x00;
}



/*

 MSG messagetext
 
 
 */


