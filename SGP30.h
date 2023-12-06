//SGP30.h

#ifndef SGP30_h
#define SGP30_h

#include "Arduino.h"
#include "Wire.h"

class SGP30
{
  public:
    SGP30(uint8_t sgpPin)
    { };

    void Startup();

    void sendSGP30Command(uint16_t command);

    bool initSGP30();

    bool checkCRC(byte data[], int length);
    
    void readAndDisplayData(const char* dataType);

  private:

    // SGP30 I2C address
    #define SGP30_I2C_ADDR 0x58
    #define MEASURE_AIR_QUALITY 0x2008
    #define MEASURE_RAW_SIGNALS 0x2050
    
};

#endif
