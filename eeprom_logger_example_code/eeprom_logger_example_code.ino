#include <EEPROM_Logger.h> //https://github.com/chischte/eeprom-logger-library.git

// NAME THE VALUES TO BE STORED ON EEPROM BY ENTERING THEM IN THE LIST BELOW:
enum logger {
  emptyLog,        // marks empty logs
  toolResetError,  // example value name
  timeoutError     // example value name
};

// DEFINE THE EEPROM SIZE OF YOUR BOARD:
// EEPROM size Arduino Nano/Uno: 1024 bytes (ATmega328P) -> maxAddress = 1023
// EEPROM size Arduino Mega:     4096 bytes (ATmega2560) -> maxAddress = 4095
// IT IS POSSIBLE TO USE ONLY A RANGE OF THE EEPROM
// THE ASSIGNED SIZE HAS TO BE AT LEAST ((numberOfValues*8)+10)bytes, THE MORE, THE BETTER
int eepromMinAddress = 0; // has to be 0 or bigger
int eepromMaxAddress = 1023; // has to be at least one smaller than the EEPROM size of the processor used
int numberOfErrorLogs = 5;

// CREATE AN INSTANCE OF THE LIBRARY CLASS:
EEPROM_Logger errorLogger;

void printLogEntry(int logNumber) {
  EEPROM_Logger::LogStruct structFromFunction;
  structFromFunction = errorLogger.readLog(logNumber);
  String errorCode[] = { "n.a.", "reset", "timeout" };
  Serial.print("Zaehlerstand: ");
  Serial.print(structFromFunction.logCycleNumber);
  Serial.print("  Zeit: ");
  Serial.print(structFromFunction.logCycleTime);
  Serial.print("min ");
  Serial.print(" Fehler: ");
  Serial.println(errorCode[structFromFunction.logErrorCode]);
}

void setup() {
  errorLogger.setup(eepromMinAddress, eepromMaxAddress, numberOfErrorLogs);
  Serial.begin(115200);
  Serial.println("EXIT SETUP");
  // ACTIVATE THIS FUNCTION TO CLEAR THE LOG MEMORY:
  //errorLogger.setAllZero();

}

void loop() {

  // GENERATE VALUES FOR THE LOGGER:
  static long cycleNumber = 500000;
  cycleNumber++;

  static long logTime = 5;
  logTime = logTime + 1;

  static byte errorCode = 1;
  if (errorCode == 1) {
    errorCode = 2;
  } else {
    errorCode = 1;
  }

  // WRITE LOG:
  errorLogger.writeLog(cycleNumber, logTime, errorCode);

  // PRINT A LOG ENTRY:
  int logNumber = 3;
  printLogEntry(logNumber);
  Serial.println();
  // PRINT ALL LOG ENTRIES:
  errorLogger.printAllLogs();

  delay(2000);
}
