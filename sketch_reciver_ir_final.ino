#include <Arduino.h>
#include <IRremote.h>
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  IrReceiver.begin(21,ENABLE_LED_FEEDBACK);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(IrReceiver.decode()){ 
   Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
   IrReceiver.printIRResultShort(&Serial); 
   IrReceiver.printIRSendUsage(&Serial); 
   IrReceiver.resume(); }
}
