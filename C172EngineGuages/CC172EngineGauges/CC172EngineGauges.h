#pragma once

#include "Arduino.h"
#include "4inchLCDConfig.h"

class CC172EngineGauges
{
public:
    CC172EngineGauges(uint8_t Pin1, uint8_t Pin2);
    void begin();
    void attach(uint16_t Pin3, char *init);
    void detach();
    void set(int16_t messageID, char *setPoint);
    void update();
    bool isValidNumber(const char *str);

private:
    bool    _initialised;
  //  uint8_t _pin1, _pin2, _pin3;

};