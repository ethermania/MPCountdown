/*

MPCountdown - An Atmel328 based multi digits LED display count down project
www.EtherMania.com - Marco Signorini - September 2012 

This code is provided under the Creative Commons Attribution License. More information can be found here:
http://creativecommons.org/licenses/by/3.0/

(Use this code freely! Please just remember to give us credit where it's due. Thanks!)

*/


#ifndef _SHIFTREGISTER_H_
#define _SHIFTREGISTER_H_

#include "DigitRenderer.h"

class ShiftRegister595 : public DigitRenderer {

	private:
		static const unsigned char renderer[10];
		unsigned char dataPin;
		unsigned char clockPin;
		unsigned char strobePin;
		boolean lastDigit;

	public:
		ShiftRegister595(unsigned char _dataPin, unsigned char _clockPin, unsigned char _strobePin, boolean _lastDigit);
		~ShiftRegister595();
		void refresh();

	protected:
		unsigned char translate(unsigned char value);
		virtual unsigned char translateDp();

	private:
		void clock();
		void strobe();
};


#endif /* _SHIFTREGISTER_H_ */


