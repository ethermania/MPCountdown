/*

MPCountdown - An Atmel328 based multi digits LED display count down project
www.EtherMania.com - Marco Signorini - September 2012 

This code is provided under the Creative Commons Attribution License. More information can be found here:
http://creativecommons.org/licenses/by/3.0/

(Use this code freely! Please just remember to give us credit where it's due. Thanks!)

*/


#include "Arduino.h"
#include "ShiftRegister.h"

// From digit number to LED array table conversion
const unsigned char ShiftRegister595::renderer[10] = { 0xBE, 0x12, 0x7B, 0x76, 0xD8, 0xE6, 0xCE, 0x32, 0xFE, 0xF2, };

ShiftRegister595::ShiftRegister595(unsigned char _dataPin, unsigned char _clockPin, unsigned char _strobePin) : DigitRenderer() {

	dataPin = _dataPin;
	clockPin = _clockPin;
	strobePin = _strobePin;
}

ShiftRegister595::~ShiftRegister595() {
}

unsigned char ShiftRegister595::translate(unsigned char value) {
	return renderer[value];
}

void ShiftRegister595::refresh() {
        DigitRenderer::refresh();
        
        unsigned char bitField = getBitField();
        
        digitalWrite(strobePin, LOW);
        shiftOut(dataPin, clockPin, LSBFIRST, bitField);
        digitalWrite(strobePin, HIGH);
}

void ShiftRegister595::clock() {

	digitalWrite(clockPin, true);
	digitalWrite(clockPin, false);
}

void ShiftRegister595::strobe() {

	digitalWrite(strobePin, true);
	digitalWrite(strobePin, false);
}


