/*

MPCountdown - An Atmel328 based multi digits LED display count down project
www.EtherMania.com - Marco Signorini - September 2012 

This code is provided under the Creative Commons Attribution License. More information can be found here:
http://creativecommons.org/licenses/by/3.0/

(Use this code freely! Please just remember to give us credit where it's due. Thanks!)

*/


#include "Latched.h"

#define L_A  0x01
#define L_B  0x02
#define L_C  0x04
#define L_D  0x08
#define L_E  0x10
#define L_F  0x20
#define L_G  0x40
#define L_DP 0x80

#define L_0  (L_A | L_B | L_C | L_D | L_E | L_F)
#define L_1  (L_B | L_C)
#define L_2  (L_A | L_B | L_G | L_E | L_D)
#define L_3  (L_A | L_B | L_C | L_G | L_D)
#define L_4  (L_F | L_B | L_G | L_C)
#define L_5  (L_A | L_F | L_G | L_C | L_D)
#define L_6  (L_A | L_F | L_G | L_C | L_D | L_E)
#define L_7  (L_A | L_B | L_C)
#define L_8  (L_A | L_B | L_C | L_D | L_E | L_F | L_G)
#define L_9  (L_A | L_B | L_F | L_G | L_C | L_D)


const unsigned char Latched::multiplexMask[4][8] = {    { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 },  // One LED at time 
							{ 0x05, 0x0A, 0x50, 0xA0, 0x00, 0x00, 0x00, 0x00 },  // Two LEDs at time (default)
							{ 0x55, 0xAA, 0x55, 0xAA, 0x00, 0x00, 0x00, 0x00 },  // Four LEDs at time
							{ 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00 },  // All LEDs at time
						    };

const unsigned char Latched::renderer[10] = { L_0, L_1, L_2, L_3, L_4, L_5, L_6, L_7, L_8, L_9 };

// We define here an association between ROM char generated bit and physical pin on the Atmel where
// the LED segment is connected. L_A is the LSb; L_DP is the MSb. Please refer to the schematics.
const unsigned char bitToPin[8] = { 5, 6, 0, 1, 2, 4, 3, 10 };

Latched::Latched() : DigitRenderer() {

	// Set all required bit as OUTPUT
	for (unsigned char i = 0; i < 8; i++) {
  		unsigned char pin = bitToPin[i];
		pinMode(pin, OUTPUT);
	}
}

Latched::~Latched() { 
}

unsigned char Latched::translate(unsigned char value) {
	return renderer[value];
}

unsigned char Latched::getDimmerMask(unsigned char dimmerMode, unsigned char dimmerStep) {
	return multiplexMask[dimmerMode][dimmerStep];  
}

unsigned char Latched::translateDp() {
	return L_DP;
}

void Latched::refresh() {
	DigitRenderer::refresh();

        unsigned char bitField = getBitField();
        unsigned char bitMask = 1;
        for (unsigned char i = 0; i < 8; i++) {
          
          unsigned char pin = bitToPin[i];
          byte value = (bitField & bitMask)? HIGH:LOW;
          
          digitalWrite(pin, value);
          
          bitMask <<= 1;
        }
}


