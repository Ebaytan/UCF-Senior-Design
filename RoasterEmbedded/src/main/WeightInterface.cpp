// 
// 
// 

#include "WeightInterface.h"

#define WEIGHT_SHUTDOWN_PERCENTAGE .6

Weight::Weight(int Sensor1, int Sensor2, int Sensor3, int Sensor4)
{
	this->Sensor1Pin = Sensor1;
	this->Sensor2Pin = Sensor2;
	this->Sensor3Pin = Sensor3;
	this->Sensor4Pin = Sensor4;
}

float Weight::getTotalWeight()
{
	//Note: Max adc value is 1023 == 5 V

	float tempWeight;
	float tempADCVal;
	float tempVoltage;

	//get weight on load 1
	tempADCVal = analogRead(Sensor1Pin);
	tempVoltage = (tempADCVal / 1023) * 5; //voltage, max = 5
	tempVoltage /= 1000; //convert into mV
	tempWeight += (tempVoltage - 14.77) / .48;

	//get weight on load 2
	tempADCVal = analogRead(Sensor2Pin);
	tempVoltage = (tempADCVal / 1023) * 5; //voltage, max = 5
	tempVoltage /= 1000; //convert into mV
	tempWeight += (tempVoltage - 14.93) / .57;

	//get weight on load 3
	tempADCVal = analogRead(Sensor3Pin);
	tempVoltage = (tempADCVal / 1023) * 5; //voltage, max = 5
	tempVoltage /= 1000; //convert into mV
	tempWeight += (tempVoltage - 14.98) / .57;

	//get weight on load 3
	//tempADCVal = analogRead(Sensor4Pin);
	//tempVoltage = (tempADCVal / 1023) * 5; //voltage, max = 5
	//tempVoltage /= 1000; //convert into mV
	//tempWeight += (tempVoltage - 14.93) / .57;


	return tempWeight;

}

void Weight::setStartWeight(float weight)
{
	this->startingWeight = weight;
}

float Weight::getStartWeight()
{
	return this->startingWeight;
}

bool Weight::isAtDangerousWeight()
{

	if (this->getTotalWeight() <= this->getStartWeight() * WEIGHT_SHUTDOWN_PERCENTAGE) return true;

	return false;

}
