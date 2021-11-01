// This is the version currently I'm currently using in the new, wide MFD with the expensive rotary encoders.
// CAC July 4, 2021
// CAC Nov 1, 2021: Finally fixed the rotary 2/4 pulse issue.

#define ENCODER_OPTIMIZE_INTERRUPTS

#include "Mux.h"
#include <Encoder.h>

using namespace admux;
Mux muxSoft(Pin(53, INPUT_PULLUP, PinType::Digital), Pinset(47, 48, 50, 52), 51); //SIG pin, S0-S3 pins, enable pin
Mux muxAp(Pin(59, INPUT_PULLUP, PinType::Digital), Pinset(47, 48, 50, 52),58); //SIG pin, S0-S3 pins
Mux muxAud(Pin(22, INPUT_PULLUP, PinType::Digital), Pinset(47, 48, 50, 52),23); //SIG pin, S0-S3 pins

// define mux names and set button status to not pressed (up to 16 inputs per mux)
String muxAptxt[] = {"BTN_AP", "BTN_FD", "BTN_ALT", "BTN_HDG", "BTN_VNAV", "BTN_NAV", "BTN_BC", "BTN_NOSE_UP", "BTN_APR", "BTN_VS", "BTN_FLC", "BTN_NOSE_DN", "BTN_ALT_SEL", "BTN_HDG_SYNC", "BTN_NAV_TOG", "BTN_NAV_FF"}; //follows pinout of AP board
bool muxApstatus[] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};

String muxSofttxt[] = {"BTN_SOFT_1", "BTN_SOFT_2", "BTN_SOFT_3", "BTN_SOFT_4", "BTN_SOFT_5", "BTN_SOFT_6", "BTN_SOFT_7", "BTN_SOFT_8", "BTN_SOFT_9", "BTN_SOFT_10", "BTN_SOFT_11", "BTN_SOFT_12", "BTN_NAV_VOL", "BTN_UNDEFINED", "BTN_UNDEFINED", "BTN_UNDEFINED"};
bool muxSoftstatus[] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};

String muxAudtxt[] = {"BTN_FMS", "BTN_MENU", "BTN_PROC", "BTN_ENT", "BTN_DIRECT", "BTN_FPL", "BTN_CLR", "BTN_PAN_SYNC", "BTN_PAN_LEFT", "BTN_PAN_UP", "BTN_PAN_RIGHT", "BTN_PAN_DN", "BTN_COM_VOL", "BTN_COM_FF", "BTN_COM_TOG", "BTN_CRS_SYNC"};
bool muxAudstatus[] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};

//rotary handler section
class RotaryHandler {
public:
  virtual bool update() = 0;
};

class QuadPulseRotaryHandler : public RotaryHandler {
  Encoder enc;
  long state = -999;
  const char* up;
  const char* down;
  int pulsecount = 4; //*** set this for number of pulses per click on your encoders (4 most common)
  
public:
  QuadPulseRotaryHandler(const Encoder &enc, const char* up, const char* down) : enc{enc}, up{up}, down{down} {}
  QuadPulseRotaryHandler(const Encoder &enc, const char* up, const char* down, int pulsecount) : enc{enc}, up{up}, down{down}, pulsecount{pulsecount} {}
  bool update() {
    long new_state = enc.read()/pulsecount;
    if (new_state > state){ //turning clockwise
      Serial.write(up);
      state = new_state;
      return true;
    } else if (new_state < state) { //turning counterclockwise
      Serial.write(down);    
      state = new_state;
      return true;
    }
  }
};

// define number of encoders
#define NUM_ENCODERS 14 //total number of encoders
RotaryHandler *Encoders[NUM_ENCODERS];

// define encoders  (name, pin pair for A and B pins)
//Encoder ENC_FMS_INNER(18,20);
//Encoder ENC_FMS_OUTER(19,21);
Encoder ENC_RANGE(35,34);
Encoder ENC_BARO(28,30);
Encoder ENC_CRS(26,24);
Encoder ENC_COM_OUTER(31,29);
Encoder ENC_COM_INNER(27,25);
Encoder ENC_NAV_OUTER(8,6);
Encoder ENC_NAV_INNER(2,4);
Encoder ENC_ALT_OUTER(56,54);
Encoder ENC_ALT_INNER(57,55);
Encoder ENC_HDG(61,60);
Encoder ENC_NAV_VOL(7,5);
Encoder ENC_COM_VOL(12,10);
Encoder ENC_FMS_INNER(39,41);
Encoder ENC_FMS_OUTER(40,38);


