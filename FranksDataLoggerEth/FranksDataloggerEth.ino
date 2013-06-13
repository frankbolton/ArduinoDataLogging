/*
  SD card datalogger
 
 This example shows how to log data from three analog sensors 
 to an SD card using the SD library.
 	
 The circuit:
 * analog sensors on analog ins 0, 1, and 2
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4
 
 created  24 Nov 2010
 updated 2 Dec 2010
 by Tom Igoe
 
 This example code is in the public domain.
 	 
 */

//temperature probe id 
//0x28, 0x54, 0xCD, 0x00, 0x04, 0x00, 0x00, 0xBC

#include <SD.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {  
  0x90, 0xA2, 0xDA, 0x00, 0xFA, 0x54 };
char serverName [] = "www.google.com";
char formkey [] = "dDFDLWtuSk03MFZteTZESjAwR2dpb2c6MQ";

// Initialize the Ethernet client library
EthernetClient client;

// Data wire is plugged into pin 3 on the Arduino
#define ONE_WIRE_BUS 3

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

DeviceAddress firstThermometer = {  
  0x28, 0x54, 0xCD, 0x00, 0x04, 0x00, 0x00, 0xBC };

// On the Ethernet Shield, CS is pin 4. Note that even if it's not
// used as the CS pin, the hardware CS pin (10 on most Arduino boards,
// 53 on the Mega) must be left as an output or the SD library
// functions will not work.
const int chipSelect = 4;

// Set up variables that are used for storing states in the periodic 
// data recording
long previousMillis = 0;
long interval = 20;           //number of seconds between recordings


void setup()
{
  Serial.begin(9600);
  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;


  }
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    delay(3000);
    // no point in carrying on, so do nothing forevermore:
    for(;;)
      ;  
  }  
  Serial.println("card initialized.");
  ///sensors.begin();
  // set the resolution to 10 bit (good enough?)
  sensors.setResolution(firstThermometer, 10);
  Serial.println("thermometer initialized");

  Serial.print("My IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print("."); 
  }
  Serial.println();

}


void loop()
{
  //String dataString;
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval*1000) {
    // save the last time you updated saved and sent data
    previousMillis = currentMillis;   

    Serial.print("Getting temperatures...\n\r");
    sensors.requestTemperatures();

    // make a string for assembling the data to log:
  ///  dataString = String(previousMillis/1000) + ",";

    int tempC_dec = 100*sensors.getTempC(firstThermometer);
    int tempC_int = sensors.getTempC(firstThermometer);
    String tempString = String(tempC_int)+"."+String(tempC_dec-100*tempC_int);
    Serial.println("Temperature= "+tempString);
 ///   dataString += tempString;
 ///   File dataFile = SD.open("temperatures.txt", FILE_WRITE);
    // if the file is available, write to it:
    ///if (dataFile) {
      ///dataFile.println(dataString);
      ///dataFile.close();
      // print to the serial port too:
     /// Serial.println(dataString);
   /// }  
    // if the file isn't open, pop up an error:
    ///else {
     /// Serial.println("error opening datalog.txt");
    ///} 

    String data;
    data+="entry.0.single=";
    data+=tempString;
    data+="&entry.2.single=";
    data+=String(currentMillis/1000);
    data+="&submit=Submit";


    if (client.connect(serverName, 80)) {
      Serial.println("connected");
      client.print("POST /formResponse?formkey=");
      client.print(formkey);
      client.println("&ifq HTTP/1.1");
      client.println("Host: spreadsheets.google.com");
      client.println("Content-Type: application/x-www-form-urlencoded");
      client.println("Connection: close");
      client.print("Content-Length: ");
      client.println(data.length());
      client.println();
      client.print(data);
      client.println();
      Serial.print("POST /formResponse?formkey=");
      Serial.print(formkey);
      Serial.println("&ifq HTTP/1.1");
      Serial.println("Host: spreadsheets.google.com");
      Serial.println("Content-Type: application/x-www-form-urlencoded");
      Serial.println("Connection: close");
      Serial.print("Content-Length: ");
      Serial.println(data.length());
      Serial.println();
      Serial.print(data);
      Serial.println();
      client.stop();
    }

    if (!client.connected()) {
      Serial.println();
      Serial.println("disconnecting.");
      client.stop();
    }
  }



}

















