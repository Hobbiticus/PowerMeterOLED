#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "INA236.h"


INA236 INA(0x40);

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3D ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


#define ARROW_HEIGHT   9
#define ARROW_WIDTH    8
static const unsigned char PROGMEM ArrowLeft[] =
{
  0b00001111,
  0b00011110,
  0b00111100,
  0b01111000,
  0b11110000,
  0b01111000,
  0b00111100,
  0b00011110,
  0b00001111
};
static const unsigned char PROGMEM ArrowRight[] =
{
  0b11110000,
  0b01111000,
  0b00111100,
  0b00111100,
  0b00001111,
  0b00011110,
  0b00111100,
  0b01111000,
  0b11110000
};

void setup()
{
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    while (true)
      delay(1000);
  }

  display.clearDisplay();
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  if (!INA.begin())
  {
    Serial.println("Could not connect to INA236");
    display.print("INA236 Failed");
    display.display();
    while (true)
      delay(100);
  }
  INA.setMaxCurrentShunt(2, 0.01); //TODO: set these values

  display.print("Power Meter");
  display.setCursor(0, 16);
  display.print("by Chris Weiland");
  display.display();
  delay(1000);
}



void loop()
{
  float V = INA.getBusVoltage();
  float I = INA.getCurrent();
  float P = INA.getPower();
  float Vs = INA.getShuntVoltage_mV();

  display.clearDisplay();

  display.setCursor(0, 0);
  display.print(P, 3);
  display.print(" W");

  display.setCursor(0, 8);
  display.print(V, 3);
  display.print(" V");

  display.setCursor(0, 16);
  display.print(I, 3);
  display.print(" A");

  display.setCursor(0, 24);
  display.print(Vs, 2);
  display.print(" mV");

  static unsigned long AnimateTicks = 500;  //how fast to animate - lower is faster
  if (I < 0)
  {
    //animate to the left
  }
  else
  {
    //animate to the right
  }


  display.display();
  delay(100);
}
