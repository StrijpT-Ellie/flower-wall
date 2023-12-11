//KY037.h

#ifndef KY037_h
#define KY037_h

#include "Arduino.h"

class KY037
{
  public:

    KY037(uint8_t kyPin)
    { };

    long get_sound_value(uint8_t kyPin);

  private:
    uint8_t kyPin;

};

#endif