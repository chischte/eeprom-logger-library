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
  _maxLogNumber = _numberOfLogEntries - 1;
  byte longSizeOfAnEntry = 2;

  // EVERY LOG ENTRY REQUIRES TWO LONG VARIABLES, AN ADDITIONAL LONG IS USED FOR THE SLOT MANAGER
  int eepromNumberOfLongs = _numberOfLogEntries * longSizeOfAnEntry + _longForSlotManager;

  // SET UP THE EEPROM COUNTER:
  eepromCounter.setup(eepromMinAddress, eepromMaxAddress, eepromNumberOfLongs);

  // GET THE mergedLogManager VALUES:
  int noOfFirstLog = getNoOfFirstLog();
  int noOfCurrentLog = getNoOfNextLog();

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
  // GET THE mergedLogManager VALUES:
  noOfFirstLog = getNoOfFirstLog();
  noOfCurrentLog = getNoOfNextLog();

  _everyLogOccupied = checkEntryInLastSlot();
}

bool EEPROM_Logger::checkEntryInLastSlot() {
  bool entryDetected = 0;
  // IF THE LAST LOG SLOT IS NOT 0, EVERY LOG HAS BEEN OCCUPIED:
  // CALCULATE COUNTER ADDRESS:
  int currentCounterAddress = calculateCurrentCounterNumber(_maxLogNumber);

// GET CYCLE NUMBER:
  long cycleNumber = eepromCounter.getValue(currentCounterAddress);
  currentCounterAddress++;

// GET MERGED TIME AND ERROR CODE LONG:
  long mergedTimeAndCode = eepromCounter.getValue(currentCounterAddress);

  if (cycleNumber != 0 && mergedTimeAndCode != 0) {
    entryDetected = true;
  }
  return entryDetected;
}

int EEPROM_Logger::getNoOfNextLog() {
  long mergedLogManager = eepromCounter.getValue(_addressOfLogManager);
  int currentLogNumber = unmergeNoOfCurrentLog(mergedLogManager);
  return currentLogNumber;
}

long EEPROM_Logger::getNoOfFirstLog() {
  long mergedLogManager = eepromCounter.getValue(_addressOfLogManager);
  int noOfFirstLog = unmergeNoOfFirstLog(mergedLogManager);
  return noOfFirstLog;
}

int EEPROM_Logger::calculateCurrentCounterNumber(int currentLogNumber) {
  byte numberOfLongsPerLog = 2;
  byte sizeOfTheLogManager = 1;
  int currentCounterNumber = currentLogNumber * numberOfLongsPerLog + sizeOfTheLogManager;
  return currentCounterNumber;
}

void EEPROM_Logger::switchToNextLog() {
  int noOfFirstLog = getNoOfFirstLog();
  int noOfNextLog = getNoOfNextLog();
  noOfNextLog++;

  if (noOfNextLog > _maxLogNumber) {
    noOfNextLog = 0;
    _everyLogOccupied = true;
  }
  if (_everyLogOccupied == true) {
    noOfFirstLog = noOfNextLog;
    if (noOfFirstLog > _maxLogNumber) {
      noOfFirstLog = 0;
    }
  }
  long mergedLogManager = mergeLogManger(noOfFirstLog, noOfNextLog);
  eepromCounter.set(_addressOfLogManager, mergedLogManager);
}

void EEPROM_Logger::writeLog(long cycleNumber, long logTime, byte errorCode) {

  // CALCULATE CURRENT COUNTER ADDRESS:
  int currentCounterAddress = calculateCurrentCounterNumber(getNoOfNextLog());

  // STORE CYCLE NUMBER TO CURRENT SLOT:
  eepromCounter.set(currentCounterAddress, cycleNumber);
  currentCounterAddress++;

  // STORE MERGED VALUES TO CURRENT SLOT:
  eepromCounter.set(currentCounterAddress, mergeTimeAndErrorCode(logTime, errorCode));

  switchToNextLog();
}

EEPROM_Logger::LogStruct EEPROM_Logger::readLog(int logNumber) {

  logNumber = getNoOfFirstLog() + logNumber;
  if (logNumber > _maxLogNumber) {
    logNumber -= (_maxLogNumber + 1);
  }

  // CALCULATE COUNTER ADDRESS:
  int currentCounterAddress = calculateCurrentCounterNumber(logNumber);

  // GET CYCLE NUMBER:
  long cycleNumber = eepromCounter.getValue(currentCounterAddress);
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

void EEPROM_Logger::printAllLogs() {
  for (int i = 0; i < _maxLogNumber; i++) {
    LogStruct structFromFunction;
    structFromFunction = readLog(i);
    String errorCode[] = { "n.a.", "reset", "timeout" };
    Serial.print("Zaehlerstand: ");
    Serial.print(structFromFunction.logCycleNumber);
    Serial.print("  Zeit: ");
    Serial.print(structFromFunction.logCycleTime);
    Serial.print("min ");
    Serial.print(" Fehler: ");
    Serial.println(errorCode[structFromFunction.logErrorCode]);
  }
  Serial.println();
}
