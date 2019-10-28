#include <EEPROM_Logger.h> //https://github.com/chischte/eeprom-logger-library.git
//#include <EEPROM_Counter.h> //https://github.com/chischte/eeprom-counter-library.git
//// NAME THE VALUES TO BE STORED ON EEPROM BY ENTERING THEM IN THE LIST BELOW:
enum logger {
  toolResetError,    // example value name
  timeoutError   // example value name
};

//
//// DEFINE THE EEPROM SIZE OF YOUR BOARD:
//// EEPROM size Arduino Nano/Uno: 1024 bytes (ATmega328P) -> maxAddress = 1023
//// EEPROM size Arduino Mega:     4096 bytes (ATmega2560) -> maxAddress = 4095
//
//// IT IS POSSIBLE TO USE ONLY A RANGE OF THE EEPROM
//// THIS CAN BE NECESSARY IF MORE THAN ONE FUNCTION REQUIRES ACCES TO A PART OF THE EEPROM
//// THE ASSIGNED SIZE HAS TO BE AT LEAST ((numberOfValues*8)+10)bytes
//

int eepromMinAddress = 200; // has to be 0 or bigger
int eepromMaxAddress = 300; // has to be at least one smaller than the EEPROM size of the processor used
int numberOfErrorLogs = 10;
//
//// CREATE AN INSTANCE OF THE LIBRARY CLASS:
EEPROM_Logger errorLogger;

/*
 long mergeLong(long ErrorTime, byte errorCode){

 long splitLongErrorTime(long mergedLong){
 return errorTime;

 byte splitLongErrorCode(long mergedLong) {
 return errorCode;
 */

void printLogEntry(int firstLog, int lastLog) {
  for (int i = firstLog; i < lastLog; i++) {

    EEPROM_Logger::LogStruct structFromFunction;
    structFromFunction = errorLogger.readLog(i);
    String errorCode[] = { "reset", "timeout" };
    Serial.print("Zaehlerstand: ");
    Serial.print(structFromFunction.logCycleNumber);
    Serial.print("  Zeit: ");
    Serial.print(structFromFunction.logCycleTime);
    Serial.print("min ");
    Serial.print("  Fehler:");
    Serial.println(errorCode[structFromFunction.logErrorCode]);
  }
  Serial.println(" ");
}

void setup() {
  errorLogger.setup(eepromMinAddress, eepromMaxAddress, numberOfErrorLogs);
  Serial.begin(115200);
  Serial.println("EXIT SETUP");
}

void loop() {

  errorLogger.setAllZero();

  // GENERATE VALUES FOR THE LOGGER:
  static int cycleNumber = 0;
  cycleNumber++;

  static int logTime = 0;
  logTime = logTime + 5;

  static byte errorCode = 0;
  if (errorCode == 0) {
    errorCode = 1;
  } else {
    errorCode = 0;
  }

  // WRITE LOG:
  errorLogger.writeLog(cycleNumber, logTime, errorCode);

  // PRINT OUT ALL LOGS:
  printLogEntry(0, (numberOfErrorLogs - 1));
  //printLogEntry(0, 1);

  delay(5000);
}
