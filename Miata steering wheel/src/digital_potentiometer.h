//#include "Defines.h"
//#include "Arduino.h"

#ifdef MPC4131
    #define STEPS 128 //it has 129 steps. one is for the 0 porition
#endif
/**
         * functions that sends the value to the digital potentiometer and sets it
         * the cs is the pin that controls the controller and out is the value to set
         * INPUT: is the number of the button
         * */
void setPotentiometer(int cs, int value)
{
    unsigned char out = map(value,0,RADIO_BUTTONS,STEPS,0);
    Serial.print("DAC output: ");
    Serial.println(out);
    // Puts low the chip select
    digitalWrite(cs, LOW);
    // Send the first byte equal to zero to write the Wiper register
    SPI.transfer(0);
    // Send the second byte with the position of the Wiper (0-128)
    SPI.transfer(out);
    // Puts high the chip select
    digitalWrite(cs, HIGH);
}