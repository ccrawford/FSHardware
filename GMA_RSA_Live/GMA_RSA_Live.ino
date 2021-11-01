//This version currently on the GMA July 5, 2021 CAC
// For Arduino Mega 2560
// Encoder Library: pjrc (Paul Stroffregen) https://www.pjrc.com/teensy/td_libs_Encoder.html


#define ENCODER_OPTIMIZE_INTERRUPTS

#include "Button.h"
#include "Encoder.h"


//define LED output pins
// Be sure to use a current limiting resistor.

#define LEDa1 25 // MIC 1
#define LEDa2 27 // COM 1
#define LEDb1 29 // MIC 2
#define LEDb2 31 // COM 2
#define LEDc1 30 // MIC 3
#define LEDc2 28 // COM 3
#define LEDd1 26 // 1/2
#define LEDd2 9 // TEL

#define LEDe1 11 // PA
#define LEDe2 13 // SPKR
#define LEDf1 15 // MKR
#define LEDf2 14 // HI
#define LEDg1 12 // DME
#define LEDg2 10 // NAV 1
#define LEDh1 67 // ADF
#define LEDh2 69 // NAV 2
#define LEDi1 33 // AUX
                 // BLANK
#define LEDj1 34 // SQUELCH 
#define LEDj2 32 // PLAY 
#define LEDk1 68 // PILOT 1
#define LEDk2 66 // COPILOT 1

//Below for non mux buttons only
//better to do this with a handler, but this is fine
void ToggleButtonUpdate(const Button &btn) {
  if(btn.released()) {
    Serial.write(String(btn.get_name() + "=0\n").c_str());  
  } else if(btn.pressed()) {
    Serial.write(String(btn.get_name() + "=1\n").c_str());
    delay(50); //delay to avoid double click inputs
  }
}

//LED input data word parser to drive audio status LEDs
void handleLEDData() {
  // Sent as string of 0 and 1s. First character is > and last is \n
    if (Serial.available() >= 23) { //full dataword available from RSG driver. 21 plust start/end.
      //capture full databurst and parse through it. 
      String LEDDATA = Serial.readStringUntil("\n"); 
      int led[] = {LEDa1, LEDa2, LEDb1, LEDb2, LEDc1, LEDc2, LEDd1, LEDd2, LEDe1, LEDe2, LEDf1, LEDf2, LEDg1, LEDg2, LEDh1, LEDh2, LEDi1, LEDj1, LEDj2};
      for (uint8_t i=0; i<(sizeof(led)/sizeof(led[0])); ++i){
        if (LEDDATA.charAt(i+1) == '1'){
          digitalWrite(led[i], HIGH);
        } else {
          digitalWrite(led[i], LOW);
        }
      }
    }
    
}

//rotary handler section
class RotaryHandler {
public:
  virtual void update() = 0;
};

class QuadPulseRotaryHandler : public RotaryHandler {
  Encoder enc;
  long state = -999;
  const char* up;
  const char* down;
  const int pulsecount = 2; //*** set this for number of pulses per click on your encoders (4 most common)
  
public:
  QuadPulseRotaryHandler(const Encoder &enc, const char* up, const char* down) : enc{enc}, up{up}, down{down} {}
  void update() {
    long new_state = enc.read()/pulsecount;
    if (new_state > state){ //turning clockwise
      Serial.write(up);
    } else if (new_state < state) { //turning counterclockwise
      Serial.write(down);    
    }
    state = new_state;
  }
};


// define number of buttons
#define NUM_BUTTONS 23 //total number of buttons (not on muxes)
Button Buttons[NUM_BUTTONS];


// define number of encoders
#define NUM_ENCODERS 2 //total number of encoders
RotaryHandler *Encoders[NUM_ENCODERS];

// define encoders  (name, pin pair for A and B pins)
Encoder VOL_INNER(61, 63);
Encoder VOL_OUTER(60, 62);


//define incoming data string
String LEDDATA[] = "";
int LEDIndex = 2; 

