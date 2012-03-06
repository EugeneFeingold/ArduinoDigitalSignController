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

  initLights();
  clearBuffer();

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

  if (command == "LGHT")     //light control
    return setLightState(message[0], (message[1] == '1'));
  if (command == "MSG ")     //set message
    return signController.sendMessage(inputBuffer, &options);
  if (command == "MOSM")     //message option scroll mode
    return options.setScrollMode(message[0]);
  if (command == "MOSR")     //message option scroll rate
    return options.setScrollRate(0x58);            //TODO: convert from char[] (hex) to number
  if (command == "MOSF")     //message option scroll freeze
    return options.setScrollFreeze(message[0]);    //TODO: convert from char[] (up to three digits to dec)
  if (command == "MOBM")     //message option morder mode
    return options.setBorderMode(message[0]);
    
  Serial.println("Error: Unknown command.");
}


void clearBuffer() {
  for (int i = 0; i < inputBufferSize; i++)
    inputBuffer[i] = 0x00;
}


void initLights() {
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
 
  setPinState(6, false);
  setPinState(7, false);
  setPinState(8, false); 
}



void setLightState(char light, boolean state) {
  Serial.println("Setting light " + String(light) + " to " + String(state));
  
  if (light == 'r')
    return setPinState(6, state);
  if (light == 'y') 
    return setPinState(7, state);    
  if (light == 'b') 
    return setPinState(8, state);
}


void setPinState(int pin, boolean state) {
  if (state) 
    digitalWrite(pin, LOW);
  else
    digitalWrite(pin, HIGH);
}
