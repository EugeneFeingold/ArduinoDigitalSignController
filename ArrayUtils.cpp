#include "Arduino.h"
#include "ArrayUtils.h"


void ArrayUtils::concatArrays(byte input1[], int input1Length, byte input2[], int input2Length, byte output[]) {
  for (int i = 0; i < input1Length; i++)
    output[i] = input1[i];
  for (int i = 0; i < input2Length; i++)
    output[i + input1Length] = input2[i];
}

void ArrayUtils::copyArray(byte input[], int inputLength, byte output[]) {
  for (int i = 0; i < inputLength; i++)
    output[i] = input[i];
}

void ArrayUtils::addToArray(byte array[], int pos, byte arrayToAdd[], int arrayToAddLength) {
  for (int i = 0; i < arrayToAddLength; i++)
    array[pos + i] = arrayToAdd[i];
}


void ArrayUtils::reverseArray(byte input[], int inputLength, byte output[]) {
  for (int i = 0; i < inputLength; i++)
    output[inputLength - i - 1] = input [i];
}


//reverses array of length 2
void ArrayUtils::reverseArray2(byte input[2], byte output[2]) {
  output[0] = input[1];
  output[1] = input[0];
}

//adds 0x00 after each byte
void ArrayUtils::doubleBytes(byte input[], int inputLength, byte output[]) {
  for (int i = 0; i < inputLength; i++) {
    output[i*2] = input[i];
    output[i*2 + 1] = 0x00;
  }
}
