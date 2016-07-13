/*
  Main entry file for RoastRight embedded system
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
  myViewState = ViewState::Live;
  myView.setView(myViewState);
  
  //set pin 6 to 62500 / 2 Hz = 31.25 kHz
  //setPwmFrequency(6,2);

  //Serial.println("Set pin 6's frequency to 31250");
}

void loop() {

	//if(myView-)

	myView.liveRoastViewCheck();
	//hControl.updateHeatingElement();

	//check the view type
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


