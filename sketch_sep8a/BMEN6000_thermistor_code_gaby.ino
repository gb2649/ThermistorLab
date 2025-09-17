/**************************************************************************
Thermistors
BMEN 6000 - Signal Processing for Medical Devices 2025
 **************************************************************************/

// Fill in variable values correctly: 5 pts
// Required libraries

#include <Wire.h>
#include <SPI.h>
#include <SD.h>

// Input parameters

float R1 = 10000;
float R2 = 10000;
float R3 = 10000;
float inVoltage = 3;
float sampRate = 256;

// DAQ variables

int V1;
int V2;

// Temp calculation variables

float Vdiff;
float resistance;
float logR;
float SHa = 1e-3;
float SHb = 1e-5;
float SHc = 1e-6;
float temp;
float Rx;

// Moving average variables

int i;
const int buffsize = 1;
float tempBuff[buffsize];
float sumTemp = 0;
float avgTemp;

// Save data to SD card

const int chipSelect = 4;
File tempRecord;

// Initialize data acqusition and calculations

void setup() {

  Serial.begin(9600);
  while (!Serial);

  //Uncomment to use SD card

  SD.begin(chipSelect);
  if (!SD.begin(chipSelect)) {
    Serial.println("SD not found ");
    return;
  }
  Serial.println("SD found");

  tempRecord = SD.open("tempLog.txt", FILE_WRITE);
  if (tempRecord){
  tempRecord.println("Voltage (1)  Voltage (2)  Voltage (diff)  Resistance  Temp (C)  Average temp (C)");
  tempRecord.close();
  }
  else {
    Serial.println("error opening tempRecord.txt");
    return;
  }

  pinMode(A1, INPUT);
  pinMode(A2, INPUT);

}

void loop() {

  // Read voltage from Wheatstone bridge

  V1 = analogRead(A2);  
  V2 = analogRead(A1);

  // Calculate difference between nodes (equivalent to voltage across bridge)
  // 3 pts
  Vdiff = V1 - V2;

  // Calculate resistance from voltage
  // 7 pts

  Rx = (inVoltage*R2 - (R1 + R2)*Vdiff)/(inVoltage*R1 + (R1+R2)*Vdiff)*R3;


  // Calculate temperature from resistance
  // 10 pts

  temp = 1/(SHa + SHb*log(Rx) + SHc*(pow(log(Rx),3))) - 273;


  // Moving average
  // 10 pts
  avgTemp = 0;

  


  // Output (Serial.print prints to the serial monitor; tempRecord.print prints to the SD card)

  Serial.print(V1);
  Serial.print("\t");
  Serial.print(V2);
  Serial.print("\t");
  Serial.print(Vdiff);
  Serial.print("\t");
  Serial.print(Rx);
  Serial.print("\t");
  Serial.print(temp);
  Serial.print("\t");
  Serial.println(avgTemp);

  // Uncomment to use SD card
  
  tempRecord = SD.open("tempLog.txt", FILE_WRITE);
  
  if (tempRecord) {
    tempRecord.print(V1);
    tempRecord.print("\t");
    tempRecord.print(V2);
    tempRecord.print("\t");
    tempRecord.print(Vdiff);
    tempRecord.print("\t");
    tempRecord.print(Rx);
    tempRecord.print("\t");
    tempRecord.print(temp);
    tempRecord.print("\t");
    tempRecord.println(avgTemp);
    tempRecord.close();
  }
  // else {
  //   Serial.println("error opening tempLog.txt");
  //   return;
  // }
  
  // Implement sampling rate: 5 pts

  delay(1000);  

}





