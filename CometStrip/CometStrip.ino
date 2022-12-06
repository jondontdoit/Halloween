#include <Adafruit_NeoPixel_ZeroDMA.h>

//////////////////////////////////////////////
// GLOBAL SETTINGS
//////////////////////////////////////////////
#define NUM_PIXELS     450
#define NUM_SEGMENTS     9

// Strands 1-3: Base
// Strand    4: Post
// Strand  5-6: Roof
// Strand    7: Crossbeam
// Strand    8: Post

const int segmentStartPos[NUM_SEGMENTS] = { 0, 50, 100, 150, 200, 250, 300, 350, 400 };

const int cometSpeed = 15;
const int cometColourCount = 1;
const int cometColourSize = 8;
const uint32_t cometColourValues[cometColourCount] = { 0xFFAA40 };
const uint32_t backColour = 0x302010;
const uint32_t idleColour = 0x800000;

const byte maxBrightness = 255;

//////////////////////////////////////////////
// PINS
//////////////////////////////////////////////
#define TRIGGER_PIN   0
#define NEOPIXEL_PIN  4

//////////////////////////////////////////////
// INTERNAL VARIABLES AND STRUCTURES
//////////////////////////////////////////////
// DMA NeoPixels work ONLY on SPECIFIC PINS.
// On Trinket M0: pin 4.
Adafruit_NeoPixel_ZeroDMA strip(NUM_PIXELS, NEOPIXEL_PIN, NEO_RGB);

int cometPos[NUM_SEGMENTS];
unsigned long lastRefresh;
boolean isIdle = true;

//////////////////////////////////////////////
// SETUP
//////////////////////////////////////////////
void setup() {
  // put your setup code here, to run once:
  pinMode(TRIGGER_PIN, INPUT_PULLDOWN);

  for (int x=0; x<NUM_SEGMENTS; x++) {
    cometPos[x] = segmentStartPos[x];
  }
  
  strip.begin();
  strip.setBrightness(maxBrightness);
  drawAll(idleColour);
  strip.show();
}

//////////////////////////////////////////////
// LOOP
//////////////////////////////////////////////
void loop() {
  // put your main code here, to run repeatedly:
  unsigned long now = millis();

  byte triggerVal = digitalRead(TRIGGER_PIN);

  // Run while triggered
  if (triggerVal) {
    // Draw comet
    if (now - lastRefresh > cometSpeed) {

      for (int i=0; i<NUM_SEGMENTS; i++) {
        drawComet(i, cometPos[i]);
        cometPos[i]++;
        if (i == NUM_SEGMENTS-1) {
          if (cometPos[i] >= NUM_PIXELS) cometPos[i] = segmentStartPos[i];
        } else {
          if (cometPos[i] >= segmentStartPos[i+1]) cometPos[i] = segmentStartPos[i];
        }
      }
      
      strip.show();
      lastRefresh = now;
    }
    isIdle = false;
  }

  // Idle
  if (!triggerVal && !isIdle) {
    drawAll(idleColour);
    strip.show();
    isIdle = true;;
  }


  
}

//////////////////////////////////////////////
// FUNCTIONS
//////////////////////////////////////////////
void drawComet(int seg, int startPos) {
  if (seg < 0 || seg >= NUM_SEGMENTS) seg = 0;
  if (startPos < 0 || startPos >= strip.numPixels()) startPos = 0;

  // Clear the segment
  int segStop = 0;
  if (seg == NUM_SEGMENTS-1) segStop = NUM_PIXELS;
  else segStop = segmentStartPos[seg+1];
  
  for (int i=segmentStartPos[seg]; i<segStop; i++) {
    strip.setPixelColor(i, backColour);
  }

  for (int c = 0; c < cometColourCount; c++) {
    for (int s = 0; s < cometColourSize; s++) {
      strip.setPixelColor(startPos, cometColourValues[c]);
      if (startPos == 0) startPos = strip.numPixels();
      startPos--;
    }
  }

  return;
}

void drawAll(uint32_t _c) {
  for (int x=0; x<strip.numPixels(); x++) {
    strip.setPixelColor(x, _c);
  }
  return;
}