void setup() {

  // start serial connection
  Serial.begin(115200);

  muxAp.enabled(false);
  muxSoft.enabled(false);
  muxAud.enabled(false);

  int i = 0;
  Encoders[i++] = new QuadPulseRotaryHandler{ENC_FMS_INNER, "ENC_FMS_INNER_UP\n", "ENC_FMS_INNER_DN\n",2};
  Encoders[i++] = new QuadPulseRotaryHandler{ENC_FMS_OUTER, "ENC_FMS_OUTER_UP\n", "ENC_FMS_OUTER_DN\n",2};   Encoders[i++] = new QuadPulseRotaryHandler{ENC_COM_VOL, "ENC_COM_VOL_UP\n", "ENC_COM_VOL_DN\n"};
  Encoders[i++] = new QuadPulseRotaryHandler{ENC_NAV_VOL, "ENC_NAV_VOL_UP\n", "ENC_NAV_VOL_DN\n",4};
  Encoders[i++] = new QuadPulseRotaryHandler{ENC_NAV_OUTER, "ENC_NAV_OUTER_UP\n", "ENC_NAV_OUTER_DN\n",2};
  Encoders[i++] = new QuadPulseRotaryHandler{ENC_NAV_INNER, "ENC_NAV_INNER_UP\n", "ENC_NAV_INNER_DN\n",2};
  Encoders[i++] = new QuadPulseRotaryHandler{ENC_HDG, "ENC_HDG_UP\n", "ENC_HDG_DN\n",4};
  Encoders[i++] = new QuadPulseRotaryHandler{ENC_ALT_OUTER, "ENC_ALT_OUTER_UP\n", "ENC_ALT_OUTER_DN\n",2};
  Encoders[i++] = new QuadPulseRotaryHandler{ENC_ALT_INNER, "ENC_ALT_INNER_UP\n", "ENC_ALT_INNER_DN\n",2};
  Encoders[i++] = new QuadPulseRotaryHandler{ENC_COM_OUTER, "ENC_COM_OUTER_UP\n", "ENC_COM_OUTER_DN\n",2};
  Encoders[i++] = new QuadPulseRotaryHandler{ENC_COM_INNER, "ENC_COM_INNER_UP\n", "ENC_COM_INNER_DN\n",2};
  Encoders[i++] = new QuadPulseRotaryHandler{ENC_RANGE, "ENC_RANGE_UP\n", "ENC_RANGE_DN\n",4};
  Encoders[i++] = new QuadPulseRotaryHandler{ENC_BARO, "ENC_BARO_UP\n", "ENC_BARO_DN\n",2};
  Encoders[i++] = new QuadPulseRotaryHandler{ENC_CRS, "ENC_CRS_UP\n", "ENC_CRS_DN\n",2};
 
}

unsigned long lastKeepAlive = 0;
unsigned long lastKeepAliveLong = 0;
unsigned long lastHeld = 0;

void loop() {

  for (uint8_t i = 0; i < NUM_ENCODERS; ++i) {
    // If one of the encoders is changing, short circuit the other checks to avoid chatter.
    if (Encoders[i]->update()) continue;
  }
  
// keep alive for RSG connection
  if(millis() - lastKeepAliveLong > 3000 ) {
    Serial.write("\\####RealSimGear#RealSimGear-G1000XFD1#1#3.1.9#756E6B776F06E/\n"); // 3.1.9 = latest firmware; 756E6B776F06E = RANDOM ID
    lastKeepAliveLong = millis();
    lastKeepAlive = millis();
  }
  if(millis() - lastKeepAlive > 100 ) {
    Serial.write("\n"); 
    lastKeepAlive = millis();
  }
  
  // No non-mux buttons in this build.
  //end of non-mux button check

  // check for muxAp buttons pressed/released 
  byte dataAp;
  muxAp.enabled(true);
  for(byte i=0; i< muxAp.channelCount(); i++){
      dataAp = muxAp.read(i); //get value from selected channel
      if(dataAp == LOW && muxApstatus[i] == false) {
        muxApstatus[i] = true;
        Serial.write(String(muxAptxt[i] + "=1\n").c_str());
       }
      else if (dataAp == HIGH && muxApstatus[i] == true) {
        muxApstatus[i] = false;  
        Serial.write(String(muxAptxt[i] + "=0\n").c_str());
      }
  }
  muxAp.enabled(false);
  // end of muxAp buttons pressed/released 
  
  // check for muxSoft buttons pressed/released 
  muxSoft.enabled(true);
  byte dataSoft;
  for(byte i=0; i< muxSoft.channelCount(); i++){
      dataSoft = muxSoft.read(i); //get value from selected channel
      if(dataSoft == LOW && muxSoftstatus[i] == false) {
        muxSoftstatus[i] = true;
        Serial.write(String(muxSofttxt[i] + "=1\n").c_str());
       }
      else if (dataSoft == HIGH && muxSoftstatus[i] == true) {
        muxSoftstatus[i] = false;  
        Serial.write(String(muxSofttxt[i] + "=0\n").c_str());
      }
  }
  muxSoft.enabled(false);
  // end of muxSoft buttons pressed/released 

  // check for muxAud buttons pressed/released 
  muxAud.enabled(true);
  byte dataAud;
  for(byte i=0; i< muxAud.channelCount(); i++){
      dataAud = muxAud.read(i); //get value from selected channel
      // buttons 8,9,10,11 on auto-repeat.
      // And the button is held (low) and we're currently active (true) send it again without reset.
      if(i>=8 && i<=11 && dataAud == LOW && muxAudstatus[i]) {
        Serial.write(String(muxAudtxt[i] + "=1\n").c_str());
        delay(40); // delay for repeat.
      }
      if(dataAud == LOW && muxAudstatus[i] == false) {
        muxAudstatus[i] = true;
        Serial.write(String(muxAudtxt[i] + "=1\n").c_str());
       }
      else if (dataAud == HIGH && muxAudstatus[i] == true) {
        muxAudstatus[i] = false;  
        Serial.write(String(muxAudtxt[i] + "=0\n").c_str());
      }
  }
  muxAud.enabled(false);
  // end of muxAud buttons pressed/released 


  
}
