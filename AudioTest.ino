#include <I2S.h>

void setup() {
  // put your setup code here, to run once:


 // start I2S at 8 kHz with 32-bits per sample
  if (!I2S.begin(I2S_PHILIPS_MODE, 8000, 32)) {
    Serial.println("Failed to initialize I2S!");
    while (1); // do nothing
  }
}

void loop() {
  // put your main code here, to run repeatedly:
    int arrLng = 14;
  int delayLng = 500; 
    int audioRawArr[arrLng];
    int sumAudioRaw = 0;  
  int avgAudioRaw = 0;

for(int i = 0; i < arrLng; i++){
  int audioRaw = I2S.read();

 audioRawArr[i] = audioRaw;
   
   //Serial.print(".");
   
        delay(delayLng);
  }
for(int a = 0; a < arrLng; a++){
      
    
    sumAudioRaw += audioRawArr[a];
  
          
      }
  avgAudioRaw = (sumAudioRaw / arrLng) * -1;
  //Serial.println("");
  //Serial.print("Raw Audio: ");
  Serial.println(avgAudioRaw);  
 
}
