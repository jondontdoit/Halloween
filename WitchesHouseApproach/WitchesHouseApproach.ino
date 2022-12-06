// Specifically for use with the Adafruit Feather, the pins are pre-set here!

// include SPI, MP3 and SD libraries
#include <SPI.h>
#include <SD.h>
#include <Adafruit_VS1053.h>
#include <stdint.h>
#include <Wire.h>
#include <LIDARLite_v3HP.h>
#include <SparkFunSX1509.h>

// SX1509 I2C address (set by ADDR1 and ADDR0 (00 by default):
const byte SX1509_ADDRESS = 0x3E;  // SX1509 I2C address
SX1509 io; // Create an SX1509 object to be used throughout

int colour = 0;
unsigned long flashLast, flashDelay, flashFade;

LIDARLite_v3HP myLidarLite;

uint16_t distMax;

// These are the pins used
#define VS1053_RESET   -1     // VS1053 reset pin (not used!)
#define VS1053_CS       6     // VS1053 chip select pin (output)
#define VS1053_DCS     10     // VS1053 Data/command select pin (output)
#define CARDCS          5     // Card chip select pin
// DREQ should be an Int pin *if possible* (not possible on 32u4)
#define VS1053_DREQ     9     // VS1053 Data request, ideally an Interrupt pin


Adafruit_VS1053_FilePlayer musicPlayer = 
  Adafruit_VS1053_FilePlayer(VS1053_RESET, VS1053_CS, VS1053_DCS, VS1053_DREQ, CARDCS);

uint8_t musicVol = 255;

void setup() {
  Serial.begin(115200);

  // Initialize Arduino I2C (for communication to LidarLite)
  Wire.begin();
  Wire.setClock(400000UL); // Set I2C frequency to 400kHz (for Arduino Due)
  
  // Configure the LidarLite internal parameters so as to lend itself to
  // various modes of operation by altering 'configure' input integer to
  // anything in the range of 0 to 5. See LIDARLite_v3HP.cpp for details.
  myLidarLite.configure(0);
  
  if (! musicPlayer.begin()) { // initialise the music player
     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     while (1);
  }

  Serial.println(F("VS1053 found"));
 
  musicPlayer.sineTest(0x44, 500);    // Make a tone to indicate VS1053 is working
  
  if (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
    while (1);  // don't do anything more
  }
  Serial.println("SD OK!");
  
  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(musicVol,musicVol);
  
  // If DREQ is on an interrupt pin we can do background
  // audio playing
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int

  // Call io.begin(<address>) to initialize the SX1509. If it
  // successfully communicates, it'll return 1.
  if (!io.begin(SX1509_ADDRESS)) {
      while (1) ; // If we fail to communicate, loop forever.
  }
  
  // Call io.pinMode(<pin>, <mode>) to set an SX1509 pin as
  // an output:
  for (int i=0; i<16; i++)
      io.pinMode(i, ANALOG_OUTPUT);
        
  clearLights();
  
  // Play a file in the background, REQUIRES interrupts!
//  Serial.println(F("Playing full track 001"));
//  musicPlayer.playFullFile("/track001.mp3");
  
  Serial.println(F("Playing track 002"));
  musicPlayer.startPlayingFile("/track002.mp3");

  Serial.println("Ready!");
}

void loop() {  
  unsigned long now = millis();
  
  uint16_t distance;
  uint8_t  newDistance = 0;

  newDistance = distanceContinuous(&distance);

  if (newDistance) {
    if (distance > distMax) distMax = distance;
    flashDelay = map(distance, 0, distMax, 0, 10000);
    flashFade  = map(distance, 0, distMax, 100, 5000);
    musicVol = map(distance, 0, distMax, 1, 125);
    musicPlayer.setVolume(musicVol,musicVol);
    Serial.print(distance);
    Serial.print("\t");
    Serial.print(distMax);
    Serial.print("\t");
    Serial.print(flashDelay);
    Serial.print("\t");
    Serial.print(flashFade);
    Serial.print("\t");
    Serial.print(musicVol);
    Serial.println();
  }


  if (now - flashLast > flashDelay) {
    clearLights();
  
         if (colour == 0) setLights(255,255,0);
    else if (colour == 1) setLights(255,0,0);
    else if (colour == 2) setLights(255,128,0);
    else setLights(255,255,255);
  
    delay(250);
  
    for (int i=255; i>=0; i--) {
           if (colour == 0) setLights(i,i,0);
      else if (colour == 1) setLights(i,0,0);
      else if (colour == 2) setLights(i,(i/2),0);
      else setLights(i,i,i);
      delayMicroseconds(flashFade);
    }
    
    flashLast = millis();
  
    colour++;
    if (colour>=4) colour = 0;
  }

  if (musicPlayer.stopped()) {
    Serial.println(F("Playing track 002"));
    musicPlayer.startPlayingFile("/track002.mp3");
  }

}

//---------------------------------------------------------------------
// Read Continuous Distance Measurements
//
// The most recent distance measurement can always be read from
// device registers. Polling for the BUSY flag in the STATUS
// register can alert the user that the distance measurement is new
// and that the next measurement can be initiated. If the device is
// BUSY this function does nothing and returns 0. If the device is
// NOT BUSY this function triggers the next measurement, reads the
// distance data from the previous measurement, and returns 1.
//---------------------------------------------------------------------
uint8_t distanceContinuous(uint16_t * distance) {
  uint8_t newDistance = 0;

  // Check on busyFlag to indicate if device is idle
  // (meaning = it finished the previously triggered measurement)
  if (myLidarLite.getBusyFlag() == 0) {
    // Trigger the next range measurement
    myLidarLite.takeRange();

    // Read new distance data from device registers
    *distance = myLidarLite.readDistance();

    // Report to calling function that we have new data
    newDistance = 1;
  }

  return newDistance;
}

void setLights(int _r, int _g, int _b) {
  io.analogWrite( 0, 255-_r);
  io.analogWrite( 1, 255-_g);
  io.analogWrite( 2, 255-_b);
  io.analogWrite( 3, 255-_r);
  io.analogWrite( 4, 255-_g);
  io.analogWrite( 5, 255-_b);
  io.analogWrite( 6, 255-_r);
  io.analogWrite( 7, 255-_g);
  io.analogWrite( 8, 255-_b);
  io.analogWrite( 9, 255-_r);
  io.analogWrite(10, 255-_g);
  io.analogWrite(11, 255-_b);
  io.analogWrite(12, 255-_r);
  io.analogWrite(13, 255-_g);
  io.analogWrite(14, 255-_b);
  io.analogWrite(15, 255-_r);
}

void clearLights() {
  for (int i=0; i<16; i++) 
    io.analogWrite(i, 255);
}
