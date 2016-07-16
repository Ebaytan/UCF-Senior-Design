// HeatControl.h

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


};