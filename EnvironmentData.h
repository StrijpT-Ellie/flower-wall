//EnvironmentData.h

#ifndef EnvData_h
#define EnvData_h

#include "Arduino.h"



int readDHT(int pin);

class DHT11
{
  public:

    /* Constructor
    * Initializes pin and sets it to OUTPUT mode, value HIGH
    * @param: dhtPin representing the pin connected to the DHT11 sensor; Pin type: Digital
    */
    DHT11(int dhtPin);

    /* FUNCTION
    *  Checks if the sensor connection times out. Timeout time: 10 seconds
    *  
    */
    bool sensorTimeout();

    /* Function
    * Reads the 40-bit data structure received from the sensor;
    * @return value of temperature; -1 if sensor times out or failes checksum verification
    * @param dhtPin representing the pin connected to the DHT sensor; Pin type: Digital
    */
    int readTemp(int dhtPin);
      int temp;


    /* Function
    * Reads the 40-bit data structure received from the sensor;
    * @return value of humidity; -1 if sensor times out or failes checksum verification
    * @param dhtPin representing the pin connected to the DHT sensor; Pin type: Digital
    */
    int readHumid(int dhtPin);
      int humidity;



  private:
    int _pin;

    byte readByte();


    void startupSequence();
};

#endif