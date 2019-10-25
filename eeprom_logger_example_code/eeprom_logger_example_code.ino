#include <EEPROM_Logger.h> //https://github.com/chischte/eeprom-logger-library.git
//#include <EEPROM_Counter.h> //https://github.com/chischte/eeprom-counter-library.git
//// NAME THE VALUES TO BE STORED ON EEPROM BY ENTERING THEM IN THE LIST BELOW:
enum logger {
toolResetError,    // example value name
timeoutError,   // example value name
 endOfEnum           // add additional values before this one...
                      // ...this has to be the last one!
};
int numberOfValues = endOfEnum;
//
//// DEFINE THE EEPROM SIZE OF YOUR BOARD:
//// EEPROM size Arduino Nano/Uno: 1024 bytes (ATmega328P) -> maxAddress = 1023
//// EEPROM size Arduino Mega:     4096 bytes (ATmega2560) -> maxAddress = 4095
//
//// IT IS POSSIBLE TO USE ONLY A RANGE OF THE EEPROM
//// THIS CAN BE NECESSARY IF MORE THAN ONE FUNCTION REQUIRES ACCES TO A PART OF THE EEPROM
//// THE ASSIGNED SIZE HAS TO BE AT LEAST (numberOfValues*4+10)bytes
//
int eepromMinAddress = 200; // has to be 0 or bigger
int eepromMaxAddress = 400; // has to be at least one smaller than the EEPROM size of the processor used
//
//// CREATE AN INSTANCE OF THE LIBRARY CLASS:
EEPROM_Logger errorLogger(eepromMinAddress, eepromMaxAddress, numberOfValues);
void setup() {
  Serial.begin(115200);
  Serial.println("EXIT SETUP");
}

void loop() {

//  //exampleCounter.setAllZero(); //sets all values to zero
//
//  for (int i = 0; i < 5; i++) {
//    exampleCounter.countOneUp(longTimeCounter);
//  }
//  exampleCounter.countOneUp(shortTimeCounter);
//
//  exampleCounter.countOneUp(toolIdentNumber);
//
//  exampleCounter.set(somethingElse, 112233);
//
//  long ValueFromGetValue = exampleCounter.getValue(longTimeCounter);
//  Serial.print("Value from getValue Function: ");
//  Serial.println(ValueFromGetValue);
//  exampleCounter.printDebugInformation();
//  delay(1000);

}
