/***************************************************************************
  This is a library for the BME280 humidity, temperature & pressure sensor

  Designed specifically to work with the Adafruit BME280 Breakout
  ----> http://www.adafruit.com/products/2650

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required
  to interface. The device's I2C address is either 0x76 or 0x77.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
  See the LICENSE file for details.

  Edited by Justin Dyal to work specifically for the BME280 and the Feather M0 with winc1500 WiFi
  Specific Adafruit hardware needed:
  ----> https://www.adafruit.com/product/2652
  ----> https://www.adafruit.com/product/3010

  I2C Physical Pin Connections(Feather M0 WiFi to BME280):
  3V > Vin
  GND > GND
  SCL > SCK 
  
  
 ***************************************************************************/

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>


#define SEALEVELPRESSURE_HPA (1013.25)

//I2C call
Adafruit_BME280 bme;


unsigned long delayTime;

void setup() {
    Serial.begin(9600);
    while(!Serial);    // time to get serial running
    Serial.println(F("Connecting to BME280..."));

    unsigned status;
    
    // default settings
    status = bme.begin();  
    // You can also pass in a Wire library object like &Wire2
    // status = bme.begin(0x76, &Wire2)
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
        Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
        Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
        Serial.print("        ID of 0x60 represents a BME 280.\n");
        Serial.print("        ID of 0x61 represents a BME 680.\n");
        while (1) delay(10);
    }
    
    Serial.println("Serial Monitor Output for BME280 on Feather M0 WiFi");
    delayTime = 30000;

    Serial.println();
}


void loop() { 
    printValues();
    delay(delayTime);
}


void printValues() {

    //Outputs temperature reading to serial monitor
    Serial.print("Temperature = ");
    Serial.print(((bme.readTemperature())*9/5)+32);
    Serial.println(" *F");

    //Outputs pressure reading to serial monitor
    Serial.print("Pressure = ");
    Serial.print(bme.readPressure() / 100.0F);
    Serial.println(" hPa");

    //Outputs estimated altitude reading to serial monitor
    Serial.print("Approx. Altitude = ");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println(" m");

    //Outputs relative humidity reading to serial monitor
    Serial.print("Humidity = ");
    Serial.print(bme.readHumidity());
    Serial.println(" %");

    Serial.println();
}
