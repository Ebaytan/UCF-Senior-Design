// WifiData.h

#ifndef _WIFIDATA_h
#define _WIFIDATA_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#endif

class WifiData {
public:
	WifiData();

	/*
		Will do an HTTP GET request to the Web API
		and get the current state of the roast in
		JSON format.
	*/
	void roasterStatus();

	String getroastName();
	void setroastName(String roastName);

	String getbeanType();
	void setbeanType(String beanType);

	String getroastType();
	void setroastType(String roastType);

	int gettargetTemp();
	void settargetTemp(int targetTemp);

	String getroasterState();
	void setroasterState(String state);

private:
	String roastName;
	String beanType;
	String roastType;
	int targetTemp;
	String roasterState;
};