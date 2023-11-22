#include <Wire.h>

// SGP30 I2C address
#define SGP30_I2C_ADDR 0x58
#define MEASURE_AIR_QUALITY 0x2008
#define MEASURE_RAW_SIGNALS 0x2050

void setup() {
  Serial.begin(9600);  // Initialize serial communication
  Wire.begin();        // Initialize I2C communication

  Wire.beginTransmission(SGP30_I2C_ADDR);
  Wire.write(0x06);  
  Wire.endTransmission();
  delay(20);  

  if (!initSGP30()) {   // Initialize SGP30
    Serial.println("SGP30 initialization failed!");
    while (1); //This is an infinite loop (the condition 1 is always true)
  } else {
    Serial.println("SGP30 initialized successfully!");
  }
}


void loop() {
  sendSGP30Command(MEASURE_AIR_QUALITY);  // Send Measure_air_quality command
  delay(1000);  // Wait for measurement completion (ad
  readAndDisplayData("Air Quality Data");  // read and process air quality data


  // Send Measure_raw_signals command
  // sendSGP30Command(MEASURE_RAW_SIGNALS);

  // Wait for measurement completion (adjust delay as needed)
  delay(1000);

  // Read and process raw signals data
  // readAndDisplayData("Raw Signals Data");
}

void sendSGP30Command(uint16_t command) { //is responsible for sending a command to the SGP30 sensor over the I2C 
  Wire.beginTransmission(SGP30_I2C_ADDR); //initiates communication with the device at the specified I2C address
  Wire.write(command >> 8);   // Send the high byte of the command, shifts the bits of the command variable 8 positions to the right
  Wire.write(command & 0xFF); // Send the low byte of the command
  Wire.endTransmission();//finalizes the I2C communication and transmits the command to the sensor
}


bool initSGP30() {
  Wire.beginTransmission(SGP30_I2C_ADDR);  // Initialize air quality measurements
  Wire.write(0x20);//Writes the high byte of the command
  Wire.write(0x03);  // Writes the low byte of the command, command to start air quality measurements
  Wire.endTransmission(); //Finalizes the I2C communication and sends the command to the SGP30 sensor.
  delay(10);

  Wire.requestFrom(SGP30_I2C_ADDR, 3);
  if (Wire.available() == 3) {
    byte featureSet[3];
    for (int i = 0; i < 3; i++) {
      featureSet[i] = Wire.read();
    }
    if (checkCRC(featureSet, 3)) {
      return true;   
    }
  }

  return false;  
}

bool checkCRC(byte data[], int length) {
  uint8_t crc = 0xFF;
  for (int i = 0; i < length; i++) {
    crc ^= data[i]; //XORs (exclusive OR) the current byte of the data with the current CRC value
    for (int bit = 0; bit < 8; bit++) {
      if (crc & 0x80) {
        crc = (crc << 1) ^ 0x31;  // CRC-8 formula (x^8 + x^5 + x^4 + 1)b (datasheet)
      } else {
        crc <<= 1;
      }
    }
  }

  return crc == 0;
}
// bool pollSGP30Data() {
//   Wire.requestFrom(SGP30_I2C_ADDR, 6);  
//   delay(50);  // Allows some time for data 

//   // Check if data is available
//   if (Wire.available() == 6) {  
//     byte data[6];
//     for (int i = 0; i < 6; i++) {
//       data[i] = Wire.read();
//     }
//     return true;  // data available and processed successfully
//   }
//   return false;  // Data not available :( or not processed successfully
// }

void readAndDisplayData(const char* dataType) {
  Wire.requestFrom(SGP30_I2C_ADDR, 6);  
  delay(50);
  if (Wire.available() == 6) {  
    byte data[6];
    for (int i = 0; i < 6; i++) {
      data[i] = Wire.read();
    }

    // Display data
    Serial.println(dataType);
    for (int i = 0; i < 6; i++) {
      Serial.print(data[i]);
      Serial.print(" ");
    }
    Serial.println();
  } else {
    Serial.println("Data not available or not processed successfully");
  }
}

