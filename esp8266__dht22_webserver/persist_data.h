#ifndef PERSIST_DATA_H
#define PERSIST_DATA_H

#define EEPROM_SIZE 16

int readIntFromEEPROM(int address);
void writeIntToEEPROM(int address, int number);

bool readBoolFromEEPROM(int address);
void writeBoolToEEPROM(int address, bool value);

bool read_lightManual();
void write_lightManual(bool value);

void write_lightManualOnTime(int hour, int minute);
int read_lightManualOnTimeHour();
int read_lightManualOnTimeMinute();

void write_lightManualOffTime(int hour, int minute);
int read_lightManualOffTimeHour();
int read_lightManualOffTimeMinute();

#endif

