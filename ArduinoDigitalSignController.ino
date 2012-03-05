#include <SoftwareSerial.h>

#include "SignController.h"
#include "SignOptions.h"

//SoftwareSerial mySerial(2,3);


int counter = 0;
SignController signController = SignController(2, 3);
SignOptions options = SignOptions();

void setup() {
  Serial.begin(57600);
  
  
  options.scrollMode = ScrollModes::STATIC;
  options.scrollRate = 0x48;
  options.scrollFreeze = 0;
  
  //signController.sendMessage("Your mom", &options);

}




void loop() {
  
  signController.sendMessage(String(counter++));
  
  //signController.sendMessage(String(random(100000000)));
  
  //delay(1000);
  
  
}




