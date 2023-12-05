//DHT22.h

#ifndef DHT22_h
#define DHT22_h

#include "Arduino.h"

int readDHT(uint8_t pin);

class DHT22
{
  public:

    /* Constructor
    * Initializes pin and sets it to OUTPUT mode, value HIGH
    * @param: dhtPin representing the pin connected to the DHT11 sensor; Pin type: Digital
    */
    DHT22(uint8_t dhtPin)
    {
      _pin = dhtPin;
    };

    /* Function
    * Starts up the data transfer between the sensor DHT11 and the board
    * according to manufacturer specififcations from within the data sheet
    */
    uint8_t StartSignal();  

    /* FUNCTION
    * Reads an entire transmission of the sensor, parses and checks its validity.
    * Returns pass or fail for data validation
    */
    bool checksumValidation();

    

    /* Function
    * Reads the 40-bit data structure received from the sensor;
    * @return value of temperature; -1 if sensor times out or failes checksum verification
    * @param dhtPin representing the pin connected to the DHT sensor; Pin type: Digital
    */
    uint8_t readSensor(uint8_t dhtPin);
    

    void measureTimings();


    float getTemp(uint8_t dhtPin);
      float temp;


    /* Function
    * Reads the 40-bit data structure received from the sensor;
    * @return value of humidity; -1 if sensor times out or failes checksum verification
    * @param dhtPin representing the pin connected to the DHT sensor; Pin type: Digital
    */
    float getHumid(uint8_t dhtPin);
      float humidity;


    String getLastError();

    enum errors
    {
      OK,
      TIMING_ERROR,
      BIT_TIMING_ERROR,
      CHECKSUM_ERROR
    };


  private:
    uint8_t _pin;
    uint8_t _error;
    uint32_t _timer;
    uint64_t _full64bits;
    uint8_t _humid16bits;
    uint8_t _temp16bits;
    uint8_t _checksum8bits;

    bool _firstStart = true;

    //TIMINGS
    uint8_t _bit0 = 27; //first voltage pulls before transmission
    uint8_t _bit1 = 70; //first voltage pulls before transmission
    uint8_t _t80L = 80; //moment the sensor finishes the 80 microseconds LOW signal
    uint8_t _t80H = 80; //moment the sensor finishes the 80 microseconds HIGH signal
    uint8_t _t50last = 50;

    const uint16_t samplingTime = 2100;

    uint8_t readByte();


    void startupSequence();
};

#endif