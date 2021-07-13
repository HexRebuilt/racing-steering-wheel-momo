#include <Arduino.h>

/**
 * Class that is used for changing the output of the radio connector based on the changes in the encoder state
 * */

class VolumeController
{
private:
    short currentstate =0;
    boolean direction = true;
    
public:

    /**
     * this function gives back the analogue step for the output that identifies the volume up 
     * or down singal.
     * before giving up the output it checks the difference in steps and moves accoringly.
     * OUTPUT: VOL_UP and VOL_DOWN in the defines.h file
     * INPUT: current steps position of the rotary encoder to be reached by the current state 
     * 
     * */
    int ChangeVolume(short steps){
        //direction = RotationVerse(steps);
        if (currentstate == steps) // do nothing
        {
            return 0; // 0 is a state where no analogue output is created
        }
        
        if (currentstate < steps) // increase volume
        {
            direction = true;
            currentstate++;
            Serial.print("Current volume state:");
            Serial.println(currentstate);
            return VOL_UP;
        }
        else
        {
            direction = false;
            currentstate--;
            Serial.print("Current volume state:");
            Serial.println(currentstate);
            return VOL_DOWN;
        }


    }

};
