/*
	Button - a small library for Arduino to handle button debouncing
	
	MIT licensed.
*/

#ifndef Button_h
#define Button_h
#include "Arduino.h"

class Button
{
	public:
    Button() {};
		Button(const String btn_name, uint8_t pin, uint16_t debounce_ms = 100);
    void initialize(const String btn_name, uint8_t pin, uint16_t debounce_ms);
		void begin();
		char read();
		bool toggled();
		bool pressed();
		bool held();
		bool released();
		bool has_changed();
    String get_name();
		
		const static bool PRESSED = LOW;
		const static bool RELEASED = HIGH;
	
	private:
		uint8_t  _pin;
		uint16_t _delay;
		bool     _state;
		bool     _has_changed;
		uint32_t _ignore_until;
    String   _name;
};

#endif
