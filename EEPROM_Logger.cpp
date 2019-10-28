/*
 * *****************************************************************************
 * EEPROM_Logger.cpp
 * Library to store values on the non volatile arduino EEPROM
 * Michael Wettstein
 * October 2019, Zürich
 * *****************************************************************************
 */

#include "EEPROM_Logger.h"
#include "Arduino.h"

EEPROM_Logger::EEPROM_Logger() {
}

void EEPROM_Logger::setup(int eepromMinAddress, int eepromMaxAddress, int numberOfLogEntries) {

  // EVERY LOG ENTRY REQUIRES TWO LONG VARIABLES, AN ADDITIONAL LONG IS USED FOR THE SLOT MANAGER
  _numberOfLogEntries = numberOfLogEntries;
  _eepromNumberOfLongs = numberOfLogEntries * 2 + _oneForTheSlotManager;
  eepromCounter.setup(eepromMinAddress, eepromMaxAddress, _eepromNumberOfLongs);

  // GET THE mergedLogManager-long:
  long mergedLogManager = eepromCounter.getValue(_addressOfSlotManager);
  int noOfFirstLog = unmergeNoOfFirstLog(mergedLogManager);
  int noOfCurrentLog = unmergeNoOfCurrentLog(mergedLogManager);

  // CHECK IF LOG MANAGER CONTAINS VALID VALUES:
  bool outOfValidRange = 0;
  if (noOfFirstLog < 0 || noOfFirstLog > (_numberOfLogEntries - 1)) {
    outOfValidRange = true;
  }
  if (noOfCurrentLog < 0 || noOfCurrentLog > (_numberOfLogEntries)) {
    outOfValidRange = true;
  }
  // IF NOT (NEW SETUP OR NEW BOARD) CLEAR THE WHOLE MEMORY:
  if (outOfValidRange) {
    eepromCounter.setAllZero();
  }
}

void EEPROM_Logger::writeLog(long cycleNumber, long cycleTime, byte errorCode) {

  long mergedLogManager = eepromCounter.getValue(_addressOfSlotManager);
  int firstLog = 1;
  int currentLogSlot = mergedLogManager;



// STORE CYCLE NUMBER TO CURRENT SLOT:
  eepromCounter.set(_currentLogSlot, cycleNumber);
  _currentLogSlot++;

// STORE MERGED VALUES TO CURRENT SLOT:
  eepromCounter.set(_currentLogSlot, mergedTimeAndCode);
  _currentLogSlot++;
  eepromCounter.set(_addressOfSlotManager, _currentLogSlot);

}


//******************************************************************************
// MERGE FUNCTIONS:
//******************************************************************************
long EEPROM_Logger::mergeTimeAndErrorCode(long logTime, byte errorCode) {
  long mergedTimeAndCode = ((logTime << 8) | errorCode);
  return mergedTimeAndCode;
}

long EEPROM_Logger::mergeLogManger(int noOfFirstLog, int noOfCurrentLog) {
  long mergedLogManager = noOfFirstLog << 16 | noOfCurrentLog;
  return mergedLogManager;
}

//******************************************************************************
// UNMERGE FUNCTIONS:
//******************************************************************************
byte EEPROM_Logger::unmergeErrorCode(long mergedTimeAndCode) {
  byte errorCode = mergedTimeAndCode;
  return errorCode;
}

long EEPROM_Logger::unmergeTime(long mergedTimeAndCode) {
  long logTime = mergedTimeAndCode >> 8;
  return logTime;
}

int EEPROM_Logger::unmergeNoOfFirstLog(long mergedLogManager) {
  int noOfFirstLog = mergedLogManager >> 16;
  return noOfFirstLog;
}

int EEPROM_Logger::unmergeNoOfCurrentLog(long mergedLogManager) {
  int noOfCurrentLog = mergedLogManager;
  return noOfCurrentLog;
}
//******************************************************************************

EEPROM_Logger::LogStruct EEPROM_Logger::readLog(int logNumber) {

  long mergedLogManager = eepromCounter.getValue(_addressOfSlotManager);
  int firstLog = mergedLogManager >> 16;
  logNumber--; //zero indexed;
  int currentReadSlot = firstLog + (logNumber) * 2;
  long cycleNumber = eepromCounter.getValue(currentReadSlot);
  currentReadSlot++;
  long mergedTimeAndCode = eepromCounter.getValue(currentReadSlot);
  long cycleTime = mergedTimeAndCode >> 8;
  byte errorCode = mergedTimeAndCode;

  newLog.logCycleNumber = cycleNumber;
  newLog.logCycleTime = cycleTime;
  newLog.logErrorCode = errorCode;

//eepromCounter.printDebugInformation();
  return newLog;
}
