///*
// * *****************************************************************************
// EEPROM_Logger.cpp
// Library to store values on the non volatile arduino EEPROM
// Michael Wettstein
// September 2019, Zürich
// * *****************************************************************************
// */

#include "EEPROM_Logger.h"
#include "Arduino.h"

EEPROM_Logger::EEPROM_Logger(int eepromMinAddress, int eepromMaxAddress, int numberOfValues) {
EEPROM_Counter eepromCounter(eepromMinAddress,eepromMaxAddress,numberOfValues);
}

EEPROM_Logger::LogStruct EEPROM_Logger::FunctionReturningStruct()
{
    newLog.logErrorCode = 100;
    return newLog;
}
