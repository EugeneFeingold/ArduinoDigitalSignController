
#include "Arduino.h"

class ArrayUtils {
 public:
  void concatArrays(byte input1[], int input1Length, byte input2[], int input2Length, byte output[]);
  void copyArray(byte input[], int inputLength, byte output[]);
  void addToArray(byte array[], int pos, byte arrayToAdd[], int arrayToAddLength);
  
  void reverseArray(byte input[], int inputLength, byte output[]);
  void reverseArray2(byte input[2], byte output[2]);
  void doubleBytes(byte input[], int inputLength, byte output[]);
  
 private: 
  
};

