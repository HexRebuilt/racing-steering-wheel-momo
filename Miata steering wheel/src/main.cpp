#include <Arduino.h>
#include "defines.h"
#include <string.h>

int buttons[RADIO_BUTTONS]={MUTEBUTTON, SKIP_BUTTON, BACK_BUTTON, CALL_ON, CALL_OFF, VOICE_CMD, PAUSE,0,0};

void setup() {
  // put your setup code here, to run once:
  
  //push-button configuration
  for (int i = 0; i < RADIO_BUTTONS-2; i++) //exclude the volume ones
  {
    pinMode(buttons[i],INPUT);
  }


  Serial.begin(9600);
  delay(100);
  if (Serial.available())
  {
    Serial.println("Pin configuration DONE");
  }
  
  
}

void loop() {
  // put your main code here, to run repeatedly:

  //int sensorValue = digitalRead(MUTEBUTTON); //gives a 0 if it is low and 1 if high
  
  
  
  //Serial.println(sensorValue);


  delay(DEFAULT_DELAY);
}