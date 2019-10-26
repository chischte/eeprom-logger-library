/*
 * *****************************************************************************
 * EEPROM_Logger.h
 * Library to manage log entries and store them on the non volatile arduino EEPROM
 * Michael Wettstein
 * OctoberSeptember 2019, Zürich
 * *****************************************************************************
 * The usage of this library is explained in the readme of the GitHub repository:
 * https://github.com/chischte/eeprom-logger-library.git
 // * *****************************************************************************
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

  struct LogStruct {
    long logCycleNumber;
    long logCycleTime;
    byte logErrorCode;
  };

  LogStruct newLog;

  // FUNTIONS:
  LogStruct FunctionReturningStruct();
  EEPROM_Logger(int eepromMinAddress, int eepromMaxAddress, int numberOfValues);

  // VARIABLES:

private:
  // FUNCTIONS:
  void MergeTimeAndErrorCode(long errorTime, byte errorCode);
  void SplitTimeAndErrorCode(long eepromTimeAndError);

  // VARIABLES:
  int _oldestErrorSlot = 1;

};
#endif
