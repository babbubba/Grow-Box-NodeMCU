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

void write_temeperatureControlActive(bool temeperatureControlAvtive) {
   writeBoolToEEPROM(10, temeperatureControlAvtive);
}

void write_temperatureMin(int temperatureMin) {
  writeIntToEEPROM(11, temperatureMin);
}

void write_temperatureMax(int temperatureMax) {
  writeIntToEEPROM(13, temperatureMax);
}

bool read_temeperatureControlActive() {
    return readBoolFromEEPROM(10);
}

int read_temperatureMin() {
    return readIntFromEEPROM(11);
}

int read_temperatureMax() {
      return readIntFromEEPROM(13);
}

void write_temperatureMinNight(int temperatureMinNight) {
  writeIntToEEPROM(15, temperatureMinNight);
}

void write_temperatureMaxNight(int temperatureMaxNight) {
  writeIntToEEPROM(17, temperatureMaxNight);
}

int read_temperatureMinNight() {
    return readIntFromEEPROM(15);
}

int read_temperatureMaxNight() {
      return readIntFromEEPROM(17);
}


// Timeout heating
void write_heatingActiveForMinutes(int heatingActiveForMinutes) {
  writeIntToEEPROM(19, heatingActiveForMinutes);
}
int read_heatingActiveForMinutes() {
      return readIntFromEEPROM(19);
}
void write_heatingActiveForSeconds(int heatingActiveForSeconds) {
  writeIntToEEPROM(21, heatingActiveForSeconds);
}
int read_heatingActiveForSeconds() {
      return readIntFromEEPROM(21);
}
void write_heatingIdleForMinutes(int heatingIdleForMinutes) {
  writeIntToEEPROM(23, heatingIdleForMinutes);
}
int read_heatingIdleForMinutes() {
      return readIntFromEEPROM(23);
}
void write_heatingIdleForSeconds(int heatingIdleForSeconds) {
  writeIntToEEPROM(25, heatingIdleForSeconds);
}
int read_heatingIdleForSeconds() {
      return readIntFromEEPROM(25);
}