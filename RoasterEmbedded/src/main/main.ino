/*
  Main entry file for RoastRight embedded system
*/

#include <Wire.h>
#include <SPI.h>
#include <max6675.h>
#include <registers.h>
#include <pin_magic.h>
#include <Adafruit_TFTLCD.h>
#include <Adafruit_STMPE610.h>
#include <Adafruit_ILI9341.h>
#include <gfxfont.h>
#include <Adafruit_GFX.h>
#include "Views.h"


View myView = View();


void setup(void)
{
  Serial.begin(9600); //begin serial communication

  myView.initView(); //initializes the values and objects necessary for the display

  //set pin 6 to 62500 / 2 Hz = 31.25 kHz
  setPwmFrequency(6,2);

  Serial.println("Set pin 6's frequency to 31250");
}

void loop() {

	if(myView.)

	myView.liveRoastViewCheck();

	//check the view type
}

/**
 * Will take the target temperature and calculate the necessary duty cycle and
 * set it to that value.
 * Reference: https://www.arduino.cc/en/Tutorial/PWM
 * 
 * target temperature that we would like to go to
 */
void setDutyCycle(int target){

  //from 0 to 255, 0% to 100%
  //analogWrite(TEMP_CONTROL_PIN, target/something);
  
}

/**
 * Will take the voltage across the temp sensor and
 * convert that into degrees Farenheit
 */
double getCurrentTemp(){
  //sample temperature

  return 0.0;
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

