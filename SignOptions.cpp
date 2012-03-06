#include "SignOptions.h"


SignOptions::SignOptions() {
   _scrollMode = ScrollModes::MOVE_TO_LEFT_LOOP; 
   _scrollRate = 0x48;
   _scrollFreeze = 0;
   _borderMode = BorderModes::NONE;  
   
}



void SignOptions::setScrollMode(int scrollMode) {
  _scrollMode = scrollMode;
}

void SignOptions::setScrollRate(int scrollRate) {
  _scrollRate = scrollRate;
}

void SignOptions::setScrollFreeze(int scrollFreeze) {
  _scrollFreeze = scrollFreeze;
}

void SignOptions::setBorderMode(int borderMode) {
  _borderMode = borderMode;
}


int SignOptions::getScrollMode() {
  return _scrollMode;
}

int SignOptions::getScrollRate() {
  return _scrollRate;
}

int SignOptions::getScrollFreeze() {
  return _scrollFreeze;
}

int SignOptions::getBorderMode() {
  return _borderMode;
}


