ADP2000 Arduino Library

Author: Rabindra Sharma  Date: 22-12-2024

Introduction

The ADP2000 is a high-precision differential pressure sensor with built-in temperature measurement capabilities. It communicates via the I2C interface, making it simple to integrate with microcontrollers like the ESP32. This library provides an easy-to-use interface for working with the ADP2000 in the Arduino framework.

The ADP2000 zip is a library compatible ADP2000.

Features

Measure differential pressure in Pascals (Pa).

Measure temperature in degrees Celsius (C).

Built-in I2C communication.

CRC validation for reliable data transmission.

This library has been tested on an ESP32 microcontroller.

Getting Started

Prerequisites

Arduino IDE installed.

ESP32 or compatible microcontroller.

ADP2000 sensor connected to the I2C pins.

Installation

Clone or download this repository.

Copy the library files to your Arduino libraries directory (~/Documents/Arduino/libraries/).

Restart the Arduino IDE.

Ensure pull-up resistors are present on the SDA and SCL lines if not included in your module.


Notes

The library uses a scaling factor to convert raw sensor data into meaningful values.

Ensure CRC validation passes to verify data integrity.



