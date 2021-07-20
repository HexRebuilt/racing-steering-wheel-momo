#include <Arduino.h>
#include <string.h>
#include <stdlib.h>
#include <SPI.h>
#include <LedControl.h>
#include <PinChangeInterrupt.h>
#include <RotaryEncoderAdvanced.h>
#include <TinyGPS++.h>

#include "defines.h"
//#include "ignore_undesired_press.h"
#include "volume.h"
#include "apps/HumanInterface/human_interface.h"
#include "apps/HumanInterface/Lcd8Digit.h"
#include "apps/HumanInterface/LedBar.h"
#include "digital_potentiometer.h"
#include "encoder.h"

unsigned short buttons[RADIO_BUTTONS] = {ACTIVE_BUTTON, SKIP_BUTTON, BACK_BUTTON, CALL_ON_BUTTON, CALL_OFF_BUTTON, VOICE_CMD_BUTTON, PAUSE_BUTTON};
unsigned short buttonState[RADIO_BUTTONS] = {0};
unsigned short radioOutputStep = 0;
int rpm = 0;

//RotaryEncoderAdvanced<float> volumeEncoder (VOL_CLK,VOL_DATA,PAUSE_BUTTON,VOL_SENSITIVITY,-256,+256);
//RotaryEncoderAdvanced<float> brightnessEncoder(BRIGHTNESS_CLK,BRIGNTNESS_DATA,LCD_MODE_BUTTON,BRIGHTNESS_SENSITIVITY,MIN_BRIGHT_LCD,MAX_BRIGHT_LCD);

Encoder_KY040 volumewheel, ledwheel;

VolumeController volumecontroller;

Lcd8Digit lcd8Digit;
LedBar ledBar;

TinyGPSPlus gps;

void interruptVolume()
{
  rpm = volumewheel.Steps();
}
void interruptPause()
{
  //volumeEncoder.readPushButton();
  Serial.println("Pause");
}
void interruptBrightness()
{
  lcd8Digit.SetBrightness(ledwheel.Steps());
  ledBar.SetBrightness(ledwheel.Steps());
}

void menuUp(){
  Serial.println("Menu UP");
  lcd8Digit.UpMenu();
} 

void menuDown(){
  Serial.println("Menu DOWN");
  lcd8Digit.DownMenu();
}

void interruptLCDmode()
{
  Serial.println("LCD mode");
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);

  //push-button configuration
  for (int i = 0; i < RADIO_BUTTONS; i++) //exclude the volume ones
  {
    pinMode(buttons[i], INPUT);
  }

  SPI.begin();
  //output pins
  pinMode(RADIO_OUT, OUTPUT);
  pinMode(SPI_CLOCK,OUTPUT);
  pinMode(SPI_DATA, OUTPUT);
 
  setPotentiometer(RADIO_OUT, NO_OUT);
  SPISettings(10000000, MSBFIRST, SPI_MODE3);
  
  //set encoderpins as Pin Change Interrupts
  volumewheel.Encodersetup(VOL_CLK, VOL_DATA);
  ledwheel.Encodersetup(BRIGHTNESS_CLK, BRIGNTNESS_DATA);
  attachPCINT(digitalPinToPCINT(VOL_CLK), interruptVolume, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PAUSE_BUTTON), interruptPause,FALLING);
  attachPCINT(digitalPinToPCINT(BRIGHTNESS_CLK), interruptBrightness, CHANGE);
  attachInterrupt(digitalPinToInterrupt(LCD_MODE_BUTTON), interruptLCDmode, FALLING);
  
  //setting up the other switches as interrupt
  pinMode(UP_PIN, INPUT_PULLUP);
  pinMode(DOWN_PIN,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(UP_PIN),menuUp,FALLING);
  attachInterrupt(digitalPinToInterrupt(DOWN_PIN),menuDown,FALLING);

  delay(100);
  Serial.println("Pin configuration DONE");

  lcd8Digit.Initialize();
  ledBar.Initialize();

  //gps configuration
  Serial3.begin(GPSBAUD);
  Serial.println("GPS STARTED");

  Serial.println("Configuration DONE");
  delay(DEFAULT_DELAY);

}

/**
 * function that modify the output for the radio depending on the input given by the button presse
 * */

void RadioOutput(short step)
{
  //TODO
}

