
#include <Wire.h>
#include <DS3231.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>


DS3231 clock;             // Create an object to drive the clock
RTCDateTime dt;           // Create an object to get the date
Adafruit_BME280 bme;      // Create an object to drive the temperature, humidity and pressure
const int chipSelect = 4; // 

int sec = 1000;             // 1s equal to 1000ms
int nextime = 10 * sec;     // Calculate the delay between to 2 measures.
float calibrateC = -1.00;   // Adjustment of the temperature 
char t[32];                 // Use with sprintf to format the date

void setup()
{
  Serial.begin(9600);
  initSD();
  initClock();
  initBme();
  

  
}

void loop()
{
  dt = clock.getDateTime();
  sprintf(t, "%02d%02d%02d-%02d%02d%02d",  dt.year, dt.month, dt.day, dt.hour, dt.minute, dt.second);
  // For leading zero look to DS3231_dateformat example

  writeSerial();
  writeSD();

  delay(nextime);
}
void initClock() {
   // Initialize DS3231
  Serial.println(F("Initialize DS3231"));
  if (!clock.begin()) {
    Serial.println(F("clock failed!"));
    while(1);
  }
  Serial.println(F("Initialize DS3231 done"));
}
void initBme() {
  Serial.println(F("Initialize bme280"));
  if (!bme.begin(0x76)) {
   Serial.println(F("Could not find a valid BME280 sensor, check wiring!"));
   while (1);
  }
  Serial.println("Initialize bme280 done");
  // Set sketch compiling time
  clock.setDateTime(__DATE__, __TIME__);
  
}

void initSD() {
  Serial.println(F("Initialize SDcard"));
 
  
  if (!SD.begin(chipSelect)) {
   Serial.println(F("initialization failed!"));
   while(1);
  }
  Serial.println(F("Initialize SDcard done"));
  
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  if(dataFile) {
    dataFile.println("date;temp;hygro;pression");
    dataFile.close();
     Serial.println(F("Initialize file SDcard done"));
  }
  else {
     Serial.println(F("Initialize file SDcard failed!"));
      Serial.println(dataFile);
  }
}

void writeSD() {
   File dataFile = SD.open("datalog.txt", FILE_WRITE);
  if(dataFile) {
     dataFile.print(t);
     dataFile.print(";");
     dataFile.print((bme.readTemperature()+ calibrateC));
     dataFile.print(";");
     dataFile.print(bme.readHumidity());
     dataFile.print(";");
     dataFile.println(bme.readPressure() / 100.0F);
     dataFile.close();
  }
  else {
    Serial.println(F("SD card writing failed"));
  }
  
}

void writeSerial() {
     Serial.print(t);
     Serial.print(";");
     Serial.print((bme.readTemperature()+ calibrateC));
     Serial.print(";");
     Serial.print(bme.readHumidity());
     Serial.print(";");
     Serial.println(bme.readPressure() / 100.0F);   
}
