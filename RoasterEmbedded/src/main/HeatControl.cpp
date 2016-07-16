// 
// 
// 

#include "HeatControl.h"
#include <max6675.h>

#define thermoDO  4
#define thermoCS  5
#define thermoCLK 6

#define TARGET_TEMP 250
#define TEMP_CONTROL_PIN 3

//heating element pins
#define HEATING_1 37
#define HEATING_2 44

// ThermoCouple
#define thermo_gnd_pin 45
#define thermo_vcc_pin 47
#define thermo_so_pin 49
#define thermo_cs_pin 23
#define thermo_sck_pin 53

MAX6675 thermocouple(thermo_sck_pin, thermo_cs_pin, thermo_so_pin);

HeatControl::HeatControl() {
	// configure pins for temp sensor
	pinMode(thermo_vcc_pin, OUTPUT);
	pinMode(thermo_gnd_pin, OUTPUT);
	digitalWrite(thermo_vcc_pin, HIGH);
	digitalWrite(thermo_gnd_pin, LOW);
	digitalWrite(thermo_cs_pin, HIGH);
	pinMode(HEATING_1, OUTPUT); //heating 1 
	pinMode(HEATING_2, OUTPUT); //heating 2
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
	//off the heating elements.
	if (currentTemp >= 475) {
		analogWrite(HEATING_2, 0);
		analogWrite(HEATING_1, 0);
	}
	//if our current temperature is above
	//our target temp by 5 degrees or more
	//we will shut down the heating element until
	//that condition no longer applies
	else if (currentTemp >= TARGET_TEMP + 5) {

		analogWrite(HEATING_2, 0);
		analogWrite(HEATING_1, 0);

		//set duty cycle to 0
		setDutyCycle(0);
		Serial.println("Set heating element off!");

		//TODO: turn off fan
	}
	else if (currentTemp > TARGET_TEMP + 10) {
		//turn on fan
		analogWrite(HEATING_2, 255);
	}
	else {
		setDutyCycle(255);
		Serial.println("Heating element has been turned on!");
	}
}