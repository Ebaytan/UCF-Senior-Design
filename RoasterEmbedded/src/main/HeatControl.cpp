// 
// 
// 

#include "HeatControl.h"
#include <Adafruit_TFTLCD.h>
#include <Adafruit_STMPE610.h>
#include <Adafruit_ILI9341.h>

#include <max6675.h>

#define thermoDO  4
#define thermoCS  5
#define thermoCLK 6

#define TARGET_TEMP 250
#define TEMP_CONTROL_PIN 3

//heating element pins
#define HEATING_1_PIN 37
#define HEATING_2_PIN 44

// ThermoCouple
#define thermo_gnd_pin 45
#define thermo_vcc_pin 47
#define thermo_so_pin 49
#define thermo_cs_pin 23
#define thermo_sck_pin 53

//Fan pins
#define OUTAKE_FAN_PIN
#define INTAKE_FAN_PIN

int switchState = 1;


MAX6675 thermocouple(thermo_sck_pin, thermo_cs_pin, thermo_so_pin);

#define TFT_DC 46
#define TFT_CS 5
#define TFT_MOSI 9
#define TFT_CLK 6
#define TFT_RST 8
#define TFT_MISO 7


// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
//Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
// If using the breakout, change pins as desired
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

HeatControl::HeatControl() {
	// configure pins for temp sensor
	pinMode(thermo_vcc_pin, OUTPUT);
	pinMode(thermo_gnd_pin, OUTPUT);
	digitalWrite(thermo_vcc_pin, HIGH);
	digitalWrite(thermo_gnd_pin, LOW);
	digitalWrite(thermo_cs_pin, HIGH);
	pinMode(HEATING_1_PIN, OUTPUT); //heating 1 
	pinMode(HEATING_2_PIN, OUTPUT); //heating 2

	coolMode = false;
}

void HeatControl::updateHeatingElement()
{
	double tempTemp = getTemp();

	if (coolMode == true && tempTemp <= 250 && tempTemp > 90) {
		digitalWrite(HEATING_1_PIN, LOW);
		Serial.println("Secondary heating element has been turned off.");

	}
	else if (coolMode == true && tempTemp <= 90) {
		//TODO: turn off everything
		Serial.println("Roast is done!");
	}
	//turn off heating
	else if (tempTemp >= 360 && coolMode == false) {
		Serial.println("Temp is greater than 212 degrees F!");
		digitalWrite(HEATING_2_PIN, LOW);
		delay(50);
		digitalWrite(HEATING_1_PIN, HIGH);

		coolMode = true;
		//half a second between switches
	}
	else if (tempTemp >= 212 && tempTemp < 360 && coolMode == false) {
		if (switchState == 1) {
			digitalWrite(HEATING_2_PIN, LOW);
			delay(50);
			digitalWrite(HEATING_1_PIN, HIGH);
			delay(400);

		}
		else {
			digitalWrite(HEATING_1_PIN, LOW);
			delay(50);
			digitalWrite(HEATING_2_PIN, HIGH);
			delay(600);
		}
		switchState *= -1;
	}
	else {
		Serial.println("Temp is not greater than 212 degrees F");
	}


	updateTemp();
}

void HeatControl::updateTemp()
{
	//current temperature value
	tft.setCursor(200, 110);
	tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);  tft.setTextSize(2);
	tft.println(getTemp());
}

void HeatControl::updateViewCheck(ViewState state)
{
	//We are currenlty roasting beans
	if (state == ViewState::Predefined) {
		updateTemp();
	}
	//idle view, first view seen when you start up roaster
	if (state == ViewState::Home) {

	}
}

double getTemp() {
	double temp;

	Serial.print("Temp (F): ");
	temp = thermocouple.readFahrenheit();
	Serial.println(temp);

	return temp;
}