void GPStest(){
  if (gps.location.isUpdated()){
      // Latitude in degrees (double)
      Serial.print("Latitude= "); 
      Serial.print(gps.location.lat(), 6);      
      // Longitude in degrees (double)
      Serial.print(" Longitude= "); 
      Serial.println(gps.location.lng(), 6); 
       
      // Raw latitude in whole degrees
      Serial.print("Raw latitude = "); 
      Serial.print(gps.location.rawLat().negative ? "-" : "+");
      Serial.println(gps.location.rawLat().deg); 
      // ... and billionths (u16/u32)
      Serial.println(gps.location.rawLat().billionths);
      
      // Raw longitude in whole degrees
      Serial.print("Raw longitude = "); 
      Serial.print(gps.location.rawLng().negative ? "-" : "+");
      Serial.println(gps.location.rawLng().deg); 
      // ... and billionths (u16/u32)
      Serial.println(gps.location.rawLng().billionths);

      // Raw date in DDMMYY format (u32)
      Serial.print("Raw date DDMMYY = ");
      Serial.println(gps.date.value()); 

      // Year (2000+) (u16)
      Serial.print("Year = "); 
      Serial.println(gps.date.year()); 
      // Month (1-12) (u8)
      Serial.print("Month = "); 
      Serial.println(gps.date.month()); 
      // Day (1-31) (u8)
      Serial.print("Day = "); 
      Serial.println(gps.date.day()); 

      // Raw time in HHMMSSCC format (u32)
      Serial.print("Raw time in HHMMSSCC = "); 
      Serial.println(gps.time.value()); 

      // Hour (0-23) (u8)
      Serial.print("Hour = "); 
      Serial.println(gps.time.hour()); 
      // Minute (0-59) (u8)
      Serial.print("Minute = "); 
      Serial.println(gps.time.minute()); 
      // Second (0-59) (u8)
      Serial.print("Second = "); 
      Serial.println(gps.time.second()); 
      // 100ths of a second (0-99) (u8)
      Serial.print("Centisecond = "); 
      Serial.println(gps.time.centisecond()); 

      // Raw speed in 100ths of a knot (i32)
      Serial.print("Raw speed in 100ths/knot = ");
      Serial.println(gps.speed.value()); 
      // Speed in knots (double)
      Serial.print("Speed in knots/h = ");
      Serial.println(gps.speed.knots()); 
      // Speed in miles per hour (double)
      Serial.print("Speed in miles/h = ");
      Serial.println(gps.speed.mph()); 
      // Speed in meters per second (double)
      Serial.print("Speed in m/s = ");
      Serial.println(gps.speed.mps()); 
      // Speed in kilometers per hour (double)
      Serial.print("Speed in km/h = "); 
      Serial.println(gps.speed.kmph()); 

      // Raw course in 100ths of a degree (i32)
      Serial.print("Raw course in degrees = "); 
      Serial.println(gps.course.value()); 
      // Course in degrees (double)
      Serial.print("Course in degrees = "); 
      Serial.println(gps.course.deg()); 

      // Raw altitude in centimeters (i32)
      Serial.print("Raw altitude in centimeters = "); 
      Serial.println(gps.altitude.value()); 
      // Altitude in meters (double)
      Serial.print("Altitude in meters = "); 
      Serial.println(gps.altitude.meters()); 
      // Altitude in miles (double)
      Serial.print("Altitude in miles = "); 
      Serial.println(gps.altitude.miles()); 
      // Altitude in kilometers (double)
      Serial.print("Altitude in kilometers = "); 
      Serial.println(gps.altitude.kilometers()); 
      // Altitude in feet (double)
      Serial.print("Altitude in feet = "); 
      Serial.println(gps.altitude.feet()); 

      // Number of satellites in use (u32)
      Serial.print("Number os satellites in use = "); 
      Serial.println(gps.satellites.value()); 

      // Horizontal Dim. of Precision (100ths-i32)
      Serial.print("HDOP = "); 
      Serial.println(gps.hdop.value()); 
    }
}

void loop()
{
  // put your main code here, to run repeatedly:
  //Serial.println("main");

  lcd8Digit.SetRPMDC(rpm * 10);
  lcd8Digit.SetSpeed((int)gps.speed.kmph());
  lcd8Digit.Update();
  
  ledBar.SetRPMDC(rpm * 10);
  ledBar.Update();  //testfunction for digital potentiometer
  

  while (Serial3.available()){
    gps.encode(Serial3.read());

  }
  GPStest();

  // setPotentiometer(RADIO_OUT,rpm);
  // int read = analogRead(A0);
  // Serial.print("Analog read A0: ");
  // Serial.println(read);
  lcd8Digit.SetTime(gps.time.hour(),gps.time.minute(),gps.time.second());

  
  delay(DEFAULT_DELAY);
}
