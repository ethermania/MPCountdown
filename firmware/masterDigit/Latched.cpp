/*

MPCountdown - An Atmel328 based multi digits LED display count down project
www.EtherMania.com - Marco Signorini - September 2012 

This code is provided under the Creative Commons Attribution License. More information can be found here:
http://creativecommons.org/licenses/by/3.0/

(Use this code freely! Please just remember to give us credit where it's due. Thanks!)

*/


#include "Latched.h"

const unsigned char Latched::renderer[10] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09 };

Latched::Latched() : DigitRenderer() {
}

Latched::~Latched() { 
}

unsigned char Latched::translate(unsigned char value) {
	return renderer[value];
}

void Latched::refresh() {
	DigitRenderer::refresh();
}


