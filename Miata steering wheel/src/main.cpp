#include <Arduino.h>
#include <string.h>
#include <stdlib.h>
#include <SPI.h>
#include <LedControl.h>

#include "defines.h"
#include "ignore_undesired_press.h"
#include "encoder.h"
#include "volume.h"



unsigned short buttons[RADIO_BUTTONS] = {ACTIVE_BUTTON, SKIP_BUTTON, BACK_BUTTON, CALL_ON_BUTTON, CALL_OFF_BUTTON, VOICE_CMD_BUTTON, PAUSE_BUTTON};
unsigned short buttonState[RADIO_BUTTONS] = {0};
unsigned short radioOutputStep=0;

Encoder_KY040 volumewheel,ledwheel;
VolumeController volumecontroller;

LedControl lcd=LedControl(LCD_DIN,LCD_CLK,LCD_CS,1);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  //push-button configuration
  for (int i = 0; i < RADIO_BUTTONS; i++) //exclude the volume ones
  {
    pinMode(buttons[i],INPUT);
  }

  //output pins
  pinMode(RADIO_OUT,OUTPUT);
  
  //setting up rotary encoder
  volumewheel.Encodersetup(VOL_CLK,VOL_DATA);
  ledwheel.Encodersetup(BRIGHTNESS_CLK,BRIGNTNESS_DATA);
  pinMode(LCD_MODE_BUTTON,INPUT);
 
  
  delay(100);
  Serial.println("Pin configuration DONE");
  
  //lcd initialization
  lcd.shutdown(0,false);
  lcd.setScanLimit(0,8); //8 digit
  lcd.setIntensity(0,1);
  lcd.clearDisplay(0);
  delay(10);
  lcd.setChar(0,7,'-',false);
  lcd.setChar(0,6,'-',false);
  lcd.setChar(0,5,'H',false);
  lcd.setChar(0,4,'E',false);
  lcd.setChar(0,3,'L',false);
  lcd.setChar(0,2,'L',false);
  lcd.setChar(0,1,'0',false);
  lcd.setChar(0,0,'-',false);
  lcd.setIntensity(0,8);
  

  delay(DEFAULT_DELAY);
}

/**
 * function that modify the output for the radio depending on the input given by the button presse
 * */

void RadioOutput(short step)
{
  //TODO
}

void loop() {
  // put your main code here, to run repeatedly:

  //int sensorValue = digitalRead(MUTEBUTTON); //gives a 0 if it is low and 1 if high
  

  volumewheel.Steps();
  radioOutputStep = volumecontroller.ChangeVolume(volumewheel.Steps());
  RadioOutput (radioOutputStep);


  if (InputCleanup(digitalRead(PAUSE_BUTTON), buttonState[6]))
  {
    Serial.print("button state:");
    Serial.println(buttonState[6]);

  }
  
  

  //delay(500);
  delay(DEFAULT_DELAY);
}
