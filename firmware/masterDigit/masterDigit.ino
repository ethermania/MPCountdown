/*

MPCountdown - An Atmel328 based multi digits LED display count down project
www.EtherMania.com - Marco Signorini - September 2012 

This code is provided under the Creative Commons Attribution License. More information can be found here:
http://creativecommons.org/licenses/by/3.0/

(Use this code freely! Please just remember to give us credit where it's due. Thanks!)

*/


#include "DigitRenderer.h"
#include "Latched.h"
#include "ShiftRegister.h"

#define CLOCKPIN	7
#define DATAPIN		9
#define STROBEPIN	8

#define MODE_BUTTON	0
#define UP_BUTTON	1

#define NUMDIGITS	9
#define HOURUNITDIGIT   4
#define HOURTENDIGIT    5
#define MODE_COUNTDOWN  NUMDIGITS
#define MODE_RUN	(MODE_COUNTDOWN+1)

#define BUTTONDEBOUNCE  400
#define BLINKTIME	200

// All digit renderers are stored in an array for easy access
DigitRenderer* digits[NUMDIGITS];

// Factors needed to convert seconds in DDDHHMMSS
// and to setup each digit when in set mode
const unsigned long factors[NUMDIGITS] = { 1, 10, 60, 600, 3600, 36000, 86400, 864000, 8640000 };
const unsigned char modfactors[NUMDIGITS] = { 10, 6, 10, 6, 10, 3, 10, 10, 10 };

// The time to be shown is stored in seconds
unsigned long timeCounter;

// Working mode flags
bool countDown;
bool runningMode;
bool digitBlank;
unsigned char setupDigit;

// Other required program variables
unsigned long lastTime;
unsigned long buttonDebounceTime[2];

void setup() {
  
	// Allocate digit renderers
	for (unsigned char i = 0; i < NUMDIGITS-1; i++) {
		digits[i] = new ShiftRegister595(DATAPIN,CLOCKPIN,STROBEPIN, (i==(NUMDIGITS-2)));
	}
	digits[NUMDIGITS-1] = new Latched();

        // Initialize program variables
	timeCounter = 0;
	countDown = true;
	runningMode = false;
	digitBlank = false;
	setupDigit = 0;

	lastTime = millis();
	buttonDebounceTime[0] = millis();
	buttonDebounceTime[1] = millis();

        // Initialilze hardware pins
	pinMode(CLOCKPIN, OUTPUT);
	pinMode(DATAPIN, OUTPUT);
	pinMode(STROBEPIN, OUTPUT);

	// Start rendering digits
	updateDigits(timeCounter);
}


void loop() {
  
  	boolean forceRefresh = false;

	if (runningMode)
		forceRefresh = normalRun();
	else
		forceRefresh = setupMode();

	// Digits multiplex refresh
	for (unsigned char i = 0; i < NUMDIGITS; i++)
		digits[i]->refresh();
}

// ----------------------------------------------------------------------------

boolean normalRun() {

	// Checks if user presses the MODE button
	// and changes the run mode accordingly
	bool modePressed = checkButton(MODE_BUTTON);
	if (modePressed) {
  
		lastTime = millis();
		runningMode = false;
		
		return false;
	}

	// Check if user presses the UP button
	// and changes the digit value accordingly
	boolean upPressed = checkButton(UP_BUTTON);
	if (upPressed) {

 		// Cicle through possible dimmer modes
 		for (unsigned char i = 0; i < NUMDIGITS; i++)
 			digits[i]->cicleDimmeringMode();
	}

	long diff = millis() - lastTime;
	if (abs(diff) > 1000) {
		lastTime += 1000;

		// Counts up or down as requested
		if (countDown)
			timeCounter--;
		else
			timeCounter++;

		// When the counter reaches the zero, 
		// we can start to count up
		if (timeCounter == 0)
			countDown = false;

		// Updates the renderes with the new value
		updateDigits(timeCounter);

		return true;
	}

	return false;
}

// ----------------------------------------------------------------------------

boolean setupMode() {

	// Checks if user presses the MODE button
	// and changes the run mode accordingly
	bool modePressed = checkButton(MODE_BUTTON);
	if (modePressed) {
  
		// Unblanks all digits...
		for (int i = 0; i < NUMDIGITS; i++) {
			digits[i]->blankDigit(false);
			digits[i]->setDp(false);
		}

		// ... goes to next digit or...
		setupDigit++;

		if (setupDigit == MODE_COUNTDOWN) {
  
  			// ... goes to the count up/down mode selection or...
			digits[0]->setDp(true);
			digits[NUMDIGITS-1]->setDp(true);
  		}

		if (setupDigit == MODE_RUN) {
  
  			// ... to the normal operation...
  			timeCounter = 0;

  			// ...Calculate the new countdownTime...
  			for (int i = 0; i < NUMDIGITS; i++) {
  			  	unsigned long factor = factors[i];
    			  	unsigned char curDigitVal = digits[i]->getValue();
      			  	timeCounter += factor * curDigitVal;
			}

			// ... and starts the normal operation mode
			lastTime = millis();
			setupDigit = 0;
			digitBlank = false;
			runningMode = true;
			return false;
		}
	}

	// Check if user presses the UP button
	// and changes the digit value accordingly
	boolean upPressed = checkButton(UP_BUTTON);
	if (upPressed) {
  
		if (setupDigit < MODE_COUNTDOWN) {  
  
    	    		unsigned char currDigitValue = digits[setupDigit]->getValue();
    
    	    		currDigitValue++;
    	     		if (currDigitValue == modfactors[setupDigit])
    	      		    	currDigitValue = 0;    
    
    	  		// Updates the renderer with the new value
       	  		digits[setupDigit]->update(currDigitValue);
           	} else if (setupDigit == MODE_COUNTDOWN) {
             
           		countDown = !countDown;
           	}
	}

	// Blinks current digit
	long diff = millis() - lastTime;
	if (abs(diff) > BLINKTIME) {
		lastTime += BLINKTIME;

		unsigned char digit = setupDigit;
		if (setupDigit == MODE_COUNTDOWN) {
  			if (countDown)
  				digit = 0;
  			else
  				digit = NUMDIGITS-1;
  		}
		digitBlank = !digitBlank;
		digits[digit]->blankDigit(digitBlank);

		return true;
	}

	return false;
}

// ----------------------------------------------------------------------------

bool checkButton(unsigned char buttonId) {
  
	long diff = millis() - buttonDebounceTime[buttonId];
	if (abs(diff) > BUTTONDEBOUNCE) {
  		buttonDebounceTime[buttonId] = millis();

		int buttonVal = analogRead(buttonId);
		if (buttonVal < 200)
			return true;
		else
			buttonDebounceTime[buttonId]-=BUTTONDEBOUNCE+1;
	}
	return false;
}

// ----------------------------------------------------------------------------

void updateDigits(unsigned long currentTime) {
  
  for (unsigned char i = NUMDIGITS; i > 0; i--) {
    
    unsigned char value = (currentTime/factors[i-1]) % modfactors[i-1];
    digits[i-1]->update(value);
  }

  // Fix hours calculations
  unsigned char hours = (currentTime/3600) % 24;
  digits[HOURUNITDIGIT]->update(hours%10);
  digits[HOURTENDIGIT]->update(hours/10);
}


