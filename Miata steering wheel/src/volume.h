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
    void ChangeVolume(short steps){
        //direction = RotationVerse(steps);
        if (currentstate == steps) // do nothing
        {
            return;
        }
        
        if (currentstate < steps) // increase volume
        {
            direction = true;
        }
        else
        {
            direction = false;
        }


    }

};
