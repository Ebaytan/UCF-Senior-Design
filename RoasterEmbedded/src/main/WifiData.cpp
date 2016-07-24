// 
// 
// 

#include "WifiData.h"
#include <ArduinoJson.h>
#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include <string.h>
#include "utility/debug.h"

// Interrupt & Control Pins
#define ADAFRUIT_CC3000_IRQ   3  // MUST be an interrupt pin!
#define ADAFRUIT_CC3000_VBAT  8 // WEN
#define ADAFRUIT_CC3000_CS    10

Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
	SPI_CLOCK_DIVIDER); // you can change this clock speed

						// WiFi SSID & Password
#define WLAN_SSID       "Ebaytan" // cannot be longer than 32 characters!
#define WLAN_PASS       "ebaytan0818"

						// Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define WLAN_SECURITY   WLAN_SEC_WPA2

#define IDLE_TIMEOUT_MS  3000      // Amount of time to wait (in milliseconds) with no data
						// received before closing the connection.  If you know the server
						// you're accessing is quick to respond, you can reduce this value.

const size_t MAX_CONTENT_SIZE = 512; // Max size of the HTTP response

									 // JWT Token for:
									 // Username: Ebaytan
									 // Password: 1234
									 // JWT eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJfaWQiOiI1NzZhZWEyZmFkODI0Y2I1MWU3N2UyZWIiLCJ1c2VybmFtZSI6IkViYXl0YW4iLCJwYXNzd29yZCI6IiQyYSQxMCQzSHFkWHNOZnJpUHZoMUdnMzBQaHQuUUxkdVhYcVdqNi54dm9HUzJyZUFuVmhkellnOW9XaSIsImVtYWlsIjoiaWV2YW5iYXl0YW5AZ21haWwuY29tIiwiX192IjowLCJyb2FzdGluZ3Byb2ZpbGVzIjpbIk5ldyBFbmdsYW5kIiwiTmV3IEVuZ2xhbmQiXSwicm9hc3RlciI6eyJyb2FzdGluZ1N0YXR1cyI6InN0YXJ0LXBlbmRpbmciLCJyb2FzdERhdGEiOiJOZXcgRW5nbGFuZCJ9LCJyYXRpbmciOnsiYXZnUmF0aW5nIjowLCJ0b3RhbEFjY1JhdGluZyI6MCwibnVtYmVyb2ZSYXRpbmdzIjowfX0.vp37wfxjkXHcNRT_lUHAqdD-vEXCSN6LC8wbxCe-cyg

									 ////===============================================================================
									 //// List of URI's for GET/POST requests
									 ////===============================================================================

									 // DNS of Web API
#define WEBSITE                 "ec2-54-174-178-132.compute-1.amazonaws.com"

									 // Unique URI's for GET/POST requests
#define homeURI                 "/"
#define roastListURI            "/api/roast"
#define roastStatusURI          "/api/control"

								// 3 Roast Type URI's
#define lightRoastURI           "/api/roast/New%20England"
#define mediumRoastURI          "/api/roast/American"
#define darkRoastURI            "/api/roast/French%20Roast"

								// 3 Roast Type / START Mode URI's for roaster
#define lightRoastStartURI      "/api/control/roaster?mode=start&roastData=New%20England"
#define mediumRoastStartURI     "/api/control/roaster?mode=start&roastData=American"
#define darkRoastStartURI       "/api/control/roaster?mode=start&roastData=French%20Roast"

								// 3 Roast Type / STOP Mode URI's for roaster
#define lightRoastStopURI      "/api/control/roaster?mode=stop&roastData=New%20England"
#define mediumRoastStopURI     "/api/control/roaster?mode=stop&roastData=American"
#define darkRoastStopURI       "/api/control/roaster?mode=stop&roastData=French%20Roast"

								// 3 Roast Type / PAUSE Mode URI's for roaster
#define lightRoastPauseURI      "/api/control/roaster?mode=pause&roastData=New%20England"
#define mediumRoastPauseURI     "/api/control/roaster?mode=pause&roastData=American"
#define darkRoastPauseURI       "/api/control/roaster?mode=pause&roastData=French%20Roast"

									 ////===============================================================================
									 //// Global Variables
									 ////===============================================================================

uint32_t ip;


