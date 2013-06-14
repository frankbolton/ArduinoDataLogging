ArduinoDataLogging
==================

arduino + networking + sensing

First off... connect a DS18B20 to the Arduino board.
Important things to remember for this circuit are that you need to get the 
Vcc = 5v (I pulled it from a 5v pin on the headers)
Gnc (also pulled from the headers)
Signal = pin 3. This was setup in the arduino code 
	// Data wire is plugged into pin 3 on the Arduino
	#define ONE_WIRE_BUS 3

electrical connection also needs to be done thoughtfully. You need a pull-up resistor. I used a 4.7k resistor. Pull up means it's connected between the 5V and the data line, close to the arduino.

To use this you also need to get the following libraries:
#include <SD.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include <Ethernet.h>
