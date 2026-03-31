#pragma once
#include "Led.h"

class SevenSegmentDisplay {
  private:
    int _points = 0;
    vector<Led> _leds;
  
  public:
    void init(vector<Led> leds) {
      _leds = leds;

      for (Led led : _leds) {
        led.toggle();
        delay(50);
        led.toggle();
      }
    }

    void addPoint() {
      _points++;
      if (_points == 10) {
        resetScore();
      }
    }

    void resetScore() {
      _points = 0;
    }

    int getScore() {
      return _points;
    }

    void showScore() {
      static const bool digits[10][7] = {
          {1,1,1,1,1,1,0}, // 0
          {0,1,1,0,0,0,0}, // 1
          {1,1,0,1,1,0,1}, // 2
          {1,1,1,1,0,0,1}, // 3
          {0,1,1,0,0,1,1}, // 4
          {1,0,1,1,0,1,1}, // 5
          {1,0,1,1,1,1,1}, // 6
          {1,1,1,0,0,0,0}, // 7
          {1,1,1,1,1,1,1}, // 8
          {1,1,1,1,0,1,1}  // 9
      };

      int digit = (_points >= 0 && _points <= 9) ? _points : 0;

      for (int i = 0; i < 7; i++) {
          _leds[i].enabled(digits[digit][i]);
      }
  }
};