WifiData::WifiData()
{
	Serial.begin(115200); //begin serial communication
	Serial.println("Setting up the wifi");

	Serial.println(F("\nInitializing..."));
	if (!cc3000.begin())
	{
		Serial.println(F("Couldn't begin()! Check your wiring?"));
		while (1);
	}

	Serial.print(F("\nAttempting to connect to ")); Serial.println(WLAN_SSID);
	if (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
		Serial.println(F("Failed!"));
		while (1);
	}
	Serial.println(F("Connected!"));

	this->beanType = "";
	this->roastName = "";
	this->roastType = "";
	this->targetTemp = 350;
	this->roasterState = "";

}

String WifiData::getroastName()
{
	return this->roastName;
}

void WifiData::setroastName(String roastName)
{
	this->roastName = roastName;
}

String WifiData::getbeanType()
{
	return this->beanType;
}

void WifiData::setbeanType(String beanType)
{
	this->beanType = beanType;
}

String WifiData::getroastType()
{
	return this->roastType;
}

void WifiData::setroastType(String roastType)
{
	this->roastType = roastType;
}

int WifiData::gettargetTemp()
{
	return this->targetTemp;
}

void WifiData::settargetTemp(int targetTemp)
{
	this->targetTemp = targetTemp;
}

String WifiData::getroasterState()
{
	return this->roasterState;
}

void WifiData::setroasterState(String state)
{
	this->roasterState = state;
}


/*
	HTTP requests needed
*/

