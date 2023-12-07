#include "DHT22.h"
#include "SGP30.h"

#include <Wire.h>

const uint8_t dhtPin = 4;
const uint8_t sgpPin = SDA;
const uint8_t kyPin = 2;

DHT22 DHT22(dhtPin); //constructor dht22
SGP30 SGP30(sgpPin); //constructor sgp30

void setup() 
{
  //serial start
  Serial.begin(115200);
  Wire.begin();

  while (!Serial && millis() < 5000)
    delay(200);

  Serial.print(F("\nStarting Flower Wall on "));
  Serial.print("ESP-WHOOP-32");
  Serial.print(F(" with "));
  Serial.println("DHT22, SGP30, KY037");

  //delay for startup
  delay(500);

  SGP30.Startup();
  SGP30.initSGP30();
  
  pinMode(dhtPin, OUTPUT);
  digitalWrite(dhtPin, HIGH);
  SGP30.Startup();
  delay(2000); //passing unstable time

  pinMode(kyPin, INPUT); 
}

void loop() 
{
  Serial.println("SGP30:  \n");
  SGP30.sendSGP30Command(MEASURE_AIR_QUALITY);
  SGP30.readAndDisplayData("Air Quality Data");

  Serial.println("DHT22:  \n");
  Serial.println(DHT22.getTemp(dhtPin));
  Serial.println(DHT22.getHumid(dhtPin));

  Serial.println("KY037:  \n");
  Serial.println(get_sound_value());
}

long get_sound_value() 
{
  long sum = 0;
  int i = 0;
  const int max = 500;
  while(i < max) {
    sum += analogRead(kyPin);
    i++;
  }

  return sum / max;
}
