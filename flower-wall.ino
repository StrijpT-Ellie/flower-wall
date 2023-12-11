#include "DHT22.h"
#include "SGP30.h"
#include "KY037.h"

#include <Wire.h>

const uint8_t dhtPin = 4;
const uint8_t sgpPin = SDA;
const uint8_t kyPin = 2;

DHT22 DHT22(dhtPin); //constructor dht22
SGP30 SGP30(sgpPin); //constructor sgp30
KY037 KY037(kyPin); //constructor ky037

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
  
  pinMode(dhtPin, OUTPUT);
  digitalWrite(dhtPin, HIGH);
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
  Serial.println(KY037.get_sound_value(kyPin));

  delay(2000);
}
