#pragma once
#include <Arduino.h>

class UltraSoundSensor {
  private:
    int _EchoPin = 0;
    int _TriggPin = 0;
    int _MaxDist = 0;
    int _MinDist = 0; 
    long _Time = 0;
    long _Dist = -1;

  public:
    void init(int EchoPin, int TrigPin, int MinDist, int MaxDist) {
      this->_EchoPin = EchoPin;
      this->_TriggPin = TrigPin;
      this->_MinDist = MinDist;
      this->_MaxDist = MaxDist;

      pinMode(EchoPin, INPUT);
      pinMode(TrigPin, OUTPUT);
    }

    void resetTrig() {
      digitalWrite(_TriggPin, LOW);
      delayMicroseconds(2);
    }

    void setTrig() {
      digitalWrite(_TriggPin, HIGH);
      delayMicroseconds(10);
    }

    void updateTime() {
      _Time = pulseIn(_EchoPin, HIGH);
    }

    void updateDist() {
      _Dist = _Time*0.034/2;
    }

    bool objectDetected() {
      return !(_Dist >= _MaxDist || _Dist <= _MinDist);
    }

    void resetData() {
      _Time = 0;
      _Dist = -1;
    }
};