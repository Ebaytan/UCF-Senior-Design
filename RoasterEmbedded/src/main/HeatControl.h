// HeatControl.h

//#include "Views.h"

#ifndef _HEATCONTROL_h
#define _HEATCONTROL_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#endif

class HeatControl {
public:
	/**
	* Will initialize the heating element object and set pins as
	* INPUT or OUTPUT as necessary
	*/
	HeatControl();

	/*
		Will alter the heating element depending on the difference
		between the current temperature and the target temperature
	*/
	void updateHeatingElement();

	/**
	* returns the current temperature in degrees  Farenheit 
	*/
	double getTemp();

	/*
	*	updates the current temperature in the predefined view
	*/
	void updateTemp();
	
	/*
	*	Will check which view state we are in and check the view
	*	for the necessary data to update
	*/
	void updateViewCheck(ViewState state);

private:
	bool coolMode;
};