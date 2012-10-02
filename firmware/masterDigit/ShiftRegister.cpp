/*

MPCountdown - An Atmel328 based multi digits LED display count down project
www.EtherMania.com - Marco Signorini - September 2012 

This code is provided under the Creative Commons Attribution License. More information can be found here:
http://creativecommons.org/licenses/by/3.0/

(Use this code freely! Please just remember to give us credit where it's due. Thanks!)

*/

#include "Arduino.h"
#include "ShiftRegister.h"

#define S_A  0x01
#define S_B  0x02
#define S_C  0x40
#define S_D  0x20
#define S_E  0x10
#define S_F  0x04
#define S_G  0x08
#define S_DP 0x80

#define S_0  (S_A | S_B | S_C | S_D | S_E | S_F)
#define S_1  (S_B | S_C)
#define S_2  (S_A | S_B | S_G | S_E | S_D)
#define S_3  (S_A | S_B | S_C | S_G | S_D)
#define S_4  (S_F | S_B | S_G | S_C)
#define S_5  (S_A | S_F | S_G | S_C | S_D)
#define S_6  (S_A | S_F | S_G | S_C | S_D | S_E)
#define S_7  (S_A | S_B | S_C)
#define S_8  (S_A | S_B | S_C | S_D | S_E | S_F | S_G)
#define S_9  (S_A | S_B | S_F | S_G | S_C | S_D)

// From digit number to LED array table conversion
const unsigned char ShiftRegister595::renderer[10] = { S_0, S_1, S_2, S_3, S_4, S_5, S_6, S_7, S_8, S_9 };

ShiftRegister595::ShiftRegister595(unsigned char _dataPin, unsigned char _clockPin, unsigned char _strobePin, boolean _lastDigit) : DigitRenderer() {

	dataPin = _dataPin;
	clockPin = _clockPin;
	strobePin = _strobePin;
	lastDigit = _lastDigit;

	if (lastDigit)
		digitalWrite(strobePin, LOW);
}

ShiftRegister595::~ShiftRegister595() {
}

unsigned char ShiftRegister595::translate(unsigned char value) {
	return renderer[value];
}

void ShiftRegister595::refresh() {
        DigitRenderer::refresh();
        
        unsigned char bitField = getBitField();
        
        //digitalWrite(strobePin, LOW);
        shiftOut(dataPin, clockPin, LSBFIRST, bitField);
        
        // The last digit in the chain is responsible for latch the output buffers
        if (lastDigit) {
          digitalWrite(strobePin, HIGH);
          digitalWrite(strobePin, LOW);
        }
}

void ShiftRegister595::clock() {

	digitalWrite(clockPin, true);
	digitalWrite(clockPin, false);
}

void ShiftRegister595::strobe() {

	digitalWrite(strobePin, true);
	digitalWrite(strobePin, false);
}


