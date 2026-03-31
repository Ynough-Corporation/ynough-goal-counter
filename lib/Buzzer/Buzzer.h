#pragma once
#include <Arduino.h>

class Buzzer {
  private:
  int _pin = 0;
  bool _status = false;

  void _setStatus(bool status)
  {
    digitalWrite(_pin, status);
    this->_status = status;
  }

public:
  Buzzer(int pin)
  {
    this->_pin = pin;
    pinMode(pin, OUTPUT);
    this->_setStatus(false);
  }

  void enabled(bool state)
  {
    this->_setStatus(state);
  }

  void toggle()
  {
    this->enabled(!this->_status);
  }
};