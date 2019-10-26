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


EEPROM_Logger::EEPROM_Logger(int eepromMinAddress, int eepromMaxAddress, int numberOfLogEntries) {
  //EVERY LOG ENTRY REQUIRES TWO LONG VARIABLES, AN ADDITIONAL LONG IS USED FOR THE SLOT MANAGER
  _eepromNumberOfLongs = numberOfLogEntries * 2 + _oneForTheSlotManager;
  _eepromMinAddress=eepromMinAddress;
  _eepromMaxAddress=eepromMaxAddress;
}

// ALLENFALLS GLEICHE FUNKTION MIT BOOL-WRITE-FLAG BEIM AUFRUF DER FUNKTION
// NOTFALLS EINE INSTANZ FÜR READ-FUNCTION UND EINE FÜR WRITE-FUNCTION
// VERHALTEN BEI  VERSCHIEBEN DES SPEICHERPLATZES?

long EEPROM_Logger::eepromReadAndWrite(long value, long slot){
static EEPROM_Counter eepromCounter( _eepromMinAddress, _eepromMaxAddress, _eepromNumberOfLongs);
eepromCounter.countOneUp(1);
}

void EEPROM_Logger::storeLog(long cycleNumber, long cycleTime, byte ErrorCode) {

  // STORE CYCLE NUMBER TO CURRENT SLOT:
//eepro
// MERGE CYCLE TIME AND ERROR CODE:

// STORE MERGED VALUES TO CURRENT SLOT:
}

EEPROM_Logger::LogStruct EEPROM_Logger::functionReturningStruct() {
  newLog.logErrorCode = 100;
  newLog.logCycleNumber = 4321;
  newLog.logCycleTime = 44;

  return newLog;
}
