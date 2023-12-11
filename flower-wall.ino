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

  Serial.println("Starting Flower Wall on ESP-WROOM-32");
  Serial.println("Using: DHT22, SGP30, KY037");

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
  SGP30.sendSGP30Command(MEASURE_AIR_QUALITY);
  delay(10); // Delay in between I2C transactions, otherwise will not work and you receive message: "Data not available or not processed successfully"
  SGP30.readAndDisplayData();

  Serial.print("DHT22 TEMP: "); Serial.println(DHT22.getTemp(dhtPin));
  Serial.print("DHT22 HUMID: "); Serial.println(DHT22.getHumid(dhtPin));

  Serial.print("KY037: "); Serial.println(get_sound_value());
  Serial.println("\n");
  delay(500);
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
