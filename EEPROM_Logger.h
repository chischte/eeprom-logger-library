/*
 * *****************************************************************************
 EEPROM_Counter.h
 Library to store long values on the non volatile arduino EEPROM
 Michael Wettstein
 September 2019, Zürich
 * *****************************************************************************
 The EEPROM memory has a specified lifetime of 100,000 write/erase cycles
 To wear out the EEPROM memory evenly, the storage location will be changed periodically
 Storage data Type long (4Bytes)
 * *****************************************************************************
 EEPROM USAGE
 address of the _storeLocation_: 0
 address of the writecounter: _storeLocation (4bytes)
 address of value-n: _storelocation + n*sizeof(long)  
 * *****************************************************************************
 TODO:
 CLEAN UP THE WHOLE CODE
 CHANGE BYTES BACK TO BYTES (NOT INT)
 */

#ifndef EEPROM_Logger_h
#define EEPROM_Logger_h

#include "Arduino.h"
#include <EEPROM_Counter.h>

class EEPROM_Logger {
public:
  // FUNTIONS:
  EEPROM_Logger(int eepromMinAddress, int eepromMaxAddress, int numberOfValues);
  //void countOneUp(int valueNumber);
  // FUNTIONS:
  //EEPROM_Logger(int eepromMinAddress, int eepromMaxAddress, int numberOfValues);


  // VARIABLES:
  // n.a.

private:
  // FUNCTIONS:


  // VARIABLES:


};
#endif
