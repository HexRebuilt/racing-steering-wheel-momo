#include <Arduino.h>
#include <string.h>
#include <stdlib.h>
#include <SPI.h>
#include <LedControl.h>
#include <PinChangeInterrupt.h>
#include <RotaryEncoderAdvanced.h>
#include <TinyGPS++.h>

#include "defines.h"
#include "apps/HumanInterface/human_interface.h"
#include "apps/HumanInterface/Lcd8Digit.h"
#include "apps/HumanInterface/LedBar.h"
#include "encoder.h"
#include "inputManager.h"

unsigned short radioOutputStep = 0;
int rpm = 0;


Encoder_KY040 volumewheel, ledwheel;

InputManager inputManager;

Lcd8Digit lcd8Digit;
LedBar ledBar;

TinyGPSPlus gps;



//series of interrupt associated functions
void interruptVolume()
{
  rpm = volumewheel.Steps();
  inputManager.ChangeVolume(volumewheel.Steps());
}
void interruptPause()
{
  //buttonPressed = pause;
  //RadioOutput(buttonPressed);
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

  //radio buttons chain
  pinMode(BUTTON_CHAIN_PIN,INPUT);
  pinMode(ROCKER_CHAIN_PIN,INPUT);
  
  inputManager.Startup();

  delay(100);
  Serial.println("Pin configuration DONE");

  lcd8Digit.Initialize();
  ledBar.Initialize();

  //gps configuration
  Serial3.begin(GPSBAUD);
  Serial.println("GPS STARTED");
  
  delay(DEFAULT_DELAY);
  Serial.println("Configuration DONE");
  

}

void loop()
{
  // put your main code here, to run repeatedly:
  inputManager.AnalogButtonDecoder(analogRead(BUTTON_CHAIN_PIN));
  inputManager.AnalogRockerDecoder(analogRead(ROCKER_CHAIN_PIN));
  inputManager.SetDAC();
  //delay(1000);
  while (Serial3.available()){
    gps.encode(Serial3.read());
  }
  
  //updating the output devices
  lcd8Digit.SetSatellites(gps.satellites.value());
  lcd8Digit.SetTime(gps.time.hour(),gps.time.minute(),gps.time.second());
  lcd8Digit.SetSpeed((int)gps.speed.kmph());
  lcd8Digit.SetRPMDC(rpm * 10);
  lcd8Digit.Update();
  
  ledBar.SetRPMDC(rpm * 10);
  ledBar.Update();  
  
  delay(DEFAULT_DELAY);
}
