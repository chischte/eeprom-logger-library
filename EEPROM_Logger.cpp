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
  _numberOfLogEntries = numberOfLogEntries;
  byte longSizeOfAnEntry = 2;

  // EVERY LOG ENTRY REQUIRES TWO LONG VARIABLES, AN ADDITIONAL LONG IS USED FOR THE SLOT MANAGER
  int eepromNumberOfLongs = _numberOfLogEntries * longSizeOfAnEntry + _longForSlotManager;

  //SET UP THE EEPROM COUNTER:
  eepromCounter.setup(eepromMinAddress, eepromMaxAddress, eepromNumberOfLongs);

  // GET THE mergedLogManager-long:
  long mergedLogManager = eepromCounter.getValue(_addressOfLogManager);
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

  // GET THE mergedLogManager-long:
  mergedLogManager = eepromCounter.getValue(_addressOfLogManager);
  noOfFirstLog = unmergeNoOfFirstLog(mergedLogManager);
  noOfCurrentLog = unmergeNoOfCurrentLog(mergedLogManager);

  // IF THE FIRST LOG IS NOT LOG 0, EVERY LOG HAS BEEN OCCUPIED:

  // ...IS THIS ALWAYS CORRECT?????

  if (noOfCurrentLog != 0) {
    _everyLogOccupied = true;
  }

}

int EEPROM_Logger::getCurrentLogNumber() {
  long mergedLogManager = eepromCounter.getValue(_addressOfLogManager);
  int currentLogNumber = unmergeNoOfCurrentLog(mergedLogManager);
  return currentLogNumber;
}

int EEPROM_Logger::calculateCurrentCounterNumber(int currentLogNumber) {
  byte numberOfLongsPerLog = 2;
  byte sizeOfTheLogManager = 1;
  int currentCounterNumber = currentLogNumber * numberOfLongsPerLog + sizeOfTheLogManager;
  return currentCounterNumber;
}

void EEPROM_Logger::switchToNextLog() {
  long mergedLogManager = eepromCounter.getValue(_addressOfLogManager);
  int noOfFirstLog = unmergeNoOfFirstLog(mergedLogManager);
  int noOfCurrentLog = unmergeNoOfCurrentLog(mergedLogManager);
  noOfCurrentLog++;
  if (noOfCurrentLog > (_numberOfLogEntries - 1)) {
    noOfCurrentLog = 0;
    _everyLogOccupied = true;
  }
  if (_everyLogOccupied == true) {
    noOfFirstLog = noOfCurrentLog + 1;
  }

  mergedLogManager = mergeLogManger(noOfFirstLog, noOfCurrentLog);
  eepromCounter.set(_addressOfLogManager, mergedLogManager);
}

void EEPROM_Logger::writeLog(long cycleNumber, long logTime, byte errorCode) {

  // CALCULATE CURRENT COUNTER ADDRESS:
  int currentCounterAddress = calculateCurrentCounterNumber(getCurrentLogNumber());

  // STORE CYCLE NUMBER TO CURRENT SLOT:
  eepromCounter.set(currentCounterAddress, cycleNumber);
  currentCounterAddress++;

  // STORE MERGED VALUES TO CURRENT SLOT:
  eepromCounter.set(currentCounterAddress, mergeTimeAndErrorCode(logTime, errorCode));

  switchToNextLog();
}

EEPROM_Logger::LogStruct EEPROM_Logger::readLog(int logNumber) {

  // CALCULATE COUNTER ADDRESS:
  int currentCounterAddress = calculateCurrentCounterNumber(logNumber);

  // GET CYCLE NUMBER:
  int cycleNumber = eepromCounter.getValue(logNumber);
  currentCounterAddress++;

  // GET MERGED TIME AND ERROR CODE LONG:
  long mergedTimeAndCode = eepromCounter.getValue(currentCounterAddress);

  // UNMERGE THE VALUES:
  int cycleTime = unmergeTime(mergedTimeAndCode);
  int errorCode = unmergeErrorCode(mergedTimeAndCode);

  // ASSIGN THE VALUES TO THE LOG STRUCT:
  newLog.logCycleNumber = cycleNumber;
  newLog.logCycleTime = cycleTime;
  newLog.logErrorCode = errorCode;

  //eepromCounter.printDebugInformation();

  // RETURN THE VALUES:
  return newLog;
}

//******************************************************************************
// MERGE FUNCTIONS:
//******************************************************************************
long EEPROM_Logger::mergeTimeAndErrorCode(long logTime, byte errorCode) {
  long mergedTimeAndCode = ((logTime << 8) | errorCode);
  return mergedTimeAndCode;
}

long EEPROM_Logger::mergeLogManger(int noOfFirstLog, int noOfCurrentLog) {
  long mergedLogManager = noOfFirstLog;
  mergedLogManager = mergedLogManager << 16 | noOfCurrentLog;
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

void EEPROM_Logger::setAllZero() {
  eepromCounter.setAllZero();
  _everyLogOccupied = false;
}
