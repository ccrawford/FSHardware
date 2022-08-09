// GFC 500 Autopilot based on RealSimGear.
// 11 LEDs. 3 single rotary encoders, 2 with push button. 12 stand alone buttons (so 14 total buttons).
// The 12th LED is the TRK LED, which is set manually.
// For Arduino Mega 2560
// Encoder Library: pjrc (Paul Stroffregen) https://www.pjrc.com/teensy/td_libs_Encoder.html
// RSG Pin assignments:
/*
 * 14 NAV
15  
16  
17  
18  
19  
20  
21  
22  LED NAV (3)
23  VS
24  
25  LED HDG (2)
26  LED AP (5)
27  LVL
28  LED APR (4)
29  YD
30  LED LVL (1)
31  HDG Sync
32  LED FD (6)
33  AP
34  LED IAS (7)
35  IAS
36  LED YD (8)
37  VNAV
38  VS LED (10)
39  APR
40  LED VS (11)
41  FD
42  ENC HDG A
43  TRK
44  ALT LED (9)
45  enc wheel A
46        USING THIS FOR TRK LED. NOT SET BY SIMULATOR.
47  HDG
48  ENC HDG B
49  enc wheel B
50  ALT SYNC
51  ALT
52  alt enc a
53  alt enc B

 */

 /* RSG CommandMapping
ENC_HDG_DN=HEADING_BUG_DEC
ENC_HDG_UP=HEADING_BUG_INC
BTN_HDG_SYNC+=HEADING_BUG_SET#PLANE HEADING DEGREES MAGNETIC|Degrees
ENC_TRK_UP=VOR1_OBI_INC
ENC_TRK_DN=VOR1_OBI_DEC
ENC_ALT_UP=AP_ALT_VAR_INC
ENC_ALT_DN=AP_ALT_VAR_DEC
BTN_ALT_SYNC+=UNAVAILABLE
BTN_AP+=AP_MASTER
BTN_FD+=TOGGLE_FLIGHT_DIRECTOR
BTN_HDG+=AP_PANEL_HEADING_HOLD
BTN_ALT+=AP_ALT_HOLD
BTN_NAV+=AP_NAV1_HOLD
BTN_VNAV+=AS1000.VNAV.TOGGLE
BTN_APR+=AP_APR_HOLD
BTN_VS+=AP_PANEL_VS_HOLD
ENC_Wheel_UP=AP_VS_VAR_INC
ENC_Wheel_UP@AUTOPILOT FLIGHT LEVEL CHANGE:Bool#1=AP_SPD_VAR_DEC
ENC_Wheel_UP@AUTOPILOT FLIGHT LEVEL CHANGE:Bool#0=AP_VS_VAR_INC
ENC_Wheel_DN@AUTOPILOT FLIGHT LEVEL CHANGE:Bool#1=AP_SPD_VAR_INC
ENC_Wheel_DN@AUTOPILOT FLIGHT LEVEL CHANGE:Bool#0=AP_VS_VAR_DEC
BTN_IAS+=FLIGHT_LEVEL_CHANGE
BTN_YD+=YAW_DAMPER_TOGGLE
BTN_LVL+=AP_WING_LEVELER
<0=AUTOPILOT WING LEVELER|Bool#1
<1=AUTOPILOT HEADING LOCK|Bool#1
<2=AUTOPILOT NAV1 LOCK|Bool#1
<3=AUTOPILOT APPROACH HOLD|Bool#1
<4=AUTOPILOT MASTER|Bool#1
<5=AUTOPILOT FLIGHT DIRECTOR ACTIVE|Bool#1
<6=AUTOPILOT AIRSPEED HOLD|Bool#1
<7=AUTOPILOT YAW DAMPER|Bool#1
<8=AUTOPILOT ALTITUDE LOCK|Bool#1
<9=AUTOPILOT VERTICAL HOLD|Bool#1
<10=AUTOPILOT GLIDESLOPE HOLD|Bool#1
 
  */

#define ENCODER_OPTIMIZE_INTERRUPTS

#include "Button.h"
#include "Encoder.h"


//define LED output pins
// Be sure to use a current limiting resistor.
// These need to be in order that RSG presents them

#define LEDlvl 30 // LVL (1)
#define LEDhdg 25 // HDG (2)
#define LEDnav 22 // NAV (3)
#define LEDapr 28 // APR (4)
#define LEDap 26 // AP (5)
#define LEDfd 32 // FD (6)
#define LEDias 34 // IAS (7)
#define LEDyd 36 // YD (8)
#define LEDalt 44 // ALT (9)
#define LEDvs 40 // VS (10)
#define LEDvnav 38fli // Glide Slope VNAV (11)
#define LEDtrk 46 // DOES NOT COME FROM RSG. 

bool TRKmode = false;

int LED[] = {LEDlvl, LEDhdg, LEDnav, LEDapr, LEDap, LEDfd, LEDias, LEDyd, LEDalt, LEDvs, LEDvnav, LEDtrk};

//Below for non mux buttons only
//better to do this with a handler, but this is fine
void ToggleButtonUpdate(const Button &btn) {
  if(btn.released()) {
    Serial.write(String(btn.get_name() + "=0\n").c_str());  
  } else if(btn.pressed()) {
    Serial.write(String(btn.get_name() + "=1\n").c_str());
    if(btn.get_name() == "BTN_TRK") {
      TRKmode = !TRKmode;
    }
    delay(50); //delay to avoid double click inputs
  }
}

