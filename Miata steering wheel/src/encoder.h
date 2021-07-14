#include <Arduino.h>

class Encoder_KY040
{
private:
    /* data */
    int pinA; // Connected to CLK on KY-040
    int pinB;  // Connected to DT on KY-040
    int pinSW;
    int encoderPosCount = 0;
    int pinALast;
    int aVal;
    boolean bCW;



public:

    /**
     * needs 3 pins:
     * INPUT: pins: CW, DATA, SWITCH. if no witch is available put it to 0
     * */
    void Encodersetup(int pincw, int pindata)
    {
        pinA = pincw;
        pinB = pindata;
        
        pinMode(pinA, INPUT);
        pinMode(pinB, INPUT);
        /*
        if (pinsw != 0) // oly if switch is present
        {
            pinSW = pinsw;
            pinMode(pinSW,INPUT);
        }
        */
      
        /* Read Pin A
        Whatever state it's in will reflect the last position
        */
        pinALast = digitalRead(pinA);

        Serial.println("Encoder initialized");
    }

    /**
     * gives you back the number of steps travelled
     * */
    int Steps()
    {
        aVal = digitalRead(pinA);
        if (aVal != pinALast)
        { // Means the knob is rotating
            // if the knob is rotating, we need to determine direction
            // We do that by reading pin B.
            if (digitalRead(pinB) != aVal)
            { // Means pin A Changed first - We're Rotating Clockwise
                encoderPosCount++;
                bCW = true;
            }
            else
            { // Otherwise B changed first and we're moving CCW
                bCW = false;
                encoderPosCount--;
            }
            Serial.print("Rotated: ");
            if (bCW)
            {
                Serial.println("clockwise");
            }
            else
            {
                Serial.println("counterclockwise");
            }
            Serial.print("Encoder Position: ");
            Serial.println(encoderPosCount);
        }
        pinALast = aVal;
        return encoderPosCount;
    }
};
