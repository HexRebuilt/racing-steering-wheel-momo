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


Encoder_KY040 volumewheel, ledwheel;

InputManager inputManager;

Lcd8Digit lcd8Digit;
LedBar ledBar;

TinyGPSPlus gps;

RpmReader rpm(RPMDCPIN);

Timer upMenuTimer(INPUT_DELAY);
Timer downMenuTimer(INPUT_DELAY);
short tmz = 0;

unsigned int buttonADC = 0, rockerADC = 0, ecuADC = 0;

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
void interruptBrightness()
{
  lcd8Digit.SetBrightness(ledwheel.Steps());
  ledBar.SetBrightness(ledwheel.Steps());
}

void menuUp()
{
  if (upMenuTimer.isNewInput())
  {
    Serial.println("Menu UP");
    lcd8Digit.UpMenu();
  }
}

void menuDown()
{
  if (downMenuTimer.isNewInput())
  {
    Serial.println("Menu DOWN");
    lcd8Digit.DownMenu();
  }
}

void interruptTimeZone()
{
  Serial.println("Setting Timezone mode");
  tmz++;//change timezone
  if (tmz > +14)
  {
    tmz = -12;
  }
  //updating the stored timezone
  EEPROM.put(TIME_ZONE_ADDRESS,tmz);
  lcd8Digit.SetTimeZone(tmz);
}

void interruptRPM(){
  //rpm.incrementRpmCount();
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  //EEPROM.put(TIME_ZONE_ADDRESS,0);
 
  
  //set encoderpins as Pin Change Interrupts
  volumewheel.Encodersetup(VOL_CLK, VOL_DATA);
  ledwheel.Encodersetup(BRIGHTNESS_CLK, BRIGNTNESS_DATA);
  attachPCINT(digitalPinToPCINT(VOL_CLK), interruptVolume, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PAUSE_BUTTON), interruptPause,FALLING);
  attachPCINT(digitalPinToPCINT(BRIGHTNESS_CLK), interruptBrightness, CHANGE);
  attachInterrupt(digitalPinToInterrupt(LCD_MODE_BUTTON),interruptTimeZone, FALLING);
  
  //setting up the rpm pin
  pinMode(RPMDCPIN,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(RPMDCPIN),interruptRPM,FALLING);

  //radio buttons chain
  pinMode(BUTTON_CHAIN_PIN,INPUT);
  pinMode(ROCKER_CHAIN_PIN,INPUT);
  pinMode(ECU_CHAIN_PIN,INPUT);

  inputManager.Startup();

  delay(100);
  Serial.println("Pin configuration DONE");
  EEPROM.get(TIME_ZONE_ADDRESS,tmz);//get the current timezone
  //gps configuration
  Serial3.begin(GPSBAUD);
  Serial.println("GPS STARTED");

  lcd8Digit.Initialize();
  lcd8Digit.SetTimeZone(tmz);
  ledBar.Initialize();
  Serial.println("LCD & LED configuration DONE");

  
  delay(DEFAULT_DELAY);
  Serial.println("Configuration DONE");
  

}

void loop()
{
  // put your main code here, to run repeatedly:
  buttonADC = analogRead(BUTTON_CHAIN_PIN);
  rockerADC = analogRead(ROCKER_CHAIN_PIN);
  ecuADC = analogRead (ECU_CHAIN_PIN);
  inputManager.AnalogButtonDecoder(buttonADC);
  inputManager.AnalogRockerDecoder(rockerADC);
  inputManager.AnalogECUDecoder(ecuADC);
  
  inputManager.SetDAC();
  //delay(1000);
  while (Serial3.available()){
    gps.encode(Serial3.read());
  }
  
  //updating the output devices
  lcd8Digit.SetSatellites(gps.satellites.value());
  lcd8Digit.SetTime(gps.time.hour(),gps.time.minute(),gps.time.second());
  lcd8Digit.SetSpeed((int)gps.speed.kmph());
  lcd8Digit.SetRPMDC(0);
  lcd8Digit.Update();
  
  ledBar.SetRPMDC(50);
  ledBar.Update();  
  
  delay(DEFAULT_DELAY);
}
