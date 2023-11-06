#include "EnvironmentData.h"

//environmental parameters
int temp = 0;
int humidity = 0;

int dhtPin;
//int CO2 = 0;
//int soundLv = 0;

//CONSTRUCTOR
DHT11::DHT11(int dhtPin)
{
  _pin = dhtPin;
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, HIGH);
}


//0 means no timeout, 1 means timed out
bool DHT11::sensorTimeout() 
{
  //timeout check; DEF timeout as: the read value stays the same over 10 seconds
  unsigned int loop = 10000;
  while(digitalRead(dhtPin) == LOW)
  {
    if(--loop <= 0)
    {
      Serial.println("DHT11 TIMEOUT");
      return 1;
    }
  }
  loop = 10000;
  while(digitalRead(dhtPin) == HIGH)
  {
    if(--loop <= 0)
    {
      Serial.println("DHT11 TIMEOUT");
      return 1;
    }
  }
}

byte DHT11::readByte()
{
  byte val = 0;

  for(int i = 0; i < 8; i++)
  {
    while(digitalRead(_pin) == LOW) ;

    if(digitalRead(_pin) == HIGH)
    {
      val |= (1 << (7 - i));
    }
    while(digitalRead(_pin) == HIGH) ;
  }
  return val;
}

//DHT11 data gathering
//DATA SHEET: https://www.mouser.com/datasheet/2/758/DHT11-Technical-Data-Sheet-Translated-Version-1143054.pdf
int DHT11::readTemp(int pin) 
{
  //the data structure of the sensor consists of 40 bits in the format of 5 rounds of 8 bits: (int)RH, (decimal)RH,
  //(int) Temp, (decimal) Temp, and the checksum || the checksum is the last bit of the first 4 rounds combined
  uint8_t data[5];
  uint8_t index = 0;

  //empty buffer
  for (int i = 0; i < 5; i++) data[i] = 0;

  //startup times of sensor
  pinMode(dhtPin, OUTPUT);
  digitalWrite(dhtPin, LOW);
  delay(18);
  digitalWrite(dhtPin, HIGH);
  delay(40);
  pinMode(pin, INPUT);
  Serial.println("Passed startup time");

  //timeout check; DEF timeout as: the read value stays the same over 10 seconds
  bool timeoutCheck = sensorTimeout();
  if(timeoutCheck == 1) return -1;
  Serial.println("Passed timeout check");


  //data read; 
  //Data struct of the sensor is 40 bits
  for(int i = 0; i < 40 && index < 5; i++)
  {
    data[i] = readByte();
    index++;

  }

  //pull output values
  //we could aproximate the decimals(index 1 and 3) later; ignoring them here
  temp = data[2];

  //Checksum check
  uint8_t sum = data[0] + data[1] + data[2] + data[3];
  if(data[4] != sum)
  {
    Serial.println("DATA HAS FAILED VALIDATION VIA CHECKSUM");
    return -1;
  }

  return temp;

}

int DHT11::readHumid(int pin) 
{
  //the data structure of the sensor consists of 40 bits in the format of 5 rounds of 8 bits: (int)RH, (decimal)RH,
  //(int) Temp, (decimal) Temp, and the checksum || the checksum is the last bit of the first 4 rounds combined
  uint8_t data[5];
  uint8_t index = 0;

  //empty buffer
  for (int i = 0; i < 5; i++) data[i] = 0;

  //startup times of sensor
  pinMode(dhtPin, OUTPUT);
  digitalWrite(dhtPin, LOW);
  delay(18);
  digitalWrite(dhtPin, HIGH);
  delay(40);
  pinMode(pin, INPUT);
  Serial.println("Passed startup time");

  //timeout check; DEF timeout as: the read value stays the same over 10 seconds
  bool timeoutCheck = sensorTimeout();
  if(timeoutCheck == 1) return -1;
  Serial.println("Passed timeout check");


  //data read; 
  //Data struct of the sensor is 40 bits
  for(int i = 0; i < 40 && index < 5; i++)
  {
    data[i] = readByte();
    index++;

  }

  //pull output values
  //we could aproximate the decimals(index 1 and 3) later; ignoring them here
  humidity = data[0];

  //Checksum check
  uint8_t sum = data[0] + data[1] + data[2] + data[3];
  if(data[4] != sum)
  {
    Serial.println("DATA HAS FAILED VALIDATION VIA CHECKSUM");
    return -1;
  }

  return humidity;

}