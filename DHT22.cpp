#include "DHT22.h"

//environmental parameters
float temp = 0;
float humidity = 0;
uint8_t dhtPin;


uint8_t DHT22::StartSignal()
{
  if(!_firstStart && millis() - _timer < samplingTime)
  {
    return OK;
  }
  _timer = millis();
  _firstStart = false;

  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, LOW);
  delay(2);
  return 0;
}

bool DHT22::checksumValidation()
{
  uint8_t sum = highByte(_humid16bits) + lowByte(_humid16bits) + highByte(_temp16bits) + lowByte(_temp16bits);
  return sum == _checksum8bits;
}

//DHT22 data gathering
//DATA SHEET: https://www.sparkfun.com/datasheets/Sensors/Temperature/DHT22.pdf
uint8_t DHT22::readSensor(uint8_t pin) 
{
  //the data structure of the sensor consists of 40 bits in the format of 5 rounds of 8 bits: (int)RH, (decimal)RH,
  //(int) Temp, (decimal) Temp, and the checksum || the checksum is the last bit of the first 4 rounds combined
  if(!_firstStart && (millis() - _timer) < samplingTime)
  {
    return OK;
  }
  _timer = millis();
  _firstStart = false;

  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, LOW);
  delay(2);

  //StartSignal();

  digitalWrite(_pin, HIGH);
  pinMode(_pin, INPUT);
  int32_t t = pulseIn(_pin, HIGH, 250);
  if(t == 0) return (_error = TIMING_ERROR);

  _full64bits = 0;

  //START READING
  for(int i = 0; i < 40; i++)
  {
    uint32_t t = micros();
    while(digitalRead(_pin) == 0)
    {
      if ((micros()-t) > _t50last + 30) return (_error = TIMING_ERROR);
    }
    delayMicroseconds(40);
    if(digitalRead(_pin) == 1) _full64bits++;
    if(i != 39) _full64bits <<= 1;
    
    t = micros();
    while(digitalRead(_pin) == 1)
    {
      if((micros() - t) > _bit1) return (_error = TIMING_ERROR);
    }
  }

  delayMicroseconds(10);
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, HIGH);
  _humid16bits = _full64bits >> 24;
  _temp16bits = _full64bits >> 8;
  _checksum8bits = _full64bits;

  if(checksumValidation()) return OK;
  else return (_error = CHECKSUM_ERROR);
  
}


void DHT22::measureTimings() 
{
  //initial timing and measurements set

  if(!_firstStart && millis() - _timer < samplingTime)
  {
    _error = 0;
  }
  _timer = millis();
  _firstStart = false;

  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, LOW);
  delay(2);
  //StartSignal();
  _bit0 = 0;
  _bit1 = 0;

  //level to HIGH and wait for sensor
  digitalWrite(_pin, HIGH);
  pinMode(_pin, INPUT);

  uint32_t t1 = micros();
  uint32_t t2 = 0;

  while(digitalRead(_pin) == 1) //wait1
  {
    if( (micros() - t1) > 60) return; //expected 20-40 microseconds
  }
  t1 = micros(); //update time post wait1

  while(digitalRead(_pin) == 0) //wait2
  {
    t2 = micros() - t1;
    if(t2 > 100) return; //expected 80 microseconds in LOW
  }
  _t80L = t2;
  t1 = micros(); //update time post wait2

  while(digitalRead(_pin) == 0) //wait3
  {
    t2 = micros() - t1;
    if(t2 > 100) return; //expected 80 microseconds in LOW
  }
  _t80H = t2;
  t1 = micros(); //update time post wait3

  while(digitalRead(_pin) == 0) //wait4 - last before transmit
  {
    t2 = micros() - t1;
    if (t2 > 60) return; //expected 50 microseconds; this is the last transmission before the 40-bit set starts
  }
  _t50last = t2;
  t1 = micros();

  //START BIT TIME READING
  while(_bit0 == 0 || _bit1 == 0)
  {
      while(digitalRead(_pin) == 1) //read the high time; 26-28 means "0", 70 means "1"
      {
          t2 = micros() - t1;
          if(t2 > 100) return; //timed out
      }
      if(t2 > 40)
        _bit1 = t2;
      else
        _bit0 = t2;
      
      t1 = micros();

      while(digitalRead(_pin) == 0) //wait next bit
      {
        t2 = micros() - t1;
        if(t2 > 100) return;
      }
      t1 = micros();
  }

}

float DHT22::getTemp(uint8_t pin)
{
  if ( readSensor(_pin) != OK) return -1;
  if (bitRead(_temp16bits, 15)) return (_temp16bits & 0x7FFF) / -10.0;
  temp = _temp16bits / 10.0;
  return temp;
}

float DHT22::getHumid(uint8_t pin) 
{
  if ( readSensor(_pin) != OK) return -1;
  humidity = _humid16bits / 10.0;
  return humidity;
}

String DHT22::getLastError()
{
  if(_error == 0) return "OK";
  if(_error == 1) return "TIMING_ERROR";
  else if(_error == 2) return "BIT_TIMING_ERROR";
  return "CHECKSUM_ERROR";
}
