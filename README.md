# eeprom-logger-library 

**TODO:**

**UPDATE README WITH LIBRARY FUNCTIONS**

**UPDATE README WITH ERROR CODE NAMES (e.g. 00=emptyLog 01=timeout...)

**UPDATE PRINT FUNCTION WITH min/sec instead of only min**


**Library to store logs on the non volatile Arduino EEPROM memory.**

![alt text](documents/GraphicalOverview.jpg)

The library was written to log errors of a Controllino PLC controlled test rig.

The "eeprom-logger-library" makes use of the "eeprom-counter-library" to handle the eeprom read/write commands and to take care that the memory has an evenly wear off and therefore a longer lifetime.
Additional information regarding the eeprom-counter-library can be found here:
https://github.com/chischte/eeprom-counter-library.git

**The Information stored with the eeprom-error function are:** 
errorCode  ...what error did happen
 
cycleNo    ...at what tool cycle did the error happen? 

errorTime  ...at what time since the last reset did the error happen 
 
**Memory usage** 
Each error entry takes up 2 long values = 8 bytes. 
 
The 1nd long (4bytes) stores the cycle number. 

The 2nd long is split into 1 byte for the Error Code and 3 bytes for the time.

An Additional long is used to store which slot will be written next and which slot stores the oldest log. 

6 additional bytes are used by the counter-library

**Memory requirements**

Therefore to store for example 100 error logs, a total of 810 bytes is required: 

	100 x 8bytes + 2 bytes      + 2 bytes     + 6 bytes 
	log entries  + noOfFirstLog + noOfNextLog + required by the counter-library
	
**Value Ranges** 

error code:   255 possible codes (1-255), code 0 is reserved to mark empty logs

error time:   about 49 days if interpreted as seconds (millis overflow), about 4.7 hours if interpreted as milliseconds 

cycle number: about 2 billion cycles (signed long) 
 

Library Functions
-----------------

	// CREATE AN INSTANCE OF THE LIBRARY CLASS:
	EEPROM_Logger exampleErrorLogger(int eepromMinAddress, int eepromMaxAddress, int numberOfValues); 

 **An example of how the functions can be used can be found in the example code.**	
  

Installation
------------
The Library can be installed by cloning or downloading it to Arduinos default library location, e.g. user/documents/Arduino/libraries/.

The eeprom-counter library (https://github.com/chischte/eeprom-counter-library.git) has to be installed as well.
