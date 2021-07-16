#include <Arduino.h>
#include <string.h>
#include <stdlib.h>
#include <SPI.h>
#include <LedControl.h>
#include <PinChangeInterrupt.h>
#include <RotaryEncoderAdvanced.h>

#include "defines.h"
#include "ignore_undesired_press.h"
#include "volume.h"
#include "human_interface.h"
#include "digital_potentiometer.h"
#include "encoder.h"



unsigned short buttons[RADIO_BUTTONS] = {ACTIVE_BUTTON, SKIP_BUTTON, BACK_BUTTON, CALL_ON_BUTTON, CALL_OFF_BUTTON, VOICE_CMD_BUTTON, PAUSE_BUTTON};
unsigned short buttonState[RADIO_BUTTONS] = {0};
unsigned short radioOutputStep=0;
int8_t rpm = 0;

//RotaryEncoderAdvanced<float> volumeEncoder (VOL_CLK,VOL_DATA,PAUSE_BUTTON,VOL_SENSITIVITY,-256,+256);
//RotaryEncoderAdvanced<float> brightnessEncoder(BRIGHTNESS_CLK,BRIGNTNESS_DATA,LCD_MODE_BUTTON,BRIGHTNESS_SENSITIVITY,MIN_BRIGHT_LCD,MAX_BRIGHT_LCD);

Encoder_KY040 volumewheel,ledwheel;

VolumeController volumecontroller;

HumanInterface human_interface;
//LedControl lcd=LedControl(LCD_DIN,LCD_CLK,LCD_CS,1);

void interruptVolume(){
  //volumeEncoder.readAB();
  rpm = volumewheel.Steps();
  //Serial.println("Volume");
  //Serial.println(volumeEncoder.getPosition());
}
void interruptPause(){
  //volumeEncoder.readPushButton();
  Serial.println("Pause");
  
}
void interruptBrightness(){
  human_interface.SetBrightness(ledwheel.Steps());
}
void interruptLCDmode(){
  //brightnessEncoder.readPushButton();
  Serial.println("LCD mode");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  
  //push-button configuration
  for (int i = 0; i < RADIO_BUTTONS; i++) //exclude the volume ones
  {
    pinMode(buttons[i],INPUT);
  }
  
  SPI.begin();
  //output pins
  pinMode(RADIO_OUT,OUTPUT);
  setPotentiometer(RADIO_OUT,NO_OUT);
  
  //set encoderpins as Pin Change Interrupts
  volumewheel.Encodersetup(VOL_CLK,VOL_DATA);
  ledwheel.Encodersetup(BRIGHTNESS_CLK,BRIGNTNESS_DATA);
  //volumeEncoder.begin();
  //brightnessEncoder.begin();
  attachPCINT(digitalPinToPCINT(VOL_CLK),interruptVolume,CHANGE);
  attachInterrupt(digitalPinToInterrupt(PAUSE_BUTTON),interruptPause,FALLING);
  attachPCINT(digitalPinToPCINT(BRIGHTNESS_CLK),interruptBrightness,CHANGE);
  attachInterrupt(digitalPinToInterrupt(LCD_MODE_BUTTON),interruptLCDmode,FALLING);


  delay(100);
  Serial.println("Pin configuration DONE");
  
  human_interface.Initialize();

  
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

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println("main");
  //volumeEncoder.readAB();
  //brightnessEncoder.readAB();
  //human_interface.SetLEDs(volumeEncoder.getPosition()+10);
  //human_interface.SetBrightness(brightnessEncoder.getPosition());
  human_interface.SetLEDs(rpm*10);
  
  delay(250);
}
