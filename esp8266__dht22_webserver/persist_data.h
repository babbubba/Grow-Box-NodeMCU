#ifndef PERSIST_DATA_H
#define PERSIST_DATA_H

#define EEPROM_SIZE 32

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

void write_temeperatureControlActive(bool temeperatureControlAvtive);
void write_temperatureMin(int temperatureMin);
void write_temperatureMax(int temperatureMax);
void write_temperatureMinNight(int temperatureMinNight);
void write_temperatureMaxNight(int temperatureMaxNight);

bool read_temeperatureControlActive();
int read_temperatureMin();
int read_temperatureMax();
int read_temperatureMinNight();
int read_temperatureMaxNight();

void write_heatingActiveForMinutes(int heatingActiveForMinutes);
int read_heatingActiveForMinutes();
void write_heatingActiveForSeconds(int heatingActiveForSeconds);
int read_heatingActiveForSeconds();
void write_heatingIdleForMinutes(int heatingIdleForMinutes);
int read_heatingIdleForMinutes();
void write_heatingIdleForSeconds(int heatingIdleForSeconds);
int read_heatingIdleForSeconds();

#endif
