#include <Arduino.h>
#include <string.h>
#include <stdlib.h>
#include <SPI.h>
#include <LedControl.h>
#include <PinChangeInterrupt.h>
#include <RotaryEncoderAdvanced.h>
#include <TinyGPS++.h>
#include <EEPROM.h>

#include "defines.h"
#include "apps/HumanInterface/human_interface.h"
#include "apps/HumanInterface/Lcd8Digit.h"
#include "apps/HumanInterface/LedBar.h"
#include "encoder.h"
#include "inputManager.h"
#include "rpmReader.h"
#include "timer.h"


unsigned short radioOutputStep = 0;


Encoder_KY040 volumewheel, menuWheel;

InputManager inputManager;

Lcd8Digit lcd8Digit;
LedBar ledBar;

TinyGPSPlus gps;

RpmReader rpm(RPMDCPIN);


unsigned int buttonADC = 0, rockerADC = 0, ecuADC = 0, speed = 0, rpmCurrent = 0;

//series of interrupt associated functions
void interruptVolume()
{
  inputManager.ChangeVolume(volumewheel.Steps());
}
void interruptPause()
{
  inputManager.SetPause();
  Serial.println("Pause");
}
void interruptEncoderMenu()
{
  lcd8Digit.ModifyValues(menuWheel.Steps());
  ledBar.SetBrightness(lcd8Digit.GetBrightness());
}

void interruptRPM(){
  rpm.DutyCyclePinChange();
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
    
  //set encoderpins as Pin Change Interrupts
  volumewheel.Encodersetup(VOL_CLK, VOL_DATA);
  menuWheel.Encodersetup(MENU_CLK, MENU_DATA);
  attachPCINT(digitalPinToPCINT(VOL_CLK), interruptVolume, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PAUSE_BUTTON), interruptPause,FALLING);
  attachPCINT(digitalPinToPCINT(MENU_CLK), interruptEncoderMenu, CHANGE);
  
  //setting up the rpm pin
  pinMode(RPMDCPIN,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(RPMDCPIN),interruptRPM,CHANGE);

  //radio buttons chain
  pinMode(BUTTON_CHAIN_PIN,INPUT);
  pinMode(ROCKER_CHAIN_PIN,INPUT);
  pinMode(ECU_CHAIN_PIN,INPUT);
  Serial.println("Pin configuration DONE");

  inputManager.Startup();

  delay(100);
  Serial.println("Pin configuration DONE");
  //gps configuration
  Serial3.begin(GPSBAUD);
  Serial.println("GPS STARTED");

  lcd8Digit.Initialize();
  ledBar.Initialize();
  Serial.println("LCD & LED configuration DONE");

  delay(DEFAULT_DELAY);
  Serial.println("Configuration DONE");
}

void loop()
{
  // put your main code here, to run repeatedly:
  //getting the main variables
  buttonADC = analogRead(BUTTON_CHAIN_PIN);
  rockerADC = analogRead(ROCKER_CHAIN_PIN);
  ecuADC = analogRead (ECU_CHAIN_PIN);
  speed = gps.speed.kmph();
  rpmCurrent = 0; //rpm.GetRPM();

  lcd8Digit.SetSatellites(gps.satellites.value());
  lcd8Digit.SetTime(gps.time.hour(),gps.time.minute(),gps.time.second());
  lcd8Digit.SetSpeed((int) speed);
  lcd8Digit.SetRPM(rpmCurrent);
  lcd8Digit.Update();
  
  ledBar.SetRPM(rpmCurrent);
  ledBar.Update();

  if (lcd8Digit.IsRadioConfigOut())
  {
    //set config time
    inputManager.SetOutputTime(ANALOG_OUTPUT_CONFIG_TIME);
    Serial.println("RADIO CONFIG MODE");
  }
  else{
    inputManager.SetOutputTime(ANALOG_OUTPUT_TIME);
  }
  

  inputManager.AnalogButtonDecoder(buttonADC);
  inputManager.AnalogRockerDecoder(rockerADC);
  inputManager.AnalogECUDecoder(ecuADC);
  inputManager.SetSpeed(speed);

  inputManager.OutputECU();
  
  inputManager.SetPot();
  
  while (Serial3.available()){
    gps.encode(Serial3.read());
  }
  
  //updating the lcd and leds
  if (inputManager.isMenuUp())
  {
    lcd8Digit.UpMenu();
  }
  if (inputManager.IsMenuDown())
  {
    lcd8Digit.DownMenu();
  }
  

  
  delay(DEFAULT_DELAY);
}
