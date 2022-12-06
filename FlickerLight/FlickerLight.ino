// Using Teensy-LC

#define NUM_OUT  10

int outPins[NUM_OUT] = { 3, 4, 6, 9, 10, 16, 17, 20, 22, 23 };
boolean outEnable[NUM_OUT] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

unsigned long flickerTimer[NUM_OUT];
uint16_t flickerRun[NUM_OUT], flickerPause[NUM_OUT];

void setup() {
  for (uint8_t i=0; i<NUM_OUT; i++) {
    pinMode(outPins[i], OUTPUT);
    outEnable[i] = true;
    flickerRun[i] = random(50, 1000);
    flickerPause[i] = random(200, 5000);
    flickerTimer[i] = millis();
  }

  randomSeed(analogRead(0));
}

void loop() {
  unsigned long now = millis();

  for (uint8_t i=0; i<NUM_OUT; i++) {

    if (outEnable[i]) analogWrite(outPins[i], random(20,256));
      else analogWrite(outPins[i], 0);

    if (outEnable[i] && now - flickerTimer[i] > flickerRun[i]) {
      outEnable[i] = false;
      flickerTimer[i] = millis();
    }
    if (!outEnable[i] && now - flickerTimer[i] > flickerPause[i]) {
      outEnable[i] = true;
      flickerRun[i] = random(50, 1000);
      flickerPause[i] = random(200, 5000);
      flickerTimer[i] = millis();
    }

  }
}
