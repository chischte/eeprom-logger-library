/*
 * *****************************************************************************
 EEPROM_Counter.cpp
 Library to store values on the non volatile arduino EEPROM
 Michael Wettstein
 September 2019, Zürich
 * *****************************************************************************
 */

#include "EEPROM_Logger.h"
#include "Arduino.h"

EEPROM_Logger::EEPROM_Logger(int eepromMinAddress, int eepromMaxAddress, int numberOfValues) {
EEPROM_Counter eepromCounter(eepromMinAddress,eepromMaxAddress,numberOfValues);
}

struct LogStruct
{
  long logCycleNumber;
  long logCycleTime;
  byte logErrorCode;
};

//LogStruct newStruct;

struct LogStruct EEPROM_Logger::FunctionReturningStruct() {

  LogStruct currentLog;
  currentLog.logCycleNumber = 77;
  currentLog.logCycleTime = 88;
  currentLog.logErrorCode = 99;

  return currentLog;

}


/*
EEPROM_Logger::LogStruct currentLogData::getLogData(int logNumber){

struct LogStruct {
  long logCycleNumber;
  long logCycleTime;
  byte logErrorCode;
};


LogStruct currentLogData;

currentLogData.logCycleNumber=loescher;
currentLogData.logCycleTime=44;
currentLogData.logErrorCode=111;


  return currentLogData;
}
*/
