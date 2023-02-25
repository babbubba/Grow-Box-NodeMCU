#ifndef PERSIST_DATA_H
#define PERSIST_DATA_H

void writeIntToEEPROM(int address, int number);
int readIntFromEEPROM(int address);

#endif

