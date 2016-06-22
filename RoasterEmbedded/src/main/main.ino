/*
  Main entry file for RoastRight embedded system
*/

#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_STMPE610.h>

// This is calibration data for the raw touch data to the screen coordinates
#define TS_MINX 150
#define TS_MINY 130
#define TS_MAXX 3800
#define TS_MAXY 4000

#define STMPE_CS 8
Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);
#define TFT_CS 10
#define TFT_DC 9
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

//Keep track of temperature and fan rate current and target
int currentTemp = 150;
int targetTemp = 200;
int currentFanRate = 35;
int targetFanRate = 45;

void setup(void)
{
  Serial.begin(9600); //begin serial communication

  tft.begin();  //begin listening for touch
  if (!ts.begin()) {
    Serial.println("Unable to start touchscreen.");
  }
  else {
    Serial.println("Touchscreen started.");
  }

  tft.fillScreen(ILI9341_WHITE);
  // origin = left,top landscape (USB left upper)
  tft.setRotation(1);

  Serial.println("Running liveRoastView()");
  liveRoastView();
  delay(3000);

  //set pin 6 to 62500 / 2 Hz = 31.25 kHz
  setPwmFrequency(6,2);

  Serial.println("Set pin 6's frequency to 31250");
}

void loop() {

  //has the screen been touched
  //if (!ts.bufferEmpty()) {
  if (ts.touched()) {
    // Retrieve a point
    TS_Point p = ts.getPoint();
    // Scale using the calibration #'s
    // and rotate coordinate system
    p.x = map(p.x, TS_MINY, TS_MAXY, 0, tft.height());
    p.y = map(p.y, TS_MINX, TS_MAXX, 0, tft.width());
    int y = tft.height() - p.x;
    int x = p.y;

    //    Serial.println("Pair:");
    //    Serial.println(x);
    //    Serial.println(y);

    //touch on left side?
    if ( x < 160) {
      Serial.println("Left side touched!");

      //down temp pressed?
      if (x > 35 && x < 65 && y > 165 && y < 190) {
        Serial.println("Turn down temperature pressed...");
        updateValuesButton(0);
      }

      //up temp pressed?
      else if (x > 108 && x < 138 && y > 165 && y < 190) {
        Serial.println("Turn up temperature pressed...");
        updateValuesButton(1);
      }
    }

    //touch on the right side?
    else if ( x > 160) {
      Serial.println("Right side touched!");

      //down fan pressed?
      if (x > 165 && x < 195 && y > 165 && y < 190) {
        Serial.println("Turn down fan pressed...");
        updateValuesButton(2);
      }

      //up fan pressed?
      else if (x > 238 && x < 268 && y > 165 && y < 190) {
        Serial.println("Turn up fan pressed...");
        updateValuesButton(3);
      }
    }

  }

  //delay(2000);
}

/**
   Will initialize the view where the user will be
   able to control the roaster live
*/
unsigned long liveRoastView() {
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

/**
   Will update the given field with the current value

   type 0 decrease temp by 5 degrees
   type 1 increase temp by 5 degrees
   type 2 decrease fan rate by 5 percent
   type 3 increase fan rate by 5 percent
*/
void updateValuesButton(unsigned int type) {
  switch (type) {
    case 0:
      //targetTemp -= 5;
      targetTemp >= 105 ? targetTemp -= 5 : targetTemp = 100;
      if(targetTemp > 300) targetTemp = 300;
      //setDutyCycle(targetTemp);
      tft.setCursor(70, 170);
      tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);  tft.setTextSize(2);
      tft.println(targetTemp);
      Serial.println(targetTemp);
      break;
    case 1:
      targetTemp <= 295 ? targetTemp += 5 : targetTemp = 300;
      if(targetTemp > 300) targetTemp = 300;
      //setDutyCycle(targetTemp);
      tft.setCursor(70, 170);
      tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);  tft.setTextSize(2);
      tft.println(targetTemp);
      Serial.println(targetTemp);
      //Note: after lcd updated make http request
      break;
    case 2:
      targetFanRate >= 5 ? targetFanRate -= 5 : targetFanRate = 0;
      if(targetFanRate > 100) targetFanRate = 100;  //maybe set to 0?? weird bug
      tft.setCursor(200, 170);
      tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);  tft.setTextSize(2);
      tft.println(targetFanRate);
      break;
    case 3:
      targetFanRate <= 95 ? targetFanRate += 5 : targetFanRate = 100;
      if(targetFanRate > 100) targetFanRate = 100;
      tft.setCursor(200, 170);
      tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);  tft.setTextSize(2);
      tft.println(targetFanRate);
      break;
  }

  //delay(1000);

}

/**
 * Will take the target temperature and calculate the necessary duty cycle and
 * set it to that value.
 * Reference: https://www.arduino.cc/en/Tutorial/PWM
 * 
 * target temperature that we would like to go to
 */
void setDutyCycle(int target){

  //from 0 to 255, 0% to 100%
  //analogWrite(TEMP_CONTROL_PIN, target/something);
  
}

/**
 * Will take the voltage across the temp sensor and
 * convert that into degrees Farenheit
 */
double getCurrentTemp(){
  //sample temperature

  return 0.0;
}

void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if(pin == 3 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x7; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}

