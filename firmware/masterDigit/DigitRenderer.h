/*

MPCountdown - An Atmel328 based multi digits LED display count down project
www.EtherMania.com - Marco Signorini - September 2012 

This code is provided under the Creative Commons Attribution License. More information can be found here:
http://creativecommons.org/licenses/by/3.0/

(Use this code freely! Please just remember to give us credit where it's due. Thanks!)

*/


#ifndef _DIGITRENDERER_H_
#define _DIGITRENDERER_H_

#include "Arduino.h"

class DigitRenderer {

	private: 
		unsigned char currentValue;
		unsigned char renderedValue;
		unsigned char dimmerMode;
		unsigned char bitMaskCount;
		boolean blank;

	protected:
		virtual unsigned char translate(unsigned char value) = 0;
		virtual unsigned char translateDp() = 0;
		unsigned char getBitField();

	public:
		DigitRenderer();
		void update(unsigned char value);
		void setDp(boolean lit);
		unsigned char getValue();
		void blankDigit(boolean yes);

		void cicleDimmeringMode();
		virtual void refresh();
};

#endif /* _DIGITRENDERER_H_ */


