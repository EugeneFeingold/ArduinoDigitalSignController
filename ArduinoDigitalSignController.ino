
#include <SPI.h>
#include <Ethernet.h>
#include <SoftwareSerial.h>
#include "SignController.h"
#include "SignOptions.h"

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network.
// gateway and subnet are optional:
byte mac[] = { 
  0x90, 0xA2, 0xDA, 0x00, 0x88, 0x01 };

IPAddress ip(10,1,1,181);
IPAddress gateway(10,1,1, 1);
IPAddress subnet(255, 255, 255, 0);
EthernetServer server(23);

const int inputBufferSize = 200;

char inputBuffer[inputBufferSize];
int counter = 0;
boolean receivingCommand = false;

SignController signController = SignController(2, 3, true);
SignOptions options = SignOptions();


void setup() {
  Serial.begin(57600);

  Ethernet.begin(mac, ip, gateway, subnet);
  // start listening for clients
  server.begin();

  initLights();
  clearBuffer();

  signController.sendMessage("", &options);
}



void loop() {
  byte b = NULL;

  EthernetClient client = server.available();

  if (Serial.available() > 0) {
    b = Serial.read();
  }

  if (client) {
    b = client.read();
  }

  if (b != NULL) {
    Serial.print(counter);
    Serial.println(" " + String(b, HEX));
    if (counter > 0 && b == '[' && inputBuffer[counter-1] == '[') {
      clearBuffer();
      counter = 0;
      receivingCommand = true;
      Serial.println("receiving command");
    } 
    else if (receivingCommand && counter > 0 && b == ']' && inputBuffer[counter-1] == ']') {
      receivingCommand = false;
      counter--;
      inputBuffer[counter] = ' '; //terminator
      processBuffer(counter);

      clearBuffer();
      counter = 0;
    } 
    else {
      inputBuffer[counter++] = b;
      if (counter > inputBufferSize) {
        clearBuffer();
        counter = 0;
      }
    }
  }
}



void processBuffer(int len) {
  String command = String(inputBuffer).substring(0, 4);
  String message = String(inputBuffer).substring(4);

  Serial.println("Command: " + command);
  Serial.println("Message: " + message);


  if (command == "LGHT")     //light control
    return setLightState(message[0], (message[1] == '1'));
  if (command == "MSG ")     //set message
    return signController.sendMessage(message, &options);
  if (command == "MOSM")     //message option scroll mode
    return options.setScrollMode(convertStringNumberToInt(message, 16));
  if (command == "MOSR")     //message option scroll rate  00 through FF
    return options.setScrollRate(convertStringNumberToInt(message, 16));
  if (command == "MOSF")     //message option scroll freeze
    return options.setScrollFreeze(convertStringNumberToInt(message, 10));   
  if (command == "MOBM")     //message option morder mode
    return options.setBorderMode(convertStringNumberToInt(message, 10));

  Serial.println("Error: Unknown command.");
}

int convertStringNumberToInt(String str, int base) {
  char * pEnd;
  return strtol(&(str[0]), &pEnd, base);
}



void clearBuffer() {
  Serial.println("clearing buffer");
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
  if (light == 'a') {
    setLightState('r', state);
    setLightState('y', state);
    setLightState('b', state);
  }
}


void setPinState(int pin, boolean state) {
  if (state) 
    digitalWrite(pin, LOW);
  else
    digitalWrite(pin, HIGH);
}




