#include "persist_data.h"
#include <Arduino.h>
#include <EEPROM.h>

// define the number of bytes you want to access

void writeIntToEEPROM(int address, int number)
{ 
  byte byte1 = number >> 8;
  byte byte2 = number & 0xFF;
  EEPROM.write(address, byte1);
  EEPROM.write(address + 1, byte2);
}

int readIntFromEEPROM(int address)
{
  byte byte1 = EEPROM.read(address);
  byte byte2 = EEPROM.read(address + 1);
  return (byte1 << 8) + byte2;
}

void writeBoolToEEPROM(int address, bool value) {
  byte byte1 = value;
  EEPROM.write(address, byte1);
}

bool readBoolFromEEPROM(int address) {
  byte byte1 = EEPROM.read(address);
  return byte1;
}

bool read_lightManual() {
  return readBoolFromEEPROM(1);
}

void write_lightManual(bool value) {
  writeBoolToEEPROM(1, value);
}

//lightManualOnTime
void write_lightManualOnTime(int hour, int minute) {
  writeIntToEEPROM(2, hour);
  writeIntToEEPROM(4, minute);
}

int read_lightManualOnTimeHour() {
  return readIntFromEEPROM(2);
}

int read_lightManualOnTimeMinute() {
  return readIntFromEEPROM(4);
}

//lightManualOffTime
void write_lightManualOffTime(int hour, int minute) {
  writeIntToEEPROM(6, hour);
  writeIntToEEPROM(8, minute);
}

int read_lightManualOffTimeHour() {
  return readIntFromEEPROM(6);
}

int read_lightManualOffTimeMinute() {
  return readIntFromEEPROM(8);
}


