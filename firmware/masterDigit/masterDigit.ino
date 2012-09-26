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
#define REFRESHTIME	5

// All digit renderers are stored in an array for easy access
DigitRenderer* digits[NUMDIGITS];

// Factors needed to convert seconds in DDDHHMMSS
// and to setup each digit when in set mode
const unsigned long factors[NUMDIGITS] = { 8640000, 864000, 86400, 36000, 3600, 600, 60, 10, 1 };
const unsigned long modfactors[NUMDIGITS] = { 10, 10, 10, 3, 4, 6, 10, 6, 10 };

// The time to be shown is stored in seconds
unsigned long timeCounter;

// Working mode flags
bool countDown;
bool runningMode;
bool digitBlank;
unsigned char setupDigit;

// Other required program variables
unsigned long lastTime;
unsigned long lastRefreshTime;

void setup() {
  
	// Allocate digit renderers
	digits[0] = new Latched();
	for (unsigned char i = 1; i < NUMDIGITS; i++) {
		digits[i] = new ShiftRegister595(DATAPIN,CLOCKPIN,STROBEPIN);
	}

        // Initialize program variables
	timeCounter = 0;
	countDown = true;
	runningMode = false;
	digitBlank = false;
	setupDigit = 0;

	lastTime = millis();
	lastRefreshTime = millis();

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
	long diffRefresh = millis() - lastRefreshTime;
	if (forceRefresh || (abs(diffRefresh) > REFRESHTIME)) {
		lastRefreshTime = millis();
	
		for (unsigned char i = 0; i < NUMDIGITS; i++)
			digits[i]->refresh();
	}
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
  
		// Unblank all digits...
		for (int i = 0; i < NUMDIGITS; i++)
			digits[i]->blankDigit(false);

		setupDigit++;
		if (setupDigit == NUMDIGITS) {

			// ... and starts the normal operation mode
			lastTime = millis();
			setupDigit = 0;
			digitBlank = false;
			countDown = true;
			runningMode = true;
			return false;
		}
	}

	// Check if user presses the UP button
	// and changes the digit value accordingly
	bool upPressed = checkButton(UP_BUTTON);
	if (upPressed) {
  
  		unsigned char currDigitValue = digits[setupDigit]->getValue();
		unsigned int amount = factors[setupDigit];
		timeCounter -= currDigitValue * amount;
		currDigitValue++;
		if(currDigitValue == modfactors[setupDigit])
			currDigitValue = 0;

		timeCounter += currDigitValue * amount;

		// Updates the renderers with the new value
		updateDigits(timeCounter);
	}

	// Blinks current digit
	long diff = millis() - lastTime;
	if (abs(diff) > 1000) {
		lastTime += 1000;

		digitBlank = !digitBlank;
		digits[setupDigit]->blankDigit(digitBlank);

		return true;
	}

	return false;
}

// ----------------------------------------------------------------------------

bool checkButton(unsigned char buttonId) {

	int buttonVal = analogRead(buttonId);
	if (buttonVal < 200)
		return true;

	return false;
}

// ----------------------------------------------------------------------------

void updateDigits(unsigned long currentTime) {
  
  for (unsigned char i = 0; i < NUMDIGITS; i++) {
    
    unsigned char value = (currentTime/factors[i]) % modfactors[i];
    digits[i]->update(value);
  }
}