void WifiData::roasterStatus()
{
	ip = 0;

	// Try looking up the website's IP address
	Serial.print(WEBSITE); Serial.print(F(" -> "));
	while (ip == 0) {
		if (!cc3000.getHostByName(WEBSITE, &ip)) {
			Serial.println(F("Couldn't resolve!"));
		}
		delay(500);
	}

	// Setup client for CC3000
	Adafruit_CC3000_Client roasterStatusClient = cc3000.connectTCP(ip, 80);
	delay(300);

	char c; // Variable when reading in data from request
	String result = ""; // Empty string that is created for request

	// Declare buffer for JSON data that is read from request
	DynamicJsonBuffer jsonBuffer;

	// GET: Roaster status. Requires JWT in the Authorization Header
	if (roasterStatusClient.connected()) {
		roasterStatusClient.fastrprint(F("GET "));
		roasterStatusClient.fastrprint(roastStatusURI);
		roasterStatusClient.fastrprint(F(" HTTP/1.1\r\n"));
		roasterStatusClient.fastrprint(F("Host: ")); roasterStatusClient.fastrprint(WEBSITE); roasterStatusClient.fastrprint(F("\r\n"));
		roasterStatusClient.fastrprint(F("Authorization: JWT eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJfaWQiOiI1NzZhZWEyZmFkODI0Y2I1MWU3N2UyZWIiLCJ1c2VybmFtZSI6IkViYXl0YW4iLCJwYXNzd29yZCI6IiQyYSQxMCQzSHFkWHNOZnJpUHZoMUdnMzBQaHQuUUxkdVhYcVdqNi54dm9HUzJyZUFuVmhkellnOW9XaSIsImVtYWlsIjoiaWV2YW5iYXl0YW5AZ21haWwuY29tIiwiX192IjowLCJyb2FzdGluZ3Byb2ZpbGVzIjpbIk5ldyBFbmdsYW5kIiwiTmV3IEVuZ2xhbmQiXSwicm9hc3RlciI6eyJyb2FzdGluZ1N0YXR1cyI6InN0YXJ0LXBlbmRpbmciLCJyb2FzdERhdGEiOiJOZXcgRW5nbGFuZCJ9LCJyYXRpbmciOnsiYXZnUmF0aW5nIjowLCJ0b3RhbEFjY1JhdGluZyI6MCwibnVtYmVyb2ZSYXRpbmdzIjowfX0.vp37wfxjkXHcNRT_lUHAqdD-vEXCSN6LC8wbxCe-cyg"));
		roasterStatusClient.fastrprint(F("\r\n"));
		roasterStatusClient.println();
	}
	else {
		Serial.println(F("Connection failed"));
		return;
	}

	// Read data until either the connection is closed, or the idle timeout is reached
	unsigned long lastRead = millis();
	while (roasterStatusClient.connected() && (millis() - lastRead < IDLE_TIMEOUT_MS)) {
		while (roasterStatusClient.available()) {
			c = roasterStatusClient.read();
			result = result + c;
			if (result.endsWith("Connection: keep-alive")) { // Skip Headers of request
				result = "";
			}
			lastRead = millis();
		}
	}

	Serial.println(result); // Print JSON data from request
	delay(1000);

	JsonObject& root = jsonBuffer.parseObject(result);

	// Test if parsing succeeds.
	if (!root.success()) {
		Serial.println("parseObject() failed");
		return;
	}

	// Fetch values from JSON data
	const char* testSuccess = root["success"];
	//String roastData = root["roast"][0];
	//String roastName = root["roast"][0]["name"];
	String roastingStatus = root["roaster"]["roastingStatus"];
	String roastType = root["roastData"]["roastType"];
	String roastName = root["roastData"]["name"];
	String beanType = root["roastData"]["beanType"];
	//int targetTemp = root["roastData"]["RoastingData"];

	// Print values
	Serial.println(testSuccess);    // success: true or false
	Serial.println(roastingStatus); // start/pause/stop - pending
	Serial.println(roastType);      // Light/Medium/Dark

	if (String(roastingStatus) == "start-pending") { // If START command is received from iOS application

		Serial.println("\n START Command Received");

		// Set values for parameters in object
		setroastName(roastName);
		setbeanType(beanType);
		setroastType(roastType);
		setroasterState("start-pending");
		//settargetTemp(targetTemp);

		if (String(roastType) == "Light") {
			Serial.println("Light roast starting!");
			// POST: Roaster confirms request and fulfils pending roasting status
			roasterStatusClient.fastrprint(F("POST "));
			roasterStatusClient.fastrprint(lightRoastStartURI);
			roasterStatusClient.fastrprint(F(" HTTP/1.1\r\n"));
			roasterStatusClient.fastrprint(F("Host: ")); roasterStatusClient.fastrprint(WEBSITE); roasterStatusClient.fastrprint(F("\r\n"));
			roasterStatusClient.fastrprint(F("Authorization: JWT eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJfaWQiOiI1NzZhZWEyZmFkODI0Y2I1MWU3N2UyZWIiLCJ1c2VybmFtZSI6IkViYXl0YW4iLCJwYXNzd29yZCI6IiQyYSQxMCQzSHFkWHNOZnJpUHZoMUdnMzBQaHQuUUxkdVhYcVdqNi54dm9HUzJyZUFuVmhkellnOW9XaSIsImVtYWlsIjoiaWV2YW5iYXl0YW5AZ21haWwuY29tIiwiX192IjowLCJyb2FzdGluZ3Byb2ZpbGVzIjpbIk5ldyBFbmdsYW5kIiwiTmV3IEVuZ2xhbmQiXSwicm9hc3RlciI6eyJyb2FzdGluZ1N0YXR1cyI6InN0YXJ0LXBlbmRpbmciLCJyb2FzdERhdGEiOiJOZXcgRW5nbGFuZCJ9LCJyYXRpbmciOnsiYXZnUmF0aW5nIjowLCJ0b3RhbEFjY1JhdGluZyI6MCwibnVtYmVyb2ZSYXRpbmdzIjowfX0.vp37wfxjkXHcNRT_lUHAqdD-vEXCSN6LC8wbxCe-cyg"));
			roasterStatusClient.fastrprint(F("\r\n"));
			roasterStatusClient.println();

		}
		else if (String(roastType) == "Medium") {
			Serial.println("Medium roast starting!");
			// POST: Roaster confirms request and fulfils pending roasting status
			roasterStatusClient.fastrprint(F("POST "));
			roasterStatusClient.fastrprint(mediumRoastStartURI);
			roasterStatusClient.fastrprint(F(" HTTP/1.1\r\n"));
			roasterStatusClient.fastrprint(F("Host: ")); roasterStatusClient.fastrprint(WEBSITE); roasterStatusClient.fastrprint(F("\r\n"));
			roasterStatusClient.fastrprint(F("Authorization: JWT eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJfaWQiOiI1NzZhZWEyZmFkODI0Y2I1MWU3N2UyZWIiLCJ1c2VybmFtZSI6IkViYXl0YW4iLCJwYXNzd29yZCI6IiQyYSQxMCQzSHFkWHNOZnJpUHZoMUdnMzBQaHQuUUxkdVhYcVdqNi54dm9HUzJyZUFuVmhkellnOW9XaSIsImVtYWlsIjoiaWV2YW5iYXl0YW5AZ21haWwuY29tIiwiX192IjowLCJyb2FzdGluZ3Byb2ZpbGVzIjpbIk5ldyBFbmdsYW5kIiwiTmV3IEVuZ2xhbmQiXSwicm9hc3RlciI6eyJyb2FzdGluZ1N0YXR1cyI6InN0YXJ0LXBlbmRpbmciLCJyb2FzdERhdGEiOiJOZXcgRW5nbGFuZCJ9LCJyYXRpbmciOnsiYXZnUmF0aW5nIjowLCJ0b3RhbEFjY1JhdGluZyI6MCwibnVtYmVyb2ZSYXRpbmdzIjowfX0.vp37wfxjkXHcNRT_lUHAqdD-vEXCSN6LC8wbxCe-cyg"));
			roasterStatusClient.fastrprint(F("\r\n"));
			roasterStatusClient.println();

		}
		else if (String(roastType) == "Dark") {
			Serial.println("Dark roast starting!");
			// POST: Roaster confirms request and fulfils pending roasting status
			roasterStatusClient.fastrprint(F("POST "));
			roasterStatusClient.fastrprint(darkRoastStartURI);
			roasterStatusClient.fastrprint(F(" HTTP/1.1\r\n"));
			roasterStatusClient.fastrprint(F("Host: ")); roasterStatusClient.fastrprint(WEBSITE); roasterStatusClient.fastrprint(F("\r\n"));
			roasterStatusClient.fastrprint(F("Authorization: JWT eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJfaWQiOiI1NzZhZWEyZmFkODI0Y2I1MWU3N2UyZWIiLCJ1c2VybmFtZSI6IkViYXl0YW4iLCJwYXNzd29yZCI6IiQyYSQxMCQzSHFkWHNOZnJpUHZoMUdnMzBQaHQuUUxkdVhYcVdqNi54dm9HUzJyZUFuVmhkellnOW9XaSIsImVtYWlsIjoiaWV2YW5iYXl0YW5AZ21haWwuY29tIiwiX192IjowLCJyb2FzdGluZ3Byb2ZpbGVzIjpbIk5ldyBFbmdsYW5kIiwiTmV3IEVuZ2xhbmQiXSwicm9hc3RlciI6eyJyb2FzdGluZ1N0YXR1cyI6InN0YXJ0LXBlbmRpbmciLCJyb2FzdERhdGEiOiJOZXcgRW5nbGFuZCJ9LCJyYXRpbmciOnsiYXZnUmF0aW5nIjowLCJ0b3RhbEFjY1JhdGluZyI6MCwibnVtYmVyb2ZSYXRpbmdzIjowfX0.vp37wfxjkXHcNRT_lUHAqdD-vEXCSN6LC8wbxCe-cyg"));
			roasterStatusClient.fastrprint(F("\r\n"));
			roasterStatusClient.println();
		}

	}
	else if (String(roastingStatus) == "pause-pending") { // If PAUSE command is received from iOS application

		Serial.println("\n PAUSE Command Received");

		setroasterState("pause");

		// Set values for parameters in object
		setroastName(roastName);
		setbeanType(beanType);
		setroastType(roastType);
		//settargetTemp(targetTemp);

		if (String(roastType) == "Light") {
			// POST: Roaster confirms request and fulfils pending roasting status
			roasterStatusClient.fastrprint(F("POST "));
			roasterStatusClient.fastrprint(lightRoastPauseURI);
			roasterStatusClient.fastrprint(F(" HTTP/1.1\r\n"));
			roasterStatusClient.fastrprint(F("Host: ")); roasterStatusClient.fastrprint(WEBSITE); roasterStatusClient.fastrprint(F("\r\n"));
			roasterStatusClient.fastrprint(F("Authorization: JWT eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJfaWQiOiI1NzZhZWEyZmFkODI0Y2I1MWU3N2UyZWIiLCJ1c2VybmFtZSI6IkViYXl0YW4iLCJwYXNzd29yZCI6IiQyYSQxMCQzSHFkWHNOZnJpUHZoMUdnMzBQaHQuUUxkdVhYcVdqNi54dm9HUzJyZUFuVmhkellnOW9XaSIsImVtYWlsIjoiaWV2YW5iYXl0YW5AZ21haWwuY29tIiwiX192IjowLCJyb2FzdGluZ3Byb2ZpbGVzIjpbIk5ldyBFbmdsYW5kIiwiTmV3IEVuZ2xhbmQiXSwicm9hc3RlciI6eyJyb2FzdGluZ1N0YXR1cyI6InN0YXJ0LXBlbmRpbmciLCJyb2FzdERhdGEiOiJOZXcgRW5nbGFuZCJ9LCJyYXRpbmciOnsiYXZnUmF0aW5nIjowLCJ0b3RhbEFjY1JhdGluZyI6MCwibnVtYmVyb2ZSYXRpbmdzIjowfX0.vp37wfxjkXHcNRT_lUHAqdD-vEXCSN6LC8wbxCe-cyg"));
			roasterStatusClient.fastrprint(F("\r\n"));
			roasterStatusClient.println();

		}
		else if (String(roastType) == "Medium") {
			// POST: Roaster confirms request and fulfils pending roasting status
			roasterStatusClient.fastrprint(F("POST "));
			roasterStatusClient.fastrprint(mediumRoastPauseURI);
			roasterStatusClient.fastrprint(F(" HTTP/1.1\r\n"));
			roasterStatusClient.fastrprint(F("Host: ")); roasterStatusClient.fastrprint(WEBSITE); roasterStatusClient.fastrprint(F("\r\n"));
			roasterStatusClient.fastrprint(F("Authorization: JWT eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJfaWQiOiI1NzZhZWEyZmFkODI0Y2I1MWU3N2UyZWIiLCJ1c2VybmFtZSI6IkViYXl0YW4iLCJwYXNzd29yZCI6IiQyYSQxMCQzSHFkWHNOZnJpUHZoMUdnMzBQaHQuUUxkdVhYcVdqNi54dm9HUzJyZUFuVmhkellnOW9XaSIsImVtYWlsIjoiaWV2YW5iYXl0YW5AZ21haWwuY29tIiwiX192IjowLCJyb2FzdGluZ3Byb2ZpbGVzIjpbIk5ldyBFbmdsYW5kIiwiTmV3IEVuZ2xhbmQiXSwicm9hc3RlciI6eyJyb2FzdGluZ1N0YXR1cyI6InN0YXJ0LXBlbmRpbmciLCJyb2FzdERhdGEiOiJOZXcgRW5nbGFuZCJ9LCJyYXRpbmciOnsiYXZnUmF0aW5nIjowLCJ0b3RhbEFjY1JhdGluZyI6MCwibnVtYmVyb2ZSYXRpbmdzIjowfX0.vp37wfxjkXHcNRT_lUHAqdD-vEXCSN6LC8wbxCe-cyg"));
			roasterStatusClient.fastrprint(F("\r\n"));
			roasterStatusClient.println();

		}
		else if (String(roastType) == "Dark") {
			// POST: Roaster confirms request and fulfils pending roasting status
			roasterStatusClient.fastrprint(F("POST "));
			roasterStatusClient.fastrprint(darkRoastPauseURI);
			roasterStatusClient.fastrprint(F(" HTTP/1.1\r\n"));
			roasterStatusClient.fastrprint(F("Host: ")); roasterStatusClient.fastrprint(WEBSITE); roasterStatusClient.fastrprint(F("\r\n"));
			roasterStatusClient.fastrprint(F("Authorization: JWT eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJfaWQiOiI1NzZhZWEyZmFkODI0Y2I1MWU3N2UyZWIiLCJ1c2VybmFtZSI6IkViYXl0YW4iLCJwYXNzd29yZCI6IiQyYSQxMCQzSHFkWHNOZnJpUHZoMUdnMzBQaHQuUUxkdVhYcVdqNi54dm9HUzJyZUFuVmhkellnOW9XaSIsImVtYWlsIjoiaWV2YW5iYXl0YW5AZ21haWwuY29tIiwiX192IjowLCJyb2FzdGluZ3Byb2ZpbGVzIjpbIk5ldyBFbmdsYW5kIiwiTmV3IEVuZ2xhbmQiXSwicm9hc3RlciI6eyJyb2FzdGluZ1N0YXR1cyI6InN0YXJ0LXBlbmRpbmciLCJyb2FzdERhdGEiOiJOZXcgRW5nbGFuZCJ9LCJyYXRpbmciOnsiYXZnUmF0aW5nIjowLCJ0b3RhbEFjY1JhdGluZyI6MCwibnVtYmVyb2ZSYXRpbmdzIjowfX0.vp37wfxjkXHcNRT_lUHAqdD-vEXCSN6LC8wbxCe-cyg"));
			roasterStatusClient.fastrprint(F("\r\n"));
			roasterStatusClient.println();
		}

	}
	else if (String(roastingStatus) == "stop-pending") { // If STOP command is received from iOS application

		Serial.println("\n STOP Command Received");

		setroasterState("stop");

		// Set values for parameters in object
		setroastName(roastName);
		setbeanType(beanType);
		setroastType(roastType);
		//settargetTemp(targetTemp);

		if (String(roastType) == "Light") {
			// POST: Roaster confirms request and fulfils pending roasting status
			roasterStatusClient.fastrprint(F("POST "));
			roasterStatusClient.fastrprint(lightRoastStopURI);
			roasterStatusClient.fastrprint(F(" HTTP/1.1\r\n"));
			roasterStatusClient.fastrprint(F("Host: ")); roasterStatusClient.fastrprint(WEBSITE); roasterStatusClient.fastrprint(F("\r\n"));
			roasterStatusClient.fastrprint(F("Authorization: JWT eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJfaWQiOiI1NzZhZWEyZmFkODI0Y2I1MWU3N2UyZWIiLCJ1c2VybmFtZSI6IkViYXl0YW4iLCJwYXNzd29yZCI6IiQyYSQxMCQzSHFkWHNOZnJpUHZoMUdnMzBQaHQuUUxkdVhYcVdqNi54dm9HUzJyZUFuVmhkellnOW9XaSIsImVtYWlsIjoiaWV2YW5iYXl0YW5AZ21haWwuY29tIiwiX192IjowLCJyb2FzdGluZ3Byb2ZpbGVzIjpbIk5ldyBFbmdsYW5kIiwiTmV3IEVuZ2xhbmQiXSwicm9hc3RlciI6eyJyb2FzdGluZ1N0YXR1cyI6InN0YXJ0LXBlbmRpbmciLCJyb2FzdERhdGEiOiJOZXcgRW5nbGFuZCJ9LCJyYXRpbmciOnsiYXZnUmF0aW5nIjowLCJ0b3RhbEFjY1JhdGluZyI6MCwibnVtYmVyb2ZSYXRpbmdzIjowfX0.vp37wfxjkXHcNRT_lUHAqdD-vEXCSN6LC8wbxCe-cyg"));
			roasterStatusClient.fastrprint(F("\r\n"));
			roasterStatusClient.println();

		}
		else if (String(roastType) == "Medium") {
			// POST: Roaster confirms request and fulfils pending roasting status
			roasterStatusClient.fastrprint(F("POST "));
			roasterStatusClient.fastrprint(mediumRoastStopURI);
			roasterStatusClient.fastrprint(F(" HTTP/1.1\r\n"));
			roasterStatusClient.fastrprint(F("Host: ")); roasterStatusClient.fastrprint(WEBSITE); roasterStatusClient.fastrprint(F("\r\n"));
			roasterStatusClient.fastrprint(F("Authorization: JWT eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJfaWQiOiI1NzZhZWEyZmFkODI0Y2I1MWU3N2UyZWIiLCJ1c2VybmFtZSI6IkViYXl0YW4iLCJwYXNzd29yZCI6IiQyYSQxMCQzSHFkWHNOZnJpUHZoMUdnMzBQaHQuUUxkdVhYcVdqNi54dm9HUzJyZUFuVmhkellnOW9XaSIsImVtYWlsIjoiaWV2YW5iYXl0YW5AZ21haWwuY29tIiwiX192IjowLCJyb2FzdGluZ3Byb2ZpbGVzIjpbIk5ldyBFbmdsYW5kIiwiTmV3IEVuZ2xhbmQiXSwicm9hc3RlciI6eyJyb2FzdGluZ1N0YXR1cyI6InN0YXJ0LXBlbmRpbmciLCJyb2FzdERhdGEiOiJOZXcgRW5nbGFuZCJ9LCJyYXRpbmciOnsiYXZnUmF0aW5nIjowLCJ0b3RhbEFjY1JhdGluZyI6MCwibnVtYmVyb2ZSYXRpbmdzIjowfX0.vp37wfxjkXHcNRT_lUHAqdD-vEXCSN6LC8wbxCe-cyg"));
			roasterStatusClient.fastrprint(F("\r\n"));
			roasterStatusClient.println();

		}
		else if (String(roastType) == "Dark") {
			// POST: Roaster confirms request and fulfils pending roasting status
			roasterStatusClient.fastrprint(F("POST "));
			roasterStatusClient.fastrprint(darkRoastStopURI);
			roasterStatusClient.fastrprint(F(" HTTP/1.1\r\n"));
			roasterStatusClient.fastrprint(F("Host: ")); roasterStatusClient.fastrprint(WEBSITE); roasterStatusClient.fastrprint(F("\r\n"));
			roasterStatusClient.fastrprint(F("Authorization: JWT eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJfaWQiOiI1NzZhZWEyZmFkODI0Y2I1MWU3N2UyZWIiLCJ1c2VybmFtZSI6IkViYXl0YW4iLCJwYXNzd29yZCI6IiQyYSQxMCQzSHFkWHNOZnJpUHZoMUdnMzBQaHQuUUxkdVhYcVdqNi54dm9HUzJyZUFuVmhkellnOW9XaSIsImVtYWlsIjoiaWV2YW5iYXl0YW5AZ21haWwuY29tIiwiX192IjowLCJyb2FzdGluZ3Byb2ZpbGVzIjpbIk5ldyBFbmdsYW5kIiwiTmV3IEVuZ2xhbmQiXSwicm9hc3RlciI6eyJyb2FzdGluZ1N0YXR1cyI6InN0YXJ0LXBlbmRpbmciLCJyb2FzdERhdGEiOiJOZXcgRW5nbGFuZCJ9LCJyYXRpbmciOnsiYXZnUmF0aW5nIjowLCJ0b3RhbEFjY1JhdGluZyI6MCwibnVtYmVyb2ZSYXRpbmdzIjowfX0.vp37wfxjkXHcNRT_lUHAqdD-vEXCSN6LC8wbxCe-cyg"));
			roasterStatusClient.fastrprint(F("\r\n"));
			roasterStatusClient.println();
		}

	}
	else {
		Serial.println("No new command received");
	}

	roasterStatusClient.stop();
	delay(100);
	roasterStatusClient.close();
	delay(200);

	Serial.println("\n================================================================");

	//if we have a new command we must update api, in order for the api to know that the command has been acknowledged
	if (String(roastingStatus) == "start-pending" || String(roastingStatus) == "pause-pending" || String(roastingStatus) == "stop-pending") {
		updateStatus(roastType);  //update api with latest roaster status
	}
		
}

