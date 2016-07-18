// WifiData.h

//#include "Views.h"

#ifndef _WIFI_h
#define _WIFI_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#endif

class WifiData {
public:
	/**
	* Will initialize the heating element object and set pins as
	* INPUT or OUTPUT as necessary
	*/
	WifiData();

	/*
		Will alter the heating element depending on the difference
		between the current temperature and the target temperature
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

private:
  String roastName;
  String beanType;
  String roastType;
  int targetTemp;
};
