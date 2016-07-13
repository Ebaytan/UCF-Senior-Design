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
	HeatControl();

	/**
	* Will initialize the heating element
	*/
	void initHeatControl();

	/*
		Will alter the heating element depending on the difference
		between the current temperature and the target temperature
	*/
	void updateHeatingElement();

	void setDutyCycle(int onOff);

};