/*

MPCountdown - An Atmel328 based multi digits LED display count down project
www.EtherMania.com - Marco Signorini - September 2012 

This code is provided under the Creative Commons Attribution License. More information can be found here:
http://creativecommons.org/licenses/by/3.0/

(Use this code freely! Please just remember to give us credit where it's due. Thanks!)

*/


#ifndef _LATCHED_H_
#define _LATCHED_H_

#include "DigitRenderer.h"

class Latched : public DigitRenderer {

	private:
		static const unsigned char renderer[10];

	public:
		Latched();
		~Latched();
		void refresh();

	protected:
		unsigned char translate(unsigned char value);
};

#endif /* _LATCHED_H_ */


