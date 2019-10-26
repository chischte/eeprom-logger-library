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
// SET UP THE EEPROM COUNTER FOR 100 LOG ENTRIES:
// 100*2 long for the log entrys + 1 log for the slotManager= 201 long values

class EEPROM_Logger {

public:
//EEPROM_Counter eepromCounter(1001, 4095, 201);
  struct LogStruct {
    long logCycleNumber;
    long logCycleTime;
    byte logErrorCode;
  };

  LogStruct newLog;

  // FUNTIONS:
  long eepromReadAndWrite(long value, long int);
  LogStruct functionReturningStruct();
  EEPROM_Logger(int eepromMinAddress, int eepromMaxAddress, int numberOfLogEntries);

  // VARIABLES:

private:
  // FUNCTIONS:
  void storeLog(long cycleNumber, long cycleTime, byte ErrorCode);
  void mergeTimeAndErrorCode(long errorTime, byte errorCode);
  void splitTimeAndErrorCode(long eepromTimeAndError);

  // VARIABLES:
  int _oldestErrorSlot = 1;
  int _eepromMinAddress;
  int _eepromMaxAddress;
  int _eepromNumberOfLongs=1;
  byte _oneForTheSlotManager=1;

};
#endif
