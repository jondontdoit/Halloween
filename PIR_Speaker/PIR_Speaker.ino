#define INPUT_PIN   4
#define OUTPUT_PIN  3

unsigned long wait = 10000;

unsigned long last_event;

int inputState, inputStateLast;

// the setup routine runs once when you press reset:
void setup() {
  pinMode(INPUT_PIN, INPUT);
  pinMode(OUTPUT_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(OUTPUT_PIN, HIGH);
}

// the loop routine runs over and over again forever:
void loop() {
  unsigned long now = millis();

  // read the input pin:
  inputState = digitalRead(INPUT_PIN);

  if (inputState && !inputStateLast && now - last_event > wait) {
    digitalWrite(OUTPUT_PIN, LOW);
    delay(500);
    digitalWrite(OUTPUT_PIN, HIGH);
    last_event = now;
  }

  if (now - last_event <= wait) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }

  inputStateLast = inputState;
  delay(1);        // delay in between reads for stability
}
