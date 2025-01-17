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
#define NUM_BUTTONS 17 //total number of buttons (not on muxes)
Button Buttons[NUM_BUTTONS];


// define number of encoders
#define NUM_ENCODERS 6 //total number of encoders
RotaryHandler *Encoders[NUM_ENCODERS];

// define encoders  (name, pin pair for A and B pins)
Encoder ENC_FMS_INNER(49, 48);
Encoder ENC_FMS_OUTER(47, 46);
Encoder ENC_COM_OUTER(53, 52);
Encoder ENC_COM_INNER(50, 51);
Encoder ENC_NAV_VOL(9, 8);
Encoder ENC_COM_VOL(6, 7);

void setup() {

  // start serial connection
  Serial.begin(115200);
  
  // init non-mux buttons
  uint8_t i = 0;
  Buttons[i++].initialize("BTN_NAV_FF", 22, 10);
  Buttons[i++].initialize("BTN_COM_FF", 24, 10);

  Buttons[i++].initialize("BTN_CDI", 32, 10);
  Buttons[i++].initialize("BTN_OBS", 34, 10);
  Buttons[i++].initialize("BTN_MSG", 37, 10);
  Buttons[i++].initialize("BTN_FPL", 36, 10);
  Buttons[i++].initialize("BTN_PROC", 33, 10);

  Buttons[i++].initialize("BTN_ZOOM_UP", 27, 10);
  Buttons[i++].initialize("BTN_ZOOM_DN", 26, 10);
  Buttons[i++].initialize("BTN_DIRECT", 28, 10);
  Buttons[i++].initialize("BTN_MENU", 25, 10);
  Buttons[i++].initialize("BTN_CLR", 29, 10);
  Buttons[i++].initialize("BTN_ENT", 23, 10);

  Buttons[i++].initialize("BTN_NAV_COM_TOG", 30, 10);
  Buttons[i++].initialize("BTN_FMS", 31, 10);

  Buttons[i++].initialize("BTN_COM_VOL", 10, 10);
  Buttons[i++].initialize("BTN_NAV_VOL", 11, 10);

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
    //Serial.write("\\####RealSimGear#RealSimGear-GNS530#1#3.2.4#756E6B776F08B/\n"); // From the latest 5/17/2021 download
    Serial.write("\\####RealSimGear#RealSimGear-GNS430#2#3.2.4#3037373734DAF/\n"); // Test
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
