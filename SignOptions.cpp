#include "SignOptions.h"


SignOptions::SignOptions() {
  _scrollMode = ScrollModes::MOVE_TO_LEFT_LOOP; 
  _scrollRate = 0x48;
  _scrollFreeze = 0;
  _borderMode = BorderModes::NONE;  

}



void SignOptions::setScrollMode(int scrollMode) {  //see ScrollModes.ScrollModes enum
  if (scrollMode >= 0 && scrollMode <=0x0f)
    _scrollMode = scrollMode;
}

void SignOptions::setScrollRate(int scrollRate) {
  if (scrollRate >= 0 && scrollRate <= 0xff)
    _scrollRate = scrollRate;
}

void SignOptions::setScrollFreeze(int scrollFreeze) {
  if (scrollFreeze >= 0 && scrollFreeze <= 0xff)
    _scrollFreeze = scrollFreeze;
}

void SignOptions::setBorderMode(int borderMode) {
  if (borderMode >= 0 && borderMode <= 4)
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




