// Views.h

#ifndef _VIEWS_h
#define _VIEWS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#endif

enum class ViewState {
	Live,  // not used in project, but is being used as a gui example
	Predefined, //when start roast
	Home //idle
};

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
	void preDefinedRoastView(String rname, String rtype, String rbeantype, int targetTemp);

	/**
	Will update the given field with the current value

	type 0 decrease temp by 5 degrees
	type 1 increase temp by 5 degrees
	type 2 decrease fan rate by 5 percent
	type 3 increase fan rate by 5 percent
	*/
	void updateValuesButton(unsigned int type);

	/**
	* The initial view shown when the
	* roaster starts up
	*/
	void homeRoastView();
	
	/**
	* Will check for button presses and relevant data for 
	* the live roast view
	*/
	//void liveRoastViewCheck();

	/**
	* Will change the view of the lcd.  For example if we click the home button
	* we would change the ViewState to Home
	*/
	void setView(ViewState setTo);
	void setView(ViewState setTo, String rname, String rtype, String rbeantype, int targetTemp);
	void updateTemp(double temp);
	ViewState currentState;

private:


};
