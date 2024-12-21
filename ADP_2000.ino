//  Author: Rabindra Sharma  
//  Date : 22-12-2024

#include <Wire.h>
#define ADP2000_I2C_ADDRESS 0x25  // 7-bit I2C address for the ADP2000
#define CONTINUOUS_MEASUREMENT_CMD 0x361E
#define SINGLE_MEASUREMENT_CMD 0x372D
#define PRODUCT_TYPE_CMD 0xE201

// Scaling factors
#define PRESSURE_SCALE_FACTOR 240.0
#define TEMPERATURE_SCALE_FACTOR 200.0

// Function prototypes
void sendCommand(uint16_t command);
void readSensorData(uint8_t* data, size_t length);
float convertPressure(int16_t rawValue);
float convertTemperature(int16_t rawValue);
uint8_t calculateCRC8(uint8_t* data, size_t length);

void setup() {
  Wire.begin(); // Initialize I2C bus
  Serial.begin(115200); // Initialize serial communication

  // Verify product type
  uint8_t productData[3];
  sendCommand(PRODUCT_TYPE_CMD);
  readSensorData(productData, sizeof(productData));

  if (productData[0] == 0x41 && productData[1] == 0x53) {
    Serial.println("ADP2000 Sensor Detected");
  } else {
    Serial.println("Sensor Identification Failed");
    while (1);
  }
}

void loop() {
  // Perform continuous measurement
  uint8_t measurementData[6];
  sendCommand(CONTINUOUS_MEASUREMENT_CMD);
  delay(10); // Wait for measurement to complete
  readSensorData(measurementData, sizeof(measurementData));

  // Extract raw values
  int16_t rawPressure = (measurementData[0] << 8) | measurementData[1];
  int16_t rawTemperature = (measurementData[3] << 8) | measurementData[4];

  // Validate CRC
  if (calculateCRC8(measurementData, 2) != measurementData[2] ||
      calculateCRC8(&measurementData[3], 2) != measurementData[5]) {
    Serial.println("CRC Validation Failed");
    return;
  }

  // Convert raw values to physical quantities
  float pressure = convertPressure(rawPressure);
  float temperature = convertTemperature(rawTemperature);

  // Display results
  Serial.print("Pressure (Pa): ");
  Serial.println(pressure);
  Serial.print("Temperature (C): ");
  Serial.println(temperature);

  delay(50); // Read sensor data every second
}

void sendCommand(uint16_t command) {
  Wire.beginTransmission(ADP2000_I2C_ADDRESS);
  Wire.write(command >> 8); // High byte
  Wire.write(command & 0xFF); // Low byte
  Wire.endTransmission();
}

void readSensorData(uint8_t* data, size_t length) {
  Wire.requestFrom(ADP2000_I2C_ADDRESS, length);
  for (size_t i = 0; i < length; i++) {
    data[i] = Wire.read();
  }
}

float convertPressure(int16_t rawValue) {
  return rawValue / PRESSURE_SCALE_FACTOR;
}

float convertTemperature(int16_t rawValue) {
  return rawValue / TEMPERATURE_SCALE_FACTOR;
}

uint8_t calculateCRC8(uint8_t* data, size_t length) {
  uint8_t crc = 0xFF;
  for (size_t i = 0; i < length; i++) {
    crc ^= data[i];
    for (uint8_t bit = 0; bit < 8; bit++) {
      if (crc & 0x80) {
        crc = (crc << 1) ^ 0x31;
      } else {
        crc <<= 1;
      }
    }
  }
  return crc;
}
