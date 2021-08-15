#include <Arduino.h>
#include <stdio.h>
#include <math.h>

// Most relevant libraries here: Adafruit-samd v1.5.2 (board) + Adafruit_ZeroI2S + arduino-lmic
#include <Adafruit_ZeroI2S.h>
//#include <lmic.h>
//#include <hal/hal.h>

// Mic sampling parameters
// Unsing InvenSense INMP441 via I2S interface
// Further hardware info: https://www.invensense.com/wp-content/uploads/2015/02/INMP441.pdf
#define SAMPLERATE_HZ 44100
int32_t audiosamples = 128;
const int sample_delay = 1000;

// Pin mapping for RFM95w LoRaWAN interface (via LMIC)
// Further hardware info: https://www.hoperf.com/modules/lora/RFM95.html
//const lmic_pinmap lmic_pins = {
  //.nss = 8,
  //.rxtx = LMIC_UNUSED_PIN,
 // .rst = LMIC_UNUSED_PIN,
 // .dio = {3, 6, LMIC_UNUSED_PIN},
//};

// The I2S interface with the Mic using Adafruit Library.
Adafruit_ZeroI2S i2s; 

void setup() {
   
  // Configure serial port.
  Serial.begin(115200);
  Serial.println("Zero I2S Audio Tone Generator");

  // Initialize the I2S transmitter.
  if (!i2s.begin(I2S_32_BIT, SAMPLERATE_HZ)) {
    Serial.println("Failed to initialize I2S transmitter!");
    while (1);
    
  }

i2s.enableRx();
 
   
  
}

void loop() {
  
  // This is a stereo Mic. But we'll only read from LEFT channel
  digitalWrite(11, LOW);  

  float sound;
  int32_t left,right;
  int i;
  int sample=0;

  // Read a bunch of samples!!!
  int32_t samples[audiosamples];
  
  for (int i = 0; i < audiosamples; i++) {
    i2s.read(&left, &right);
    delay(1);  // Workaround delay to prevent oversizing the buffer
    sample = left;
    // convert to 18 bit signed
    sample >>= 14;
    samples[i] = abs(sample);
  }

  // Calculate mean (avg) over samples
  float meanval = 0;
  for (int i = 0; i < audiosamples; i++) {
    meanval += samples[i];
  }
  meanval /= audiosamples;

  // subtract it from all samples to get a 'normalized' output
  for (int i = 0; i < audiosamples; i++) {
    samples[i] -= meanval;
  }

  
  // find the 'peak to peak' max
  float maxsample, minsample;
  minsample = 100000;
  maxsample = -100000;
    
  for (int i = 0; i < audiosamples; i++) {
    minsample = min(minsample, samples[i]);
    maxsample = max(maxsample, samples[i]);
  }

  sound = 10 * log(maxsample - minsample);
  Serial.println(sound);
  
  
  delay(500); // Let's just process a measurement each second.
}
