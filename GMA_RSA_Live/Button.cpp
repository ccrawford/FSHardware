/*
	Button - a small library for Arduino to handle button debouncing
	
	MIT licensed.
*/

#include "Button.h"
#include <Arduino.h>

Button::Button(const String btn_name, uint8_t pin, uint16_t debounce_ms)
:  _name(btn_name)  
,  _pin(pin)
,  _delay(debounce_ms)
,  _state(HIGH)
,  _ignore_until(0)
,  _has_changed(false)
{
}

void Button::initialize(const String btn_name, uint8_t pin, uint16_t debounce_ms)
{
	// Initialize class
	this->_name = btn_name; 
	this->_pin = pin;
	this->_delay = debounce_ms;
	this->_state = HIGH;
	this->_ignore_until = 0;
	this->_has_changed = false;

	// Call begin
	this->begin();
}

void Button::begin()
{
	pinMode(_pin, INPUT_PULLUP);
}

// 
// public methods
// 

char Button::read()
{
	// ignore pin changes until after this delay time
	if (_ignore_until > millis())
	{
		// debouncing
		return -1;
	}
	
	// pin has changed 
	else if (digitalRead(_pin) != _state)
	{
		_ignore_until = millis() + _delay;
		_state = !_state;
		_has_changed = true;
	}
	
	return _state;
}

// has the button been toggled from on -> off, or vice versa
bool Button::toggled()
{
	read();
	return has_changed();
}

// mostly internal, tells you if a button has changed after calling the read() function
bool Button::has_changed()
{
	if (_has_changed == true)
	{
		_has_changed = false;
		return true;
	}
	return false;
}

// has the button gone from off -> on
bool Button::pressed()
{
	if (read() == PRESSED && has_changed() == true)
		return true;
	else
		return false;
}

bool Button::held()
{
	return (read() == PRESSED);
}

// has the button gone from on -> off
bool Button::released()
{
	if (read() == RELEASED && has_changed() == true)
		return true;
	else
		return false;
}

// Return the name of the button
String Button::get_name()
{
	return this->_name;
}