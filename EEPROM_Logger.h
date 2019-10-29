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
 * Measures if a new board is used, overwrite impossible entries
 * INTEGRATE A BOOL IN THE MERGED LOG MANAGER TO KEEP TRACK IF EVERY
 * LOG HAS BEEN OCCUPIED!
 */

#ifndef EEPROM_Logger_h
#define EEPROM_Logger_h

#include "Arduino.h"
#include <EEPROM_Counter.h>

static EEPROM_Counter eepromCounter;

class EEPROM_Logger {

public:

  struct LogStruct {
    long logCycleNumber;
    long logCycleTime;
    byte logErrorCode;
  };
  LogStruct newLog;

  // FUNTIONS:
  EEPROM_Logger();
  void writeLog(long cycleNumber, long logTime, byte errorCode);
  LogStruct readLog(int logNumber);
  void setup(int eepromMinAddress, int eepromMaxAddress, int numberOfLogEntries);
  void setAllZero();
  void printAllLogs();

  // VARIABLES:

private:
  // FUNCTIONS:
  void switchToNextLog();
  int calculateCurrentCounterNumber(int currentLogNumber);
  int getNoOfNextLog();
  long getNoOfFirstLog();
  bool checkEntryInLastSlot();
  //******************************************************************************
  // MERGE FUNCTIONS:
  //******************************************************************************
  long mergeTimeAndErrorCode(long logTime, byte errorCode);
  long mergeLogManger(int noOfFirstLog, int noOfCurrentLog);
  //******************************************************************************
  // UNMERGE FUNCTIONS:
  //******************************************************************************
  byte unmergeErrorCode(long mergedTimeAndCode);
  long unmergeTime(long mergedTimeAndCode);
  int unmergeNoOfFirstLog(long mergedLogManager);
  int unmergeNoOfCurrentLog(long mergedLogManager);
  //******************************************************************************

  // VARIABLES:
  int _numberOfLogEntries = 0;
  int _maxLogNumber = 0;
  int _addressOfLogManager = 0;
  byte _longForSlotManager = 1;
  bool _everyLogOccupied = false;
};
#endif
