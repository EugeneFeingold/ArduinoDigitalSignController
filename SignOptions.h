
#ifndef SignOptions_H
#define SignOptions_H

#include "Arduino.h"


class SignOptions {
public:
  SignOptions();
  
  void setScrollMode(int scrollMode);
  void setScrollRate(int scrollRate);
  void setScrollFreeze(int scrollFreeze);
  void setBorderMode(int borderMode);  
  
  int getScrollMode();
  int getScrollRate();
  int getScrollFreeze();
  int getBorderMode();

private:
  int _scrollMode;
  int _scrollRate;  //lower = faster
  int _scrollFreeze;
  int _borderMode;

};


namespace ScrollModes {
  enum ScrollModes {RANDOM, STATIC, MOVE_TO_LEFT, MOVE_TO_RIGHT, MOVE_TO_TOP, MOVE_TO_BOTTOM, 
    COVER_FROM_LEFT, COVER_FROM_TOP, COVER_FROM_CENTER, HORIZONTAL_SHUTTER, VERTICAL_SHUTTER,
    MOVE_TOP_BOTTOM, COVER_LEFT_RIGHT, MOVE_TO_LEFT_LOOP, COVER_DIAMOND,
    MOVE_TO_BOTTOM_DIAGONAL, COVER_FROM_LEFT_DIAGONAL, MOSAIC, RAIN  };
}

namespace BorderModes {
 enum BorderModes {NONE, FAST_1, SLOW_1, FAST_4, SLOW_4}; 
}

#endif 

