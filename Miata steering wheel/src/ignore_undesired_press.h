/**
 * File that includes a function to call that allows the system to ignore undesired repeated button readings due to the high processor speed compared to a human hand
 * 
 **/
#include <Arduino.h>

unsigned int InputCleanup(unsigned short current, unsigned short &previous){
    #ifdef DEBUG
        Serial.println(current);
        Serial.println(previous);
    #endif
    
    if (previous == current)
    {
        //Serial.println("Same state");
        return 0;
    }
    else{
        //Serial.println("Updating the state");
        previous = current;
        return 1;
    }
}
