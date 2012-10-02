/*

MPCountdown - An Atmel328 based multi digits LED display count down project
www.EtherMania.com - Marco Signorini - September 2012 

This code is provided under the Creative Commons Attribution License. More information can be found here:
http://creativecommons.org/licenses/by/3.0/

(Use this code freely! Please just remember to give us credit where it's due. Thanks!)

*/


#include "DigitRenderer.h"

static const unsigned char multiplexMask[4] = { 0x05, 0x0A, 0x50, 0xA0 };

DigitRenderer::DigitRenderer() {

	currentValue = 0;
	renderedValue = 0;
	bitMaskCount = 0;
	blank = false;
}

void DigitRenderer::update(unsigned char value) {

	currentValue = value;
	if ((value >= 0) && (value<=9)) {
		renderedValue = translate(value); 
	} else
		renderedValue = 0x00;
}

unsigned char DigitRenderer::getValue() {
	return currentValue;
}

void DigitRenderer::blankDigit(boolean yes) {
	blank = yes;
}

void DigitRenderer::refresh() {

	bitMaskCount++;
	bitMaskCount &= 0x03;
}

unsigned char DigitRenderer::getBitField() {

	return (blank)? 0x00 : (renderedValue & multiplexMask[bitMaskCount]);
}

