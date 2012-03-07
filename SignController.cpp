
#include "SignController.h"


int MAX_MESSAGE_SIZE = 201; //actually 200

/*
command structure:  prefix - command - checksum - terminator
*/


byte prefix[] = {0xa0, 0x00, 0x00, 0x01}; //init, size byte 1, size byte 2, something?

byte terminator[] = {0x00, 0x50};

byte messageCommand[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x02, 0x01, 0x00, 
  0x01, //scroll mode 
  0x00, //border mode (0 none)
  0x3e, //first digit: rate level; second digit: rate (0 slowest f fastest)
  0x03, //freeze (sec)
  0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00 };

int msgScrollModePos = 14;
int msgBorderModePos = 15;
int msgScrollRatePos = 16;
int msgScrollFreezePos = 17;
                                                                                                             

byte finalizeCommand[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x19, 0x19, 0x18, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};


SignController::SignController(int pinRX, int pinTX) : softSerial(pinRX, pinTX) {
  _debugMode = false;
}


SignController::SignController(int pinRX, int pinTX, boolean debugMode) : softSerial(pinRX, pinTX) {
  _debugMode = debugMode;
}



void SignController::sendMessage(String message) {
  SignOptions signOptions = SignOptions();
  sendMessage(message, &signOptions);
}

void SignController::sendMessage(String message, SignOptions *signOptions) {

  if (message.length() > MAX_MESSAGE_SIZE) {
    if (_debugMode) {
      Serial.print("Message too big!  Received ");
      Serial.print(message.length());
      Serial.print(" bytes.  Max is ");
      Serial.println(MAX_MESSAGE_SIZE);
    }
    return;
  }
  
  softSerial.begin(57600);
  
  int messageCommandLength = getMessageCommandLength(message);
  byte command[messageCommandLength];
  buildMessageCommand(message, signOptions, command);
  buildAndWriteCommand(command, messageCommandLength);
  
  
  buildAndWriteCommand(finalizeCommand, sizeof(finalizeCommand));


  softSerial.end();
}


//builds message command.  command[] must be initialized to getMessageCommandLength(message) bytes
void SignController::buildMessageCommand(String message, SignOptions *signOptions, byte command[]) {
  int messageCommandLength = sizeof(messageCommand);
  int messageLength = message.length() + 1;
  
  debugOutput("messageCommand", messageCommand, messageCommandLength);
  arrayUtils.copyArray(messageCommand, messageCommandLength, command);
  
  command[msgScrollModePos] = signOptions->getScrollMode();
  command[msgScrollRatePos] = signOptions->getScrollRate();
  command[msgScrollFreezePos] = signOptions->getScrollFreeze();  
  command[msgBorderModePos] = signOptions->getBorderMode();
  
  byte messageBytes[messageLength];
  (message).getBytes(messageBytes, messageLength);
  
  byte messageDoubleBytes[messageLength * 2];
  arrayUtils.doubleBytes(messageBytes, messageLength, messageDoubleBytes);
  
  byte messageLengthBytes[4] = {0x00, 0x00, 0x00, 0x00};
  messageLengthBytes[0] = (messageLength - 1) * 2;
  
  debugOutput("messageLengthBytes", messageLengthBytes, 4);

  arrayUtils.addToArray(command, messageCommandLength, messageLengthBytes, 4);
  arrayUtils.addToArray(command, messageCommandLength + 4, messageDoubleBytes, messageLength * 2);
}


int SignController::getMessageCommandLength(String message) {
  int result = message.length() * 2; //message length
  result += sizeof(messageCommand);  //command prefix length
  result += 4;                       //message length specifier
  return result;
}


void SignController::buildAndWriteCommand(byte command[], int commandLength) {
  int builtCommandLength = getBuiltCommandLength(commandLength);
  byte builtCommand[builtCommandLength];
  buildCommand(command, commandLength, builtCommand);
  writeToSerial(builtCommand, builtCommandLength);
}



void SignController::buildCommand(byte command[], int commandLength, byte builtCommand[]) {
  //add prefix, checksum and terminator to command and return in builtCommand. 
  //builtCommand must be initialized to be getBuiltCommandLength(commandLength) in size.
  
  //build command
  //prepend initializer and length (commandLength + 8)
  byte commandPrefix[4];
  
  arrayUtils.copyArray(prefix, 4, commandPrefix);
  
  commandPrefix[2] = ((commandLength + 8) >> 8) & 0xff;
  commandPrefix[1] = (commandLength + 8) & 0xff;

  
  arrayUtils.concatArrays(commandPrefix, 4, command, commandLength, builtCommand);
  
  //append checksum
  byte checksum[2];
  byte revChecksum[2];
  computeChecksum(builtCommand, commandLength + 4, checksum);
  arrayUtils.reverseArray2(checksum, revChecksum);
  
  arrayUtils.concatArrays(builtCommand, commandLength + 4, revChecksum, 2, builtCommand);
  arrayUtils.concatArrays(builtCommand, commandLength + 6, terminator, 2, builtCommand);

} 

int SignController::getBuiltCommandLength(int commandLength) {
  return commandLength + 8;
}


void SignController::computeChecksum(byte command[], int commandLength, byte checksum[2]) {
  
  unsigned int sum = 0;
  for (int i = 0; i < commandLength; i++) {
    sum += command[i];
    sum &= 0xffff;       /* Keep it within bounds. */
  }

  checksum[0] = (byte)((sum >> 8) & 0xff);
  checksum[1] = (byte)(sum & 0xff);
}



void SignController::debugOutput(String header, byte arr[], int arrSize) {
  
  if (!_debugMode)
    return;
  
  Serial.println("------------------------");
  Serial.println(header);
  for (int i = 0; i < arrSize; i+=2) {
    if (arr[i] < 0x10)
      Serial.print(0);
    Serial.print((int)arr[i], HEX);
    if (arr[i + 1] < 0x10)
      Serial.print(0);
    Serial.print((int)arr[i + 1], HEX);
    Serial.print(" ");
  }
  Serial.print("\n");
  Serial.print(arrSize);
  Serial.print(" bytes.\n");
  Serial.flush();
}



void SignController::writeToSerial( byte arr[], int arrSize) {
  softSerial.write(arr, arrSize);
  softSerial.flush();
  
  
  while (softSerial.available()) {
    softSerial.read();
  }
  
  debugOutput("writeToSerial", arr, arrSize);

  delay(50);
}


