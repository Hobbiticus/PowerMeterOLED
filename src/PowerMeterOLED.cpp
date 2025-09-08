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
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


#define ARROW_HEIGHT   8
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
  0b00011110
};
static const unsigned char PROGMEM ArrowRight[] =
{
  0b11110000,
  0b01111000,
  0b00111100,
  0b00011110,
  0b00001111,
  0b00011110,
  0b00111100,
  0b01111000
};

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  Serial.println("Hello!");

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    while (true)
      delay(1000);
  }
  Serial.println("Found display");

  display.clearDisplay();
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  // if (!INA.begin())
  // {
  //   Serial.println("Could not connect to INA236");
  //   display.print("INA236 Failed");
  //   display.display();
  //   while (true)
  //     delay(100);
  // }
  // INA.setMaxCurrentShunt(2, 0.01); //TODO: set these values
  // //5 would also work without loss of quality
  // INA.setBusVoltageConversionTime(6);
  // INA.setShuntVoltageConversionTime(6);
  // INA.setAverage(16);

  display.print("PowerMeter");
  display.setCursor(48, 16);
  display.print("by");
  display.setCursor(30, 32);
  display.print("Chris");
  display.setCursor(18, 48);
  display.print("Weiland");
  display.display();
  delay(2000);
  Serial.println("Moving on...");
}

void PrintFloat(float val, int* tens, int* millis)
{
  *tens = (int)val;
  *millis = abs((int)((val - *tens) * 1000));
}

unsigned long LastMillis = 0;
void loop()
{
  float V = 12.7654;//INA.getBusVoltage();
  float I = 2.1234;//INA.getCurrent();
  float P = 27.3456;//INA.getPower();
  float Vs = 45.2345;//INA.getShuntVoltage_mV();

  display.clearDisplay();
  display.setTextSize(2); // Draw 2X-scale text

  display.setCursor(0, 0);
  display.print(P, 3);
  display.print(" W");

  display.setCursor(0, 16);
  display.print(V, 3);
  display.print(" V");

  display.setCursor(0, 32);
  //display.print(I, 3);
  unsigned long thisMillis = millis();
  display.print(thisMillis - LastMillis);
  LastMillis = thisMillis;
  display.print(" A");

  display.setTextSize(1);

  display.setCursor(0, 48);
  display.print(Vs, 2);
  display.print(" mV");

  static unsigned long AnimateTicks = 500;  //how fast to animate - lower is faster
  float dx = (int)(millis() % AnimateTicks);
  dx /= AnimateTicks;
  int offset = (int)(dx * ARROW_WIDTH);
  offset -= ARROW_WIDTH; //start slightly off screen
  const unsigned char* arrow;
  if (I < 0)
  {
    arrow = ArrowLeft;
    offset *= -1;
  }
  else
  {
    arrow = ArrowRight;
  }
  for (int i = 0; i < SCREEN_WIDTH / ARROW_WIDTH + 1; i++)
    display.drawBitmap(i * ARROW_WIDTH + offset, 56, arrow, ARROW_WIDTH, ARROW_HEIGHT, 1);
  display.display();

  //INA.waitConversionReady();
  //delay(1);
}