//LED input data word parser to drive audio status LEDs
void handleLEDData() {
  // Sent as string of 0 and 1s. First character is > and last is \n
    if (Serial.available() >= 12) { //full dataword available from RSG driver. 11 plust start/end. Looks like <100011000101>
      String LEDDATA = Serial.readStringUntil("\n"); 
      //Serial.print(LEDDATA.length());
      for (uint8_t i=0; i<(sizeof(LED)/sizeof(LED[0])) - 1; ++i){ // Take off one for the LEDtrk which is manual.
        if (LEDDATA.charAt(i+1) == '1'){
          digitalWrite(LED[i], HIGH);
        } else {
          digitalWrite(LED[i], LOW);
        }
      }
    }

    digitalWrite(LEDtrk, TRKmode);
}

//rotary handler section
class RotaryHandler {
public:
  virtual void update() = 0;
  bool skip = false;
};

class QuadPulseRotaryHandler : public RotaryHandler {
  Encoder enc;
  long state = -999;
  const char* up;
  const char* down;
  const int pulsecount = 4; //*** set this for number of pulses per click on your encoders (4 most common)
  
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
#define NUM_BUTTONS 14 //total number of buttons (not on muxes)
Button Buttons[NUM_BUTTONS];


// define number of encoders
#define NUM_ENCODERS 4 //total number of encoders: 1 normal, 1 mouse wheel, one a software-dual
RotaryHandler *Encoders[NUM_ENCODERS];

// define encoders  (name, pin pair for A and B pins)
Encoder ENC_ALT(53, 52);

Encoder ENC_HDG(42, 48);
Encoder ENC_TRK(42, 48);

Encoder ENC_VS(45, 49);


void setup() {

  // start serial connection
  Serial.begin(115200);
  Serial.println("GFC500.ino January 2022. CAC");
  
  // initialize LED output pins
  for (uint8_t i=0; i<(sizeof(LED)/sizeof(LED[0])); ++i){
    pinMode(LED[i], OUTPUT);
    digitalWrite(LED[i], HIGH);
    delay(1000);
    digitalWrite(LED[i], LOW);
  }

  uint8_t i = 0;
  Buttons[i++].initialize("BTN_HDG_SYNC", 31, 10);
  Buttons[i++].initialize("BTN_ALT_SYNC", 50, 10);
  Buttons[i++].initialize("BTN_AP", 33, 10);
  Buttons[i++].initialize("BTN_FD", 41, 10);
  Buttons[i++].initialize("BTN_HDG", 47, 10);
  Buttons[i++].initialize("BTN_ALT", 51, 10);
  Buttons[i++].initialize("BTN_NAV", 14, 10);
  Buttons[i++].initialize("BTN_VNAV", 37, 10);  
  Buttons[i++].initialize("BTN_APR", 39, 10);
  Buttons[i++].initialize("BTN_VS", 23, 10);
  Buttons[i++].initialize("BTN_IAS", 35, 10);
  Buttons[i++].initialize("BTN_YD", 29, 10);
  Buttons[i++].initialize("BTN_LVL", 27, 10);
  Buttons[i++].initialize("BTN_TRK", 43, 10);
  
  Encoders[0] = new QuadPulseRotaryHandler{ENC_HDG, "ENC_HDG_DN\n", "ENC_HDG_UP\n"};
  Encoders[1] = new QuadPulseRotaryHandler{ENC_TRK, "ENC_TRK_DN\n", "ENC_TRK_UP\n"};
  Encoders[2] = new QuadPulseRotaryHandler{ENC_ALT, "ENC_ALT_DN\n", "ENC_ALT_UP\n"};
  Encoders[3] = new QuadPulseRotaryHandler{ENC_VS, "ENC_Wheel_UP\n", "ENC_Wheel_DN\n"};

  Encoders[1]->skip = true;
}

unsigned long lastKeepAlive = 0;
unsigned long lastKeepAliveLong = 0;

void loop() {

  //  \####RealSimGear#RealSimGear-GFC500#4#3.2.4#756E6B776F08B/   This is the RSG identifier.

  // keep alive for RSG connection
  if(millis() - lastKeepAliveLong > 3000 ) {
    Serial.write("\\####RealSimGear#RealSimGear-GFC500#4#3.2.4#756E6B776F08B/\n"); // 3.1.9 = latest firmware; 756E6B776F06E = RANDOM ID
    lastKeepAliveLong = millis();
    lastKeepAlive = millis();
  }
  if(millis() - lastKeepAlive > 100 ) {
    Serial.write("\n"); 
    lastKeepAlive = millis();
  }

  //check for incoming serial data that tells us which LEDs to light
   handleLEDData(); 

  // check for non-mux buttons pressed/released
  for(uint8_t i=0; i<NUM_BUTTONS; i++){
      ToggleButtonUpdate(Buttons[i]);
  }

  Encoders[1]->skip = !TRKmode;
  Encoders[0]->skip = TRKmode; //If we're in Track mode, skip the HDG knob update.

 
  //update encoders
  for (uint8_t i = 0; i < NUM_ENCODERS; ++i) {
    if(! Encoders[i]->skip)  Encoders[i]->update();
  }
}
