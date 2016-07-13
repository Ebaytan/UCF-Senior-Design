// WeightInterface.h

#ifndef _WEIGHTINTERFACE_h
#define _WEIGHTINTERFACE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#endif

class Weight {
public:
	/**
	* Will initialize the ADC pins used for the weight sensors
	*/
	Weight(int Sensor1, int Sensor2, int Sensor3, int Sensor4);

	/**
	* Adds up the total weight
	* among all the load sensors
	*/
	float getTotalWeight();

	/**
	* Sets the start weight when the roast process begins
	*/
	void setStartWeight(float weight);

	/**
	* Gets the start weight when the roast began
	*/
	float getStartWeight();

	/**
	* If the current bean weight is a set percentage weight of it's starting
	* point then turn off the roaster this is a high likelihood of a fire starting
	*/
	bool isAtDangerousWeight();

private:
	int8_t Sensor1Pin, Sensor2Pin, Sensor3Pin, Sensor4Pin;
	float startingWeight; //holds starting weight, necessary for calculations
	
};
