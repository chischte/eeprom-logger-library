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
//EVERY LOG ENTRY REQUIRES TWO LONG VARIABLES, AN ADDITIONAL LONG IS USED FOR THE SLOT MANAGER
  _eepromNumberOfLongs = numberOfLogEntries * 2 + _oneForTheSlotManager;
  eepromCounter.setup(eepromMinAddress, eepromMaxAddress, _eepromNumberOfLongs);
}

void EEPROM_Logger::writeLog(long cycleNumber, long cycleTime, byte errorCode) {

  long slotManagerMergedLong = eepromCounter.getValue(_addressOfSlotManager);
  int firstLog = 1;
//implement: firstLog=slotManager>>16;
  int _currentLogSlot = slotManagerMergedLong;

  // STORE CYCLE NUMBER TO CURRENT SLOT:
  eepromCounter.set(_currentLogSlot, cycleNumber);
  _currentLogSlot++;

  // MERGE CYCLE TIME AND ERROR CODE:
  long mergedTimeAndCode = ((cycleTime << 8) | errorCode);

  // STORE MERGED VALUES TO CURRENT SLOT:
  eepromCounter.set(_currentLogSlot, mergedTimeAndCode);
  _currentLogSlot++;
  eepromCounter.set(_addressOfSlotManager, _currentLogSlot);

}

EEPROM_Logger::LogStruct EEPROM_Logger::readLog(int logNumber) {

  long slotManagerMergedLong = eepromCounter.getValue(_addressOfSlotManager);
  int firstLog = slotManagerMergedLong >> 16;
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
