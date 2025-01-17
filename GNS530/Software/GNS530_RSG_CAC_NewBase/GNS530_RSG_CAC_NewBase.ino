#define ENCODER_OPTIMIZE_INTERRUPTS

#include "Button.h"
#include "Encoder.h"

//Below for non mux buttons only
//better to do this with a handler, but this is fine
void ToggleButtonUpdate(Button &btn) {
  if(btn.released()) {
    Serial.write(String(btn.get_name() + "=0\n").c_str());  
  } else if(btn.pressed()) {
    Serial.write(String(btn.get_name() + "=1\n").c_str());
    // delay(50); //delay to avoid double click inputs
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
#define NUM_BUTTONS 16 //total number of buttons (not on muxes)
Button Buttons[NUM_BUTTONS];


// define number of encoders
#define NUM_ENCODERS 6 //total number of encoders
RotaryHandler *Encoders[NUM_ENCODERS];

// define encoders  (name, pin pair for A and B pins)
Encoder ENC_FMS_INNER(43, 45);
Encoder ENC_FMS_OUTER(44, 42);
Encoder ENC_COM_OUTER(29, 27);
Encoder ENC_COM_INNER(26, 30);
Encoder ENC_NAV_VOL(23, 21);
Encoder ENC_COM_VOL(20, 22);

void setup() {

  // start serial connection
  Serial.begin(115200);
  
  // Setup in-line ground pins.
  pinMode(47, OUTPUT); digitalWrite(47, LOW); //FMS Gnd
  pinMode(28, OUTPUT); digitalWrite(28, LOW);
  pinMode(25, OUTPUT); digitalWrite(25, LOW);
  pinMode(18, OUTPUT); digitalWrite(18, LOW);
  pinMode(55, OUTPUT); digitalWrite(55, LOW);
  pinMode(54, OUTPUT); digitalWrite(54, LOW);
  
  // init non-mux buttons
  uint8_t i = 0;
  Buttons[i++].initialize("BTN_NAV_FF", 11, 10);
  Buttons[i++].initialize("BTN_COM_FF", 10, 10);

  Buttons[i++].initialize("BTN_CDI", 69, 10);
  Buttons[i++].initialize("BTN_OBS", 33, 10);
  Buttons[i++].initialize("BTN_MSG", 35, 10);
  Buttons[i++].initialize("BTN_FPL", 34, 10);
  Buttons[i++].initialize("BTN_VNAV", 32, 10);
  Buttons[i++].initialize("BTN_PROC", 68, 10);

  Buttons[i++].initialize("BTN_ZOOM_UP", 36, 10);
  Buttons[i++].initialize("BTN_ZOOM_DN", 38, 10);
  Buttons[i++].initialize("BTN_DIRECT", 40, 10);
  Buttons[i++].initialize("BTN_MENU", 41, 10);
  Buttons[i++].initialize("BTN_CLR", 39, 10);
  Buttons[i++].initialize("BTN_ENT", 37, 10);

  Buttons[i++].initialize("BTN_NAV_COM_TOG", 31, 10);
  Buttons[i++].initialize("BTN_FMS", 46, 10);

// end of init non-mux buttons

  i=0;
  Encoders[i++] = new QuadPulseRotaryHandler{ENC_FMS_INNER, "ENC_FMS_INNER_UP\n", "ENC_FMS_INNER_DN\n"};
  Encoders[i++] = new QuadPulseRotaryHandler{ENC_FMS_OUTER, "ENC_FMS_OUTER_UP\n", "ENC_FMS_OUTER_DN\n"};
  
  Encoders[i++] = new QuadPulseRotaryHandler{ENC_COM_INNER, "ENC_COM_INNER_UP\n", "ENC_COM_INNER_DN\n"};
  Encoders[i++] = new QuadPulseRotaryHandler{ENC_COM_OUTER, "ENC_COM_OUTER_UP\n", "ENC_COM_OUTER_DN\n"};
  
  Encoders[i++] = new QuadPulseRotaryHandler{ENC_NAV_VOL, "ENC_NAV_VOL_UP\n", "ENC_NAV_VOL_DN\n"};
  Encoders[i++] = new QuadPulseRotaryHandler{ENC_COM_VOL, "ENC_COM_VOL_UP\n", "ENC_COM_VOL_DN\n"};
}

unsigned long lastKeepAlive = 0;
unsigned long lastKeepAliveLong = 0;

void loop() {
   // keep alive for RSG connection
  if(millis() - lastKeepAliveLong > 3000 ) {
    Serial.write("\\####RealSimGear#RealSimGear-GNS530#1#3.2.4#756E6B776F08B/\n"); // From the latest 5/17/2021 download
    //Serial.write("\\####RealSimGear#RealSimGear-GNS430#2#3.2.4#3037373734DAF/\n"); // Test
    lastKeepAliveLong = millis();
    lastKeepAlive = millis();
  }
  if(millis() - lastKeepAlive > 1000 ) {
    Serial.write("\n"); 
    lastKeepAlive = millis();
  }
  
  // check for buttons pressed/released
  for(uint8_t i=0; i<NUM_BUTTONS; i++){
      ToggleButtonUpdate(Buttons[i]);
  }
  

  //update encoders
  for (uint8_t i = 0; i < NUM_ENCODERS; ++i) {
    Encoders[i]->update();
  }
  
}
