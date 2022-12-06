/*
  AnalogReadSerial

  Reads an analog input on pin 0, prints the result to the Serial Monitor.
  Graphical representation is available using Serial Plotter (Tools > Serial Plotter menu).
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/AnalogReadSerial
*/


// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  pinMode(11, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {

  int maxVal = 0;
  for (int i=0; i<100; i++) {  
    int leftVal  = analogRead(A0);
    int rightVal = analogRead(A1);

  //  Serial.print(leftVal);
  //  Serial.print("\t");
  //  Serial.print(rightVal);
  //  Serial.println();
  
    int avgVal = (leftVal+rightVal)/2;
    if (avgVal > maxVal)  maxVal = avgVal;
    
    delay(1); 
  }

//  if (maxVal > 128) {
//    digitalWrite(11, HIGH);
//    Serial.println("HI");
//  } else {
//    digitalWrite(11, LOW);    
//  }
  
  if (maxVal < 1)   maxVal = 1;
  if (maxVal > 255) maxVal = 255;

  Serial.println(maxVal);

  analogWrite(11, maxVal);
  
}
