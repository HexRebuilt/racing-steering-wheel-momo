#include <Arduino.h>
#include <string.h>
#include <stdlib.h>
#include <SPI.h>
#include <LedControl.h>
#include <PinChangeInterrupt.h>
#include <RotaryEncoderAdvanced.h>
#include <TinyGPS++.h>

#include "defines.h"
#include "ignore_undesired_press.h"
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
  attachInterrupt(digitalPinToInterrupt(PAUSE_BUTTON), interruptPause, FALLING);
  attachPCINT(digitalPinToPCINT(BRIGHTNESS_CLK), interruptBrightness, CHANGE);
  attachInterrupt(digitalPinToInterrupt(LCD_MODE_BUTTON), interruptLCDmode, FALLING);

  delay(100);
  Serial.println("Pin configuration DONE");

  lcd8Digit.Initialize();
  ledBar.Initialize();

  //gps configuration
  Serial1.begin(GPSBaud);
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

void loop()
{
  // put your main code here, to run repeatedly:
  //Serial.println("main");

  lcd8Digit.SetRPMDC(rpm * 10);
  lcd8Digit.SetSpeed((int)gps.speed.kmph());
  //lcd8Digit.SetTachometer();
  lcd8Digit.Update();
  ledBar.SetRPMDC(rpm * 10);
  ledBar.Update();  //testfunction for digital potentiometer
  

  // setPotentiometer(RADIO_OUT,rpm);
  // int read = analogRead(A0);
  // Serial.print("Analog read A0: ");
  // Serial.println(read);
  lcd8Digit.SetTime(gps.time.hour(),gps.time.minute(),gps.time.second());

  
  delay(DEFAULT_DELAY);
}
