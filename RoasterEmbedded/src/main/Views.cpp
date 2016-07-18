// 
// 
// 

#include "Views.h"
#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_STMPE610.h>
#include <max6675.h>

// This is calibration data for the raw touch data to the screen coordinates
#define TS_MINX 150
#define TS_MINY 130
#define TS_MAXX 3800
#define TS_MAXY 4000

//goes here
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

//Keep track of temperature and fan rate current and target
int currentTemp = 150;
int targetTemp = 200;
int currentFanRate = 35;
int targetFanRate = 45;

ViewState currentState;

View::View()
{
	currentState = ViewState::Home;
}

void View::setView(ViewState setTo) {
	currentState = setTo;
	View hello = View();

	switch (setTo) {
	case (ViewState::Live) :
		currentState = ViewState::Live;
		hello.liveRoastView();
		break;
	case (ViewState::Predefined) :
		currentState = ViewState::Predefined;
		//hello.predefinedRoastView();
		break;
	case (ViewState::Home) :
		currentState = ViewState::Home;
		hello.homeRoastView();
		break;
		
	}
}

void View::initView()
{
	tft.begin();  //begin listening for touch

	tft.fillScreen(ILI9341_WHITE);
	// origin = left,top landscape (USB left upper)
	tft.setRotation(3);

	View::setView(ViewState::Predefined);

	delay(1000);  //my not need this delay
	
}

void View::liveRoastView()
{
	//Title
	tft.setCursor(60, 10);
	tft.setTextColor(ILI9341_BLACK);  tft.setTextSize(2);
	tft.println("Live Roast Control");

	//Back buttons
	tft.fillTriangle(10, 20, 25, 10, 25, 30, ILI9341_BLACK);

	//TODO: add vertical text 'NOW'

	//Current Temp
	tft.setCursor(70, 80);
	tft.setTextColor(ILI9341_BLACK);  tft.setTextSize(2);
	tft.println("Temp");

	tft.setCursor(70, 100);
	tft.setTextColor(ILI9341_BLACK);  tft.setTextSize(2);
	tft.println(currentTemp);

	//Current Fan Rate
	tft.setCursor(200, 80);
	tft.setTextColor(ILI9341_BLACK);  tft.setTextSize(2);
	tft.println("Fan");

	tft.setCursor(200, 100);
	tft.setTextColor(ILI9341_BLACK);  tft.setTextSize(2);
	tft.println(currentFanRate);

	//TODO: add vertical text 'Target'

	//target temperature
	//temp field
	tft.setCursor(70, 170);
	tft.setTextColor(ILI9341_BLACK);  tft.setTextSize(2);
	tft.println(targetTemp);

	//temperature down button
	tft.fillTriangle(40, 170, 60, 170, 50, 185, ILI9341_BLACK);

	//temperature up button
	tft.fillTriangle(113, 185, 133, 185, 123, 170, ILI9341_BLACK);

	//target fan rate
	//fan field
	tft.setCursor(200, 170);
	tft.setTextColor(ILI9341_BLACK);  tft.setTextSize(2);
	tft.println(targetFanRate);

	//fan down button
	tft.fillTriangle(170, 170, 190, 170, 180, 185, ILI9341_BLACK);

	//fan up button
	tft.fillTriangle(243, 185, 263, 185, 253, 170, ILI9341_BLACK);

}

void View::homeRoastView() {
	tft.setRotation(3);
	tft.fillScreen(ILI9341_BLACK);

	tft.setCursor(60, 60);
	tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(3);
	tft.println("Right Roast");

	tft.setCursor(40, 100);
	tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
	tft.println("Waiting on command...");

	yield();
}

void View::preDefinedRoastView() {
		tft.fillScreen(ILI9341_WHITE);

		tft.setCursor(10, 10);
		tft.setTextColor(ILI9341_BLACK);  tft.setTextSize(2);
		tft.println("Name: French Roast");


		tft.setCursor(10, 30);
		tft.setTextColor(ILI9341_BLACK);  tft.setTextSize(2);
		tft.println("Bean Type: Peruvian");

		tft.setCursor(10, 50);
		tft.setTextColor(ILI9341_BLACK);  tft.setTextSize(2);
		tft.println("Roast Type: Dark");

		//target temp label
		tft.setCursor(20, 90);
		tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);  tft.setTextSize(2);
		tft.println("Target Temp:");

		//temperature that we are attempting to reach
		tft.setCursor(200, 90);
		tft.setTextColor(ILI9341_BLACK);  tft.setTextSize(2);
		tft.println(105.5);

		//current temp label
		tft.setCursor(20, 110);
		tft.setTextColor(ILI9341_BLACK);  tft.setTextSize(2);
		tft.println("Current Temp:");
}