void WifiData::updateStatus(String type)
{

	Serial.println("Updating API with latest roaster state.");

	ip = 0;
	// Try looking up the website's IP address
	Serial.print(WEBSITE); Serial.print(F(" -> "));
	while (ip == 0) {
		if (!cc3000.getHostByName(WEBSITE, &ip)) {
			Serial.println(F("Couldn't resolve!"));
		}
		delay(500);
	}

	// Setup client for CC3000
	Adafruit_CC3000_Client roasterStatusClient = cc3000.connectTCP(ip, 80);
	delay(300);

	char c; // Variable when reading in data from request
	String result = ""; // Empty string that is created for request

						// Declare buffer for JSON data that is read from request
						//StaticJsonBuffer<400> jsonBuffer;

	if (roastType == "Light") {
		// POST: Roaster status. Requires JWT in the Authorization Header
		Serial.println("Light Roast Request");
		if (roasterStatusClient.connected()) {
			roasterStatusClient.fastrprint(F("POST "));
			roasterStatusClient.fastrprint(lightRoastStartURI);
			roasterStatusClient.fastrprint(F(" HTTP/1.1\r\n"));
			roasterStatusClient.fastrprint(F("Host: ")); roasterStatusClient.fastrprint(WEBSITE); roasterStatusClient.fastrprint(F("\r\n"));
			roasterStatusClient.fastrprint(F("Authorization: JWT eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJfaWQiOiI1NzhlYTQ2NDI3NGZlMDA4MjUyYTY3YmIiLCJ1c2VybmFtZSI6IkViYXl0YW4iLCJwYXNzd29yZCI6IiQyYSQxMCRNckZEV2dtZHFCU0Q5UG8uWXZHNzYuVFBZS0h4Y3pienFmRnRqRTFtZ01DYzRjZUYySnFFaSIsImVtYWlsIjoiaWV2YW5iYXl0YW5AZ21haWwuY29tIiwiX192IjowLCJyb2FzdGluZ3Byb2ZpbGVzIjpbXSwicm9hc3RlciI6eyJyb2FzdGVyVUlEIjoiMTIzNCJ9LCJyYXRpbmciOnsiYXZnUmF0aW5nIjowLCJ0b3RhbEFjY1JhdGluZyI6MCwibnVtYmVyb2ZSYXRpbmdzIjowfX0.XHlai4Bn4eqNTIzeBtT24iGRe3xph92g6YBS4GMVGSw"));
			roasterStatusClient.fastrprint(F("\r\n"));
			roasterStatusClient.println();
		}
		else {
			Serial.println(F("Connection failed"));
			return;
		}
	}
	else if (roastType == "Medium") {

		// POST: Roaster status. Requires JWT in the Authorization Header
		Serial.println("Medium Roast Request");
		if (roasterStatusClient.connected()) {
			roasterStatusClient.fastrprint(F("POST "));
			roasterStatusClient.fastrprint(mediumRoastStartURI);
			roasterStatusClient.fastrprint(F(" HTTP/1.1\r\n"));
			roasterStatusClient.fastrprint(F("Host: ")); roasterStatusClient.fastrprint(WEBSITE); roasterStatusClient.fastrprint(F("\r\n"));
			roasterStatusClient.fastrprint(F("Authorization: JWT eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJfaWQiOiI1NzhlYTQ2NDI3NGZlMDA4MjUyYTY3YmIiLCJ1c2VybmFtZSI6IkViYXl0YW4iLCJwYXNzd29yZCI6IiQyYSQxMCRNckZEV2dtZHFCU0Q5UG8uWXZHNzYuVFBZS0h4Y3pienFmRnRqRTFtZ01DYzRjZUYySnFFaSIsImVtYWlsIjoiaWV2YW5iYXl0YW5AZ21haWwuY29tIiwiX192IjowLCJyb2FzdGluZ3Byb2ZpbGVzIjpbXSwicm9hc3RlciI6eyJyb2FzdGVyVUlEIjoiMTIzNCJ9LCJyYXRpbmciOnsiYXZnUmF0aW5nIjowLCJ0b3RhbEFjY1JhdGluZyI6MCwibnVtYmVyb2ZSYXRpbmdzIjowfX0.XHlai4Bn4eqNTIzeBtT24iGRe3xph92g6YBS4GMVGSw"));
			roasterStatusClient.fastrprint(F("\r\n"));
			roasterStatusClient.println();
		}
		else {
			Serial.println(F("Connection failed"));
			return;
		}

	}
	else if (roastType == "Dark") {

		// POST: Roaster status. Requires JWT in the Authorization Header
		Serial.println("Dark Roast Request");
		if (roasterStatusClient.connected()) {
			roasterStatusClient.fastrprint(F("POST "));
			roasterStatusClient.fastrprint(darkRoastStartURI);
			roasterStatusClient.fastrprint(F(" HTTP/1.1\r\n"));
			roasterStatusClient.fastrprint(F("Host: ")); roasterStatusClient.fastrprint(WEBSITE); roasterStatusClient.fastrprint(F("\r\n"));
			roasterStatusClient.fastrprint(F("Authorization: JWT eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJfaWQiOiI1NzhlYTQ2NDI3NGZlMDA4MjUyYTY3YmIiLCJ1c2VybmFtZSI6IkViYXl0YW4iLCJwYXNzd29yZCI6IiQyYSQxMCRNckZEV2dtZHFCU0Q5UG8uWXZHNzYuVFBZS0h4Y3pienFmRnRqRTFtZ01DYzRjZUYySnFFaSIsImVtYWlsIjoiaWV2YW5iYXl0YW5AZ21haWwuY29tIiwiX192IjowLCJyb2FzdGluZ3Byb2ZpbGVzIjpbXSwicm9hc3RlciI6eyJyb2FzdGVyVUlEIjoiMTIzNCJ9LCJyYXRpbmciOnsiYXZnUmF0aW5nIjowLCJ0b3RhbEFjY1JhdGluZyI6MCwibnVtYmVyb2ZSYXRpbmdzIjowfX0.XHlai4Bn4eqNTIzeBtT24iGRe3xph92g6YBS4GMVGSw"));
			roasterStatusClient.fastrprint(F("\r\n"));
			roasterStatusClient.println();
		}
		else {
			Serial.println(F("Connection failed"));
			return;
		}
	}
	// Read data until either the connection is closed, or the idle timeout is reached
	unsigned long lastRead = millis();
	while (roasterStatusClient.connected() && (millis() - lastRead < IDLE_TIMEOUT_MS)) {
		while (roasterStatusClient.available()) {
			c = roasterStatusClient.read();
			result = result + c;
			if (result.endsWith("Connection: keep-alive")) { // Skip Headers of request
				result = "";
			}
			lastRead = millis();
		}
	}

	Serial.print(result); // Print JSON data from request
	delay(1000);

	roasterStatusClient.stop();
	delay(100);
	roasterStatusClient.close();
	delay(200);

	//roasterStatusClient.close(); // Close client
}