void setup() {

  // start serial connection
  Serial.begin(115200);
  Serial.println("GMA_RSA_Live.ino July 2021. CAC");
  
  // initialize LED output pins
  pinMode(LEDa1, OUTPUT);
  pinMode(LEDa2, OUTPUT);
  pinMode(LEDb1, OUTPUT);
  pinMode(LEDb2, OUTPUT);
  pinMode(LEDc1, OUTPUT);
  pinMode(LEDc2, OUTPUT);
  pinMode(LEDd1, OUTPUT);
  pinMode(LEDd2, OUTPUT);
  pinMode(LEDe1, OUTPUT);
  pinMode(LEDe2, OUTPUT);
  pinMode(LEDf1, OUTPUT);
  pinMode(LEDf2, OUTPUT);
  pinMode(LEDg1, OUTPUT);
  pinMode(LEDg2, OUTPUT);
  pinMode(LEDh1, OUTPUT);
  pinMode(LEDh2, OUTPUT);
  pinMode(LEDi1, OUTPUT);
  pinMode(LEDj1, OUTPUT);
  pinMode(LEDj2, OUTPUT);

// Turn on the ground pins
  pinMode(22, OUTPUT);
  digitalWrite(22, LOW);
  pinMode(A11, OUTPUT);
  digitalWrite(A11, LOW);
  pinMode(6, OUTPUT);
  digitalWrite(6, LOW);

  
  // init non-mux buttons

  uint8_t i = 0;
  Buttons[i++].initialize("BTN_COM1", 18, 10);
  Buttons[i++].initialize("BTN_MIC1", 20, 10);
  Buttons[i++].initialize("BTN_COM2", 19, 10);
  Buttons[i++].initialize("BTN_MIC2", 16, 10);
  Buttons[i++].initialize("BTN_COM3", 21, 10);
  Buttons[i++].initialize("BTN_MIC3", 17, 10);
  Buttons[i++].initialize("BTN_COM_TOG", 23, 10);
  Buttons[i++].initialize("BTN_TEL", 50, 10);  

  Buttons[i++].initialize("BTN_PA", 48, 10);
  Buttons[i++].initialize("BTN_SPKR", 47, 10);
  Buttons[i++].initialize("BTN_MKR", 45, 10);
  Buttons[i++].initialize("BTN_SENS", 49, 10);
  Buttons[i++].initialize("BTN_DME", 51, 10);
  Buttons[i++].initialize("BTN_NAV1", 53, 10);
  Buttons[i++].initialize("BTN_ADF", 40, 10);
  Buttons[i++].initialize("BTN_NAV2", 38, 10);
  Buttons[i++].initialize("BTN_AUX", 36, 10);
  Buttons[i++].initialize("BTN_MANSQ", 37, 10); 
  Buttons[i++].initialize("BTN_PLAY", 39, 10);
  Buttons[i++].initialize("BTN_PILOT", 41, 10); 
  Buttons[i++].initialize("BTN_COPLT", 43, 10);
  
  Buttons[i++].initialize("BTN_VOL", 64, 10);
  
  Buttons[i++].initialize("BTN_REVERSION", 7, 10);
  
// end of init non-mux buttons

  i=0;
  Encoders[i++] = new QuadPulseRotaryHandler{VOL_INNER, "ENC_PILOT_UP\n", "ENC_PILOT_DN\n"};
  Encoders[i++] = new QuadPulseRotaryHandler{VOL_OUTER, "ENC_PASS_UP\n", "ENC_PASS_DN\n"};
}

unsigned long lastKeepAlive = 0;
unsigned long lastKeepAliveLong = 0;

void loop() {

  //  \####RealSimGear#RealSimGear-GMA-Addon#5#3.1.9#756E6B776F0107/

  // keep alive for RSG connection
  if(millis() - lastKeepAliveLong > 3000 ) {
    Serial.write("\\####RealSimGear#RealSimGear-GMA-Addon#5#3.1.9#756E6B776F0107/\n"); // 3.1.9 = latest firmware; 756E6B776F06E = RANDOM ID
    lastKeepAliveLong = millis();
    lastKeepAlive = millis();
  }
  if(millis() - lastKeepAlive > 100 ) {
    Serial.write("\n"); 
    lastKeepAlive = millis();
  }

  //check for incoming serial
   handleLEDData(); 

  // check for non-mux buttons pressed/released
  for(uint8_t i=0; i<NUM_BUTTONS; i++){
      ToggleButtonUpdate(Buttons[i]);
  }
  //end of non-mux button check
  
  //update encoders
  for (uint8_t i = 0; i < NUM_ENCODERS; ++i) {
    Encoders[i]->update();
  }
  //end of update encoders
}
