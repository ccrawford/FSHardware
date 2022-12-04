//DigiJoystick test and usage documentation

#include "DigiJoystick.h"

int sensorValue = 0;
int rawValue;
int oldValue = 0;
byte xRotVal;

void setup() {
  // Do nothing? It seems as if the USB hardware is ready to go on reset
  pinMode(0, OUTPUT);  // Not strictly necessary, 0 is the physical pin #
  pinMode(1, OUTPUT);
  pinMode(5, INPUT);
  pinMode(2, INPUT);

  digitalWrite(0,HIGH);
  digitalWrite(1,LOW);
}


void loop() {
  // If not using plentiful DigiJoystick.delay() calls, make sure to
  //DigiJoystick.update(); // call this at least every 50ms
  // calling more often than that is fine
  // this will actually only send the data every once in a while unless the data is different
  
  // you can set the values from a raw byte array with:
  // char myBuf[8] = {
  //   x, y, xrot, yrot, zrot, slider,
  //   buttonLowByte, buttonHighByte
  // };
  // DigiJoystick.setValues(myBuf);

  rawValue = analogRead(1);
  rawValue = analogRead(1);
  rawValue = constrain(rawValue, 4, 1019);
  if (rawValue < (oldValue - 4) || rawValue > (oldValue + 4)) {
    oldValue = rawValue;
    xRotVal = (byte) map(rawValue, 4, 1016, 0, 255);
  }
  

  // byte xRotVal = (byte) (analogRead(1) / 4);
  
  // Or we can also set values like this:
  //DigiJoystick.setX((byte) (millis() / 100)); // scroll X left to right repeatedly
  //DigiJoystick.setY((byte) 0x30);
  //DigiJoystick.setXROT((byte) 0x10);
  //DigiJoystick.setYROT((byte) (millis() / 255)); 
  //DigiJoystick.setZROT((byte) 0xB0);
  DigiJoystick.setSLIDER((byte) xRotVal);
  
  // it's best to use DigiJoystick.delay() because it knows how to talk to
  // the connected computer - otherwise the USB link can crash with the 
  // regular arduino delay() function
  DigiJoystick.delay(50); // wait 50 milliseconds
  
  // we can also set buttons like this (lowByte, highByte)
  //DigiJoystick.setButtons(0x00, 0x00);
}
