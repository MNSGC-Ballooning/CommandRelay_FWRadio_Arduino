// Include libraries by going to sketch -> Include Library -> SD, SoftwareSerial
#include <SoftwareSerial.h>
#include <SD.h>

/* Author: Kate Kwiecinski
 * Last Update: 5/31/17
 * Arduino Mega
 * Purpose: Communicate with ground transceivers (Freewave slaves)
 *          Relay commands received using XBee
 *          Relay GPS signals
 *          Log data and commands 
 *          Be simpler than Raspberry Pi
 */

// name of initial SD file
// Format: “NAME001.EXT”
string logFileName = "logFile.txt";

//GPS and XBee pins
const uint8_t GPS_Rx = 4;
const uint8_t GPS_Tx = 3;
const uint8_t XB_Rx = 2;
const uint8_t XB_Tx = 3;

// Runs only once at beginning:
void setup() {
  // Set up XBee as serial
  SoftwareSerial(XBee(XB_Rx, XB_Tx));

  // Set up generic GPS as serial
  SoftwareSerial(GPS(GPS_Rx, GPS_Tx));
  
  Serial.begin(115200) //FW radio
  XBee.begin(19200) //XBEE
  GPS.begin(9600);  //GPS

  //SD Card Setup
  Serial.println("Initializing SD...");
  if(!SD.begin(4)) { Serial.println("SD initialization failed!"); return; }
  Serial.println("Initialization Successful");
  logFile = SD.open(logFileName, FILE_WRITE);
  if(logFile){
    Serial.print("Begin writing to SD");
    logFile.println(logFileName); logFile.print(": ");
  }
  else{
    Serial.println("Error opening:  "); Serial.print(logFileName);
  }
  unsigned long startTime = micros(); // Start time
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
// Main:
//////////////////////////////////////////////////////////////////////////////////
void loop() {
  //print time since start
  println( "Current time = ", int(micros()-startTime));
 
  // if data is reveived from radio...
  if (Serial.available()>0) {
    int inbyte = Serial.read(); //read the data
    logFile.print("time /t"); logFile.print(int(micros()-startTime)); logFile.print("/t"); //log time of message
    logFile.print("FW received /t"); logFile.print(inbyte, BYTE);logFile.print("/t"); //log FW incoming message to SD card
    int outbyte = XBee.write(inbyte, BYTE); //print data to transmit pin of Xbee
    logFile.print("Xbee sent /t"); logFile.print(outbyte);logFile.print("/t"); //log outgoing Xbee message to SD card
  }
  // if data is received from XBee...
  if (XBee.available()>0) {
    int inbyte = XBee.read(); //read data
    logFile.print("time /t"); logFile.print(int(micros()-startTime)); logFile.print("/t"); //log time of message
    logFile.print("Xbee received /t"); logFile.print(inbyte);logFile.print("/t"); //log incoming Xbee message to SD card
    int outbyte = Serial.write(inbyte);
    logFile.print("FW sent /t"); logFile.print(outbyte);logFile.print("/t"); //log FW outgoing message to SD card
  }
  // if data is received from GPS...
  if (GPS.available()>0) {
    int inbyte = GPS.read(); //read data
    logFile.print("time /t"); logFile.print(int(micros()-startTime)); logFile.print("/t"); //log time of message
    logFile.print("GPS received /t"); logFile.print(inbyte);logFile.print("/t"); //log incoming Xbee message to SD card
    int outbyte = Serial.write(inbyte);
    logFile.print("FW sent /t"); logFile.print(outbyte);logFile.print("/t"); //log FW outgoing message to SD card
  }
  
}

// FUNCTIONS
////////////////////////////////////////////////////////////////////


