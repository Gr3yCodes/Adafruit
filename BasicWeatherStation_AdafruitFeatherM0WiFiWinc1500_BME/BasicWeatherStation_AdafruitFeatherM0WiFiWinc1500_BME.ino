// Adafruit IO Temperature & Humidity Example
// Tutorial Link: https://learn.adafruit.com/adafruit-io-basics-temperature-and-humidity
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016-2017 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

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


***************************************************************************
  Edited by Justin Dyal (@GreyCodes on Adafruit Discord) to work specifically for
  the BME280 and the Feather M0 with winc1500 WiFi
  Specific Adafruit hardware needed:
  ----> https://www.adafruit.com/product/2652
  ----> https://www.adafruit.com/product/3010

  adafruit discord: https://discord.gg/adafruit

  I2C Physical Pin Connections(Feather M0 WiFi to BME280):
  3V > Vin
  GND > GND
  SCL > SCK 
  SDA > SDI
  
*/


/************************** Configuration ***********************************

Edit the config.h tab and enter your Adafruit IO credentials and WiFi

For Feather M0 WiFi/winc1500 WiFi be sure you have the adafruit IoT certificate installed on the board.

Guide for getting the certificate is here: 
https://learn.adafruit.com/adafruit-feather-m0-wifi-atwinc1500/updating-ssl-certificates


*******************************************************/


#include "config.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

//I2C BME pin definitions
#define BME_SCK 21
#define BME_MOSI 20


//BME object creation
Adafruit_BME280 bme;

// set up the feeds
AdafruitIO_Feed *temperature = io.feed("temperature");
AdafruitIO_Feed *humidity = io.feed("humidity");
AdafruitIO_Feed *pressure = io.feed("pressure");
AdafruitIO_Feed *rechightemp =  io.feed("rechightemp");
AdafruitIO_Feed *reclowtemp = io.feed("reclowtemp");
AdafruitIO_Feed *rechighpress = io.feed("rechighpress");
AdafruitIO_Feed *reclowpress = io.feed("reclowpress");

//variables for highs/lows
//initial values for lows are to ensure the low values are calculated
double recTempHigh;
double recTempLow = 100.00;
double recPressHigh;
double recPressLow = 2000.00;



void setup() {
  

  /*
   * The below while loop allows the Feather to boot with or without the serial monitor open.
   * It will either exit the while once the monitor is opened  
   * or it will start once the board has been powered for 10 seconds.
   * This while loop was provided by @IoTPanic on Adafruit's Discord server (Thank you!)
   */
  unsigned long s = millis();    
  while(!Serial && millis()-s <= 10000);

Serial.begin(9600);        

    //Conect to the BME
    unsigned status;
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
  
   // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    //error message printing
    Serial.println(io.statusText());
    Serial.print("io.status: ");
    Serial.println(io.status());
    Serial.print("AIO_CONNECTED: ");
    Serial.println(AIO_CONNECTED);
    delay(5000);
    //lets you know something is happening if the connection takes too long
    Serial.println("Waiting for Connection...");
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

  
}





void loop() {

  /* io.run(); is required for all sketches.
  it should always be present at the top of your loop
  function. it keeps the client connected to 
  io.adafruit.com, and processes any incoming data.*/
  
  io.run();


  //general variable creations
  int arrLng = 59; //arrLng = IO upload delay in seconds - 1 (for IO uploads every 60sec put 59) 
  //arrays to record readings
  double tmpArr[arrLng];
  double humArr[arrLng];
  double pressArr[arrLng];
  //varibles to get the averages from the array data
  double sumTmp = 0;
  double avgTmp = 0;
  double sumHum = 0;
  double avgHum = 0;
  double sumPress = 0;
  double avgPress = 0;
  //variables for new record highs/lows
  double newRecTempHigh;
  double newRecTempLow;
  double newRecPressHigh;
  double newRecPressLow;
  
 
  /******Sensor Readings and High/Low Triggers*******/
  
  //for loop to save sensor readings data 
  for(int i = 0; i < arrLng; i++){

        //variable for temperature data
        double c = bme.readTemperature();  //read temperature data in C
        double f = (c * 1.8) + 32;  //convert C to F

        //variable for humidity data
         double relHum = bme.readHumidity();

         //variable for pressure data
         double p_hPa = (bme.readPressure() / 100.0F);
    
        tmpArr[i] = f;
        humArr[i] = relHum;
        pressArr[i] = p_hPa;

        //Record High Temp
        if(f > recTempHigh){
          newRecTempHigh = f;
          } 

        //Record Low Temp
        if(f < recTempLow){
          newRecTempLow = f;
          } 

        //Record High Pressure
        if(p_hPa > recPressHigh){
          newRecPressHigh = p_hPa;
          } 

        //Record Low Pressure
        if(p_hPa < recPressLow){
          newRecPressLow = p_hPa;
          
          } 
            
        //wait 1 second before starting the for loop again
        Serial.print(".");
        delay(1000);
           
   }

  //for loop to sum readings in array
  for(int a = 0; a < arrLng; a++){
      
      sumTmp += tmpArr[a];
      sumHum += humArr[a];
      sumPress += pressArr[a];
      
      }

  //finds the average reading from the array data
  avgTmp = sumTmp / (arrLng);
  avgHum = sumHum / (arrLng);
  avgPress = sumPress / (arrLng);



    
    
    /******Serial Monitor Output*******/
     
  Serial.println("");
  Serial.println("Current Readings:");
  
  //print Temp reading to the monitor
  Serial.print("Temperature: ");
  Serial.print(avgTmp);
  Serial.println("F");
  
  //Print Humidity reading
  Serial.print("humidity: ");
  Serial.print(avgHum);
  Serial.println("%");
  
  //Print Pressure reading
  Serial.print("Pressure: ");
  Serial.print(avgPress);
  Serial.println(" hPa");


  Serial.println("");
  Serial.println("Record Highs & Lows:");
  
  //Print Record High Temp
  Serial.print("Record High Temp: ");
  Serial.print(newRecTempHigh);
  Serial.println("F");
  
  
  //Print Record Low Temp
  Serial.print("Record Low Temp: ");
  Serial.print(newRecTempLow);
  Serial.println("F");
  

  //Print Record High Pressure
  Serial.print("Record High Pressure: ");
  Serial.print(newRecPressHigh);
  Serial.println(" hPa");
  

  //Print Record Low Pressure
  Serial.print("Record Low Pressure: ");
  Serial.print(newRecPressLow);
  Serial.println(" hPa");
  Serial.println("");




  /******Save to Adafruit IO*******/

        
  temperature->save(avgTmp); 
  humidity->save(avgHum);
  pressure->save(avgPress);

  //Record High Temp
  if(newRecTempHigh > recTempHigh){
     recTempHigh = newRecTempHigh;
     rechightemp->save(recTempHigh);
     } 

   //Record Low Temp
   if(newRecTempLow < recTempLow){
      recTempLow = newRecTempLow;
      reclowtemp->save(recTempLow);
      } 

   //Record High Pressure
   if(newRecPressHigh > recPressHigh){
      recPressHigh = newRecPressHigh;
      rechighpress->save(recPressHigh);
      } 

   //Record Low Pressure
   if(newRecPressLow < recPressLow){
      recPressLow = newRecPressLow;
      reclowpress->save(recPressLow);
      } 

  
}
