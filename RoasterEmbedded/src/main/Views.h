// Views.h

#ifndef _VIEWS_h
#define _VIEWS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#endif

class View {
public:
	View ();

	/**
	* Will initialize the lcd and touch objects
	* and start up initial view.
	*/
	void initView();

	/*
	* Used when the user would like to 
	* control the roaster live
	*/
	void liveRoastView();

	/*
	* Used when the user selects a predefined
	* roast profile
	*/
	void preDefinedRoastView();

	/**
	Will update the given field with the current value

	type 0 decrease temp by 5 degrees
	type 1 increase temp by 5 degrees
	type 2 decrease fan rate by 5 percent
	type 3 increase fan rate by 5 percent
	*/
	void updateValuesButton(unsigned int type);
	
	/**
	* Will check for button presses and relevant data for 
	* the live roast view
	*/
	void View::liveRoastViewCheck();

	/**
	* Will change the view of the lcd.  For example if we click the home button
	* we would change the ViewState to Home
	*/
	void View::setView(ViewState setTo);


private:
	int8_t sclk, miso, cs;
	uint8_t spiread(void);
};
