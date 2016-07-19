/*
  Main entry file for RoastRight embedded system

Flow:  
Mode = idle: (Default when system turns on)
	1) Setup pins as input and output
	Loop:
	2) Change view to the idle view
	3) check if new command has been received
		a. No new command received
			i. continue current mode
		b. New command received
			i. Command is not start
				a. If mode is idle
					1. go back to 3).
			ii. Command is start
				a. Parse data and populate wifi data object
				b. Change view to roast in progress
				c. Change mode to roasting (look below)

Mode = roasting:
	1) Update API verifying that the roast has begun
	2) Get values specific to roast (name, type, creator) and populate fields in view
	3) Turn on secondary heating element
	4) Turn on barrel motor
	Loop:
	5) Check for new command
		a. We have received new command
			i. command start
	6) Check current temperature and compare to target temp
		a. Is current temp >= 475?
			i.  Turn off heating elements
			ii.  Turn on intake and outake fans
		b. Is current temp 5 degrees above target?
			i. Yes => Turn off heating element, turn off fans (if they were on)
			ii. No => Keep heating element on or turn it back on if it was off
		c. Is current temp 30 degrees above target?
			i.  Yes => Turn on intake and outake fans
			ii. No => Continue
	7) A lot of smoke (trial and error since we don't have smoke)

*/

#include "WeightInterface.h"
#include "HeatControl.h"
#include <Wire.h>
#include <SPI.h>
#include <max6675.h>
#include <registers.h>
//#include <pin_magic.h>
#include <Adafruit_TFTLCD.h>
#include <Adafruit_STMPE610.h>
#include <Adafruit_ILI9341.h>
#include <gfxfont.h>
#include <Adafruit_GFX.h>
#include "Views.h"

View myView = View();
ViewState myViewState;
HeatControl hControl;


void setup(void)
{
  
	Serial.begin(9600); //begin serial communication

	myView.initView(); //initializes the values and objects necessary for the display
  
}

void loop() {

	if (myView.currentState == ViewState::Predefined) {
		myView.updateTemp(hControl.getTemp());
	}
	
	
}

void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if(pin == 3 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x7; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}
