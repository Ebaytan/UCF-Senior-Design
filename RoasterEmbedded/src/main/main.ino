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

#include <ArduinoJson.h>
#include <ccspi.h>
#include <Adafruit_CC3000_Server.h>
#include <Adafruit_CC3000.h>
#include "WifiData.h"
#include "WeightInterface.h"
#include "HeatControl.h"
#include <Wire.h>
#include <SPI.h>
#include <max6675.h>
#include <registers.h>
#include <Adafruit_TFTLCD.h>
#include <Adafruit_STMPE610.h>
#include <Adafruit_ILI9341.h>
#include <gfxfont.h>
#include <Adafruit_GFX.h>
#include "Views.h"

View myView = View();
ViewState myViewState = ViewState();
HeatControl hControl = HeatControl();
bool connected = false;

int psuedoRTC = 0;

void setup(void)
{

	Serial.println("Roaster initializing...");

	myView.initView(); //initializes the values and objects necessary for the display

	pinMode(30, OUTPUT);
	digitalWrite(30, LOW);

	//myView.updateTemp(hControl.getTemp());  //update temp on screen
	Serial.println("");  
}

void loop() {

	WifiData client;
	if (!connected) {
		Serial.println("Connected set to false!");
		client = WifiData();
		connected = true;
	}
	
	//set commands to pending first grab, use if / else statements
	//if (psuedoRTC / 10 == 1) {

	if (hControl.getCheckWifiOk()) {
		Serial.println("Checking for new command");
		client.roasterStatus(); //check for new command
		hControl.setCheckWifiOk(false);
	}
	
	Serial.print("Roast Name "); Serial.println(client.getroastName());
	Serial.print("Roast Type "); Serial.println(client.getroastType());
	Serial.print("Bean Type "); Serial.println(client.getbeanType());
	
	if (client.getroasterState() == "start-pending") {
		Serial.println("current roast state is being set to start-pending");
		myView.setView(ViewState::Predefined, client.getroastName(), client.getroastType(), client.getbeanType(), client.gettargetTemp());
		client.setroasterState("start");
	}
	//add live pending and stop pending
	
	if (myView.currentState == ViewState::Predefined) {
		myView.updateTemp(hControl.getTemp());  //update temp on screen
		hControl.updateHeatingElement();		//update heating elements if necessary
	}

	//psuedoRTC++;
		
}

