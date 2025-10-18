#include <IRremote.h>

const int pirPin = 16;  // PIR sensor output pin (global)
// OFF code i captured:
uint16_t rawIRTimings[181] = {
  3080,3920, 1930,970, 980,1920, 2030,920, 1030,920, 980,920, 1030,920, 1080,870,
  1080,920, 930,1970, 1980,1870, 2030,920, 980,970, 1030,920, 980,970, 980,970,
  1030,920, 930,1020, 930,1020, 930,970, 1030,920, 1030,920, 1030,920, 1030,920,
  980,970, 1030,920, 930,1020, 1030,920, 980,1920, 1980,920, 3030,3970, 1930,970,
  980,1920, 1980,920, 980,970, 980,970, 980,970, 980,970, 980,970, 980,1970,
  1930,1920, 1980,970, 930,1020, 930,1020, 930,970, 980,970, 980,970, 980,1020,
  880,1020, 930,1020, 930,1020, 930,1020, 930,1020, 930,1020, 930,1020, 930,970,
  980,970, 980,970, 980,1970, 1880,1020, 2930,4020, 1880,1020, 930,1970, 1930,1020,
  980,970, 930,1020, 930,1020, 930,1020, 930,1020, 930,1970, 1930,1970, 1880,1020,
  930,1020, 930,1020, 930,1020, 930,1020, 930,1020, 930,1020, 930,970, 980,970,
  980,970, 980,970, 930,1070, 930,970, 930,1070, 930,970, 930,1020, 930,1020,
  930,2020, 1880,1020, 3880
};
// pin initialization
const uint16_t rawLen = sizeof(rawIRTimings)/sizeof(rawIRTimings[0]);
const int GledPin    = 4;   // LED pin (through 220Ω resistor)
const int sensorPin  = 17;  // MC-38 connected here
const int RledPin    = 18;  // LED connected here

void setup() {
  Serial.begin(9600);
  IrSender.begin(21);  // TX pin = GPIO 21
  // pin modes init
  pinMode(pirPin, INPUT);           // PIR gives HIGH when motion detected
  pinMode(GledPin, OUTPUT);         // green LED as output
  pinMode(RledPin, OUTPUT);         // red LED as output
  pinMode(sensorPin, INPUT_PULLUP); // door sensor as input
  digitalWrite(GledPin, LOW);       // Start with LED off
  delay(10000);                     // the PIR takes ~30 s to fully init
}

void loop() {
  int state  = digitalRead(sensorPin); // read the door sensor state
  int motion = digitalRead(pirPin);    // read the PIR output

  if (state == LOW) {// door opened (INPUT_PULLUP with NC sensor)
    digitalWrite(RledPin, HIGH); // red light on = door opened
    delay(15000);                 // if I left the room give time to close the door

    bool am_i_in = false; // for the loop (avoid sending many times)
    for (int i = 0; i < 100; i++) { // check for motion after door opened
      motion = digitalRead(pirPin);
      if (motion == HIGH) {
        am_i_in = true;
        break;
      } else {
        delay(110);
      }
    }

    if (am_i_in == false) { // if I'm out -> turn off AC
      IrSender.sendRaw(rawIRTimings, rawLen, 38);
      delay(110);
      IrSender.sendRaw(rawIRTimings, rawLen, 38);
      delay(110);
      IrSender.sendRaw(rawIRTimings, rawLen, 38);
      Serial.println("OFF sent x3");
      digitalWrite(GledPin, HIGH); // Turn green LED on (AC off)
      delay(300);
    }
  } else { // door wasn't opened (i.e., closed)
    delay(300);
    digitalWrite(RledPin, LOW); // red light off = door closed
    digitalWrite(GledPin, LOW); // green off
  }
}
