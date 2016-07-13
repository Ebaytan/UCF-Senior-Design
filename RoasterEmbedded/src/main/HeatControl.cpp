// 
// 
// 

#include "HeatControl.h"
#include <max6675.h>

int thermoDO = 4;
int thermoCS = 5;
int thermoCLK = 6;

#define TARGET_TEMP 250
#define TEMP_CONTROL_PIN 3

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);
int vccPin = 3;
int gndPin = 2;

HeatControl::HeatControl() {
	// use Arduino pins
	pinMode(vccPin, OUTPUT); digitalWrite(vccPin, HIGH);
	pinMode(gndPin, OUTPUT); digitalWrite(gndPin, LOW);

	// wait for MAX chip to stabilize
	delay(500);
}

void HeatControl::initHeatControl()
{
}

void HeatControl::updateHeatingElement()
{
	//sample temp
	int currentTemp = thermocouple.readFahrenheit();

	//print to the serial monitor the current temperature data
	Serial.print("Current temp: "); Serial.println(currentTemp);
	Serial.print("Target temp: "); Serial.println(TARGET_TEMP);

	//Safety:
	//If the temperature meets or exceeds 475 degrees Farenheit turn
	//off the heating element.
	if (currentTemp >= 475) {
		setDutyCycle(0);
	}
	//if our current temperature is above
	//our target temp by 5 degrees or more
	//we will shut down the heating element until
	//that condition no longer applies
	else if (currentTemp >= TARGET_TEMP + 5) {
		//set duty cycle to 0
		setDutyCycle(0);
		Serial.println("Set heating element off!");

		//TODO: turn off fan
	}
	else if (currentTemp > TARGET_TEMP + 10) {
		//turn on fan
	}
	else {
		setDutyCycle(255);
		Serial.println("Heating element has been turned on!");
	}
}

void HeatControl::setDutyCycle(int onOff)
{
	analogWrite(TEMP_CONTROL_PIN, onOff);
}
