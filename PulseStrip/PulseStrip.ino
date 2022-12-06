#include <Adafruit_NeoPixel_ZeroDMA.h>

//////////////////////////////////////////////
// GLOBAL SETTINGS
//////////////////////////////////////////////
#define NUM_PIXELS 150

int cometSpeed = 10;
const int cometSegmentSize = 3;

const byte maxBrightness = 255;

//////////////////////////////////////////////
// PINS
//////////////////////////////////////////////
#define NEOPIXEL_PIN  5

//////////////////////////////////////////////
// INTERNAL VARIABLES AND STRUCTURES
//////////////////////////////////////////////
// DMA NeoPixels work ONLY on SPECIFIC PINS.
// On Trinket M0: pin 4.
Adafruit_NeoPixel_ZeroDMA strip(NUM_PIXELS, NEOPIXEL_PIN, NEO_RGB);

const int cometColourCount = 3;
uint32_t cometColourValues[3] = { 0xFFFFFF, 0x606060, 0x101010 };

int cometPos = 0;
unsigned long lastComet;

//////////////////////////////////////////////
// SETUP
//////////////////////////////////////////////
void setup() {
  // put your setup code here, to run once:
  strip.begin();
  strip.setBrightness(maxBrightness);
  strip.show();

  randomSeed(A0);

  pickColours();
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long now = millis();

  // Draw comet
  if (now - lastComet > cometSpeed) {
    drawComet(cometPos);
    cometPos++;
    if (cometPos >= strip.numPixels()) cometPos = 0;
    if (cometPos == 9) {
      cometSpeed = random(10,30);
      pickColours();
    }
    lastComet = now;
  }

}

void drawComet(int startPos) {
  if (startPos < 0 || startPos >= strip.numPixels()) startPos = 0;

  // Clear the strip
  strip.clear();

  for (int c = 0; c < cometColourCount; c++) {
    for (int s = 0; s < cometSegmentSize; s++) {
      strip.setPixelColor((strip.numPixels()-startPos-1), cometColourValues[c]);
      if (startPos == 0) startPos = strip.numPixels();
      startPos--;
    }
  }

  // Display the animation
  strip.show();

  return;
}

void pickColours() {
  // Start full
  uint8_t r = random(256);
  uint8_t g = random(256);
  uint8_t b = random(256);

  // Find brightest value
  uint8_t m = r;
  if (g > m) m = g;
  if (b > m) m = b;

  // Scale that up to full
  r = map(r, 0, m, 0, 255);
  g = map(g, 0, m, 0, 255);
  b = map(b, 0, m, 0, 255);

  // Apply to comet head
  cometColourValues[0] = makeColor(r, g, b);

  // Scale down brightness for second chunk
  r = map(r, 0, 255, 0, 128);
  g = map(g, 0, 255, 0, 128);
  b = map(b, 0, 255, 0, 128);
  cometColourValues[1] = makeColor(r, g, b);

  // Scale down brightness for second chunk
  r = map(r, 0, 255, 0,  64);
  g = map(g, 0, 255, 0,  64);
  b = map(b, 0, 255, 0,  64);
  cometColourValues[2] = makeColor(r, g, b);
  
}

static uint32_t makeColor(uint8_t r, uint8_t g, uint8_t b) {
  return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}
