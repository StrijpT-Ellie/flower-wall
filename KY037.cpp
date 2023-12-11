#include "KY037.h"

long KY037::get_sound_value(uint8_t kyPin)
{
  long sum = 0;
    int i = 0;
    const int max = 500;
    while(i < max) 
    {
      sum += analogRead(kyPin);
      i++;
    }
    return sum / max;
}