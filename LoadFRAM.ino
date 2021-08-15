#include <Wire.h>
#include "Adafruit_FRAM_I2C.h"

/* Example code for the Adafruit I2C FRAM breakout */

/* Connect SCL    to analog 5
   Connect SDA    to analog 4
   Connect VDD    to 5.0V DC
   Connect GROUND to common ground */
   
Adafruit_FRAM_I2C fram     = Adafruit_FRAM_I2C();

class FRAMConvertFloat{
   
  public: uint8_t writeBytes(float a, int i){
    
    union{      
      float f;
      uint8_t bytes[4];
    }output;
    
    output.f = a;

    return output.bytes[i];
   }

   public: float readFloat(uint8_t a, uint8_t b, uint8_t c, uint8_t d){

    union{      
      float f;
      uint8_t bytes[4];
    }output;

    output.bytes[0] = a;
    output.bytes[1] = b;
    output.bytes[2] = c;
    output.bytes[3] = d;

    return output.f;
  }

   public: float readBytes(uint8_t a, uint8_t b, uint8_t c, uint8_t d){

    union{      
      float f;
      uint8_t bytes[4];
    }output;

    output.bytes[0] = a;
    output.bytes[1] = b;
    output.bytes[2] = c;
    output.bytes[3] = d;

    return output.f;
  }
    
};



void setup(void) {
  Serial.begin(9600);
  delay(2000);
  if (fram.begin()) {  // you can stick the new i2c addr in here, e.g. begin(0x51);
    Serial.println("Found I2C FRAM");
  } else {
    Serial.println("I2C FRAM not identified ... check your connections?\r\n");
    Serial.println("Will continue in case this processor doesn't support repeated start\r\n");
  }
  delay(2000);


  

 
float hT = 94.40;
float lT = 73.29;
float tP = 1015.46;
float lP = 1011.45;

FRAMConvertFloat hTmp;
FRAMConvertFloat lTmp;
FRAMConvertFloat hHpa;
FRAMConvertFloat lHpa;



  
  /*//wrting loop  
  for(int i = 0; i < 4; i++) {
  fram.write8(0x0+i,hTmp.writeBytes(hT,i));
  }
  for(int i = 0; i < 4; i++) {
  fram.write8(0x4+i,lTmp.writeBytes(lT,i));
  }
  for(int i = 0; i < 4; i++) {
  fram.write8(0x16+i,hHpa.writeBytes(tP,i));
  }
  for(int i = 0; i < 4; i++) {
  fram.write8(0x12+i,lHpa.writeBytes(lP,i));
  }*/

uint8_t hTmpA = fram.read8(0x0); 
uint8_t hTmpB = fram.read8(0x1);
uint8_t hTmpC = fram.read8(0x2);
uint8_t hTmpD = fram.read8(0x3);

uint8_t lTmpA = fram.read8(0x4);
uint8_t lTmpB = fram.read8(0x5);
uint8_t lTmpC = fram.read8(0x6);
uint8_t lTmpD = fram.read8(0x7);

uint8_t hHpaA = fram.read8(0x16);
uint8_t hHpaB = fram.read8(0x17);
uint8_t hHpaC = fram.read8(0x18);
uint8_t hHpaD = fram.read8(0x19);

uint8_t lHpaA = fram.read8(0x12);
uint8_t lHpaB = fram.read8(0x13);
uint8_t lHpaC = fram.read8(0x14);
uint8_t lHpaD = fram.read8(0x15);
  
float recTmpH = hTmp.readFloat(hTmpA, hTmpB, hTmpC, hTmpD);
float recTmpL = lTmp.readFloat(lTmpA, lTmpB, lTmpC, lTmpD);
float recHpaH = hHpa.readFloat(hHpaA, hHpaB, hHpaC, hHpaD);
float recHpaL = lHpa.readFloat(lHpaA, lHpaB, lHpaC, lHpaD);
    
  Serial.print("hTmp: ");
  Serial.println(recTmpH);
  Serial.print("lTmp: ");
  Serial.println(recTmpL);
  Serial.print("hHpa: ");
  Serial.println(recHpaH);
  Serial.print("lHpa: ");
  Serial.println(recHpaL);
  

 
  
  
  /*// dump the entire 32K of memory!
  uint8_t value;
  for (uint16_t a = 0; a < 32768; a++) {
    value = fram.read8(a);
    if ((a % 32) == 0) {
      Serial.print("\n 0x"); Serial.print(a, HEX); Serial.print(": ");
    }
    Serial.print("0x"); 
    if (value < 0x1) 
      Serial.print('0');
    Serial.print(value, HEX); Serial.print(" ");
  }*/

  

  
  
}

void loop(void) {

}
