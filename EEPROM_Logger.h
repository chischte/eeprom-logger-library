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
 * TODO / Notes
 * KEEP TRACK OF THE 1st (oldest) LOG
 * In the first run through the store slots, entry number 1 stays at storeslot no 1
 * Afterwards if for example a value is stored on slot 5, the 1st log will be the one on slot 6
 *
 */

#ifndef EEPROM_Logger_h
#define EEPROM_Logger_h

#include "Arduino.h"
#include <EEPROM_Counter.h>

class EEPROM_Logger {
public:
  // FUNTIONS:
  EEPROM_Logger(int eepromMinAddress, int eepromMaxAddress, int numberOfValues);
  //struct logData getLogData(int logNumber);
  struct LogStruct FunctionReturningStruct();

  // VARIABLES:
  struct LogStruct {
    long logCycleNumber;
    long logCycleTime;
    byte logErrorCode;
  };
  LogStruct newStruct;

private:
  // FUNCTIONS:
  void MergeTimeAndErrorCode(long errorTime, byte errorCode);
  void SplitTimeAndErrorCode(long eepromTimeAndError);

  // VARIABLES:
  int _oldestErrorSlot = 1;

};
#endif
