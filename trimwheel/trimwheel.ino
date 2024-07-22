// Analog Trimwheel. Acts as a joystick with a single axis.

// Uses the DigiJoystick library to communicate with the computer
// https://github.com/digistump/DigisparkArduinoIntegration/tree/master/libraries/DigisparkJoystick


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
  rawValue = constrain(rawValue, 4, 1019);
  if (rawValue < (oldValue - 4) || rawValue > (oldValue + 4)) {
    oldValue = rawValue;
    xRotVal = (byte) map(rawValue, 4, 1016, 0, 255);
  }
  

  DigiJoystick.setSLIDER((byte) xRotVal);
  
  // it's best to use DigiJoystick.delay() because it knows how to talk to
  // the connected computer - otherwise the USB link can crash with the 
  // regular arduino delay() function
  DigiJoystick.delay(50); // wait 50 milliseconds
  
}
