// 
// 
// 

#include "HeatControl.h"
#include <Adafruit_TFTLCD.h>
#include <Adafruit_STMPE610.h>
#include <Adafruit_ILI9341.h>

#include <max6675.h>

#define TARGET_TEMP 268
#define TEMP_CONTROL_PIN 3

//heating element pins
#define HEATING_1_PIN 37 //smoke suppression element
#define HEATING_2_PIN 29

#define BARREL_PIN 28 //barrel

// ThermoCouple
#define thermo_gnd_pin 45
#define thermo_vcc_pin 47
#define thermo_so_pin 49
#define thermo_cs_pin 22 //was 23 but needed to be changed
#define thermo_sck_pin 53

//Fan pins
#define OUTAKE_FAN_PIN 27
#define INTAKE_FAN_PIN 26

//add as private parameter to object
#define TARGET_TEMP 268                                       //temperature we are trying to reach
#define TEMP_BEGIN_SWITCHING TARGET_TEMP/1.7                  //temp to begin switching between heating element 1 and 2
#define TEMP_DONE_COOLING 90                                  //when the roast is completely done (done cooling)
#define TURN_OFF_SMOKE_SUPPRESSION_ELEMENT TARGET_TEMP*.75    //When smoke suppression will no longer be needed


//flags for heating algorithm
bool coolMode = false;
int switchState = 1;
bool fanHasRun = false;
bool roastDone = false;
float tempFanRun = 200;

bool pinsConfigured = false;

MAX6675 thermocouple(thermo_sck_pin, thermo_cs_pin, thermo_so_pin);

HeatControl::HeatControl() {

	this->checkWifiOk = true;

	pinMode(HEATING_1_PIN, OUTPUT); //heating 1 
	pinMode(HEATING_2_PIN, OUTPUT); //heating 2
	pinMode(BARREL_PIN, OUTPUT);

	pinMode(INTAKE_FAN_PIN, OUTPUT);
	pinMode(OUTAKE_FAN_PIN, OUTPUT);
	coolMode = false;

	//"Power on test"
	digitalWrite(BARREL_PIN, LOW);
}

void HeatControl::updateHeatingElement()
{
	if (!pinsConfigured) {

		Serial.println("Configuring heating elements!");

		pinsConfigured = true;
		// configure pins for temp sensor
		pinMode(thermo_vcc_pin, OUTPUT);
		//pinMode(BARREL_PIN, OUTPUT);
		pinMode(thermo_gnd_pin, OUTPUT);
		digitalWrite(thermo_vcc_pin, HIGH);
		digitalWrite(thermo_gnd_pin, LOW);
		digitalWrite(thermo_cs_pin, HIGH);
		digitalWrite(BARREL_PIN, HIGH);
		digitalWrite(HEATING_2_PIN, HIGH);
		delay(500);
	}
	
	if (roastDone == false) {

		double tempTemp = getTemp();
		delay(500);

		//intake fan will run every 30 degrees
		if (tempFanRun == tempTemp) {
			Serial.println("Turned on intake fan.");
			digitalWrite(INTAKE_FAN_PIN, HIGH);
			delay(4000);
			digitalWrite(INTAKE_FAN_PIN, LOW);

			tempFanRun += 30;
		}

		//turn on intake fan for 3 seconds
		if (tempTemp >= 220 && !fanHasRun) {
			Serial.println("Turning on intake fan for 8 seconds");
			digitalWrite(INTAKE_FAN_PIN, HIGH);
			digitalWrite(OUTAKE_FAN_PIN, HIGH);
			delay(8000);
			digitalWrite(OUTAKE_FAN_PIN, LOW);
			digitalWrite(INTAKE_FAN_PIN, LOW);

			fanHasRun = true;
		}

		if (coolMode == true && tempTemp <= 270 && tempTemp > TEMP_DONE_COOLING) {
			digitalWrite(HEATING_1_PIN, LOW);
			Serial.println("Secondary heating element has been turned off.");
		}

		else if (coolMode == true && tempTemp <= TEMP_DONE_COOLING) {
			//TODO: turn off everything
			digitalWrite(HEATING_2_PIN, LOW);
			digitalWrite(HEATING_1_PIN, LOW);
			digitalWrite(BARREL_PIN, LOW);
			digitalWrite(OUTAKE_FAN_PIN, LOW);
			digitalWrite(INTAKE_FAN_PIN, LOW);
			roastDone = true;
			Serial.println("Turning off all heating elements and barrel!");
		}
		//turn off heating
		else if (tempTemp >= TARGET_TEMP && coolMode == false) {
			setCheckWifiOk(false);
			Serial.println("tempTemp >= TARGET_TEMP && coolMode == false");
			digitalWrite(HEATING_2_PIN, LOW);
			delay(50);
			digitalWrite(HEATING_1_PIN, HIGH);

			coolMode = true;
			digitalWrite(OUTAKE_FAN_PIN, HIGH);  //Turn on outake fan during cooling
			digitalWrite(INTAKE_FAN_PIN, HIGH);
			//half a second between switches
		}
		else if (tempTemp >= TEMP_BEGIN_SWITCHING && tempTemp < TARGET_TEMP  && coolMode == false) {
			Serial.println("tempTemp >= TEMP_BEGIN_SWITCHING && tempTemp < TARGET_TEMP  && coolMode == false");
			if (switchState == 1) {
				digitalWrite(HEATING_2_PIN, LOW);
				delay(50);
				digitalWrite(HEATING_1_PIN, HIGH);
				delay(450);
			}
			else {
				digitalWrite(HEATING_1_PIN, LOW);
				delay(50);
				digitalWrite(HEATING_2_PIN, HIGH);
				delay(550);
			}
			switchState *= -1;

			//digitalWrite(OUTAKE_FAN_PIN, HIGH);

		}
		else {
			Serial.println("Temp is not greater than 212 degrees F");
		}

		//updateViewCheck();

		//Serial.print("Temperature (F): "); Serial.println(getTemp());
	}
	else {
		Serial.println("Roast is done!");
	}

}

double HeatControl::getTemp() {
	double temp;

	Serial.print("Temp (F): ");
	temp = thermocouple.readFahrenheit();
	Serial.println(temp);
	delay(300);

	return temp;
}

void HeatControl::setCheckWifiOk(bool state)
{
	this->checkWifiOk = state;
}

bool HeatControl::getCheckWifiOk()
{
	return this->checkWifiOk;
}
