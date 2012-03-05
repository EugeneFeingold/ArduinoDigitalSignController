#ifndef SignController_H
#define SignController_H


#include "Arduino.h"
#include <SoftwareSerial.h>

#include "ArrayUtils.h"
#include "SignOptions.h"

class SignController {
  
  public:
    SignController(int pinRX, int pinTX);
    SignController(int pinRX, int pinTX, boolean debugMode);
    
    void sendMessage(String message);
    void sendMessage(String message, SignOptions *signOptions);
    
  private:
  
    void buildMessageCommand(String message, SignOptions *signOptions, byte command[]) ;
    int getMessageCommandLength(String message);
    
    void buildAndWriteCommand(byte command[], int commandLength);
    void buildCommand(byte command[], int commandLength, byte builtCommand[]);
    void computeChecksum(byte command[], int commandLength, byte b[2]);
    
    
    
    void writeToSerial(byte arr[], int arrSize);
    
    
    void debugOutput(String header, byte arr[], int arrSize);
    
    SoftwareSerial softSerial;
    ArrayUtils arrayUtils;
    boolean _debugMode;
  
};


#endif
