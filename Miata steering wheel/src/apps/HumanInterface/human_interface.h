#include <Arduino.h>

/**
 * this class in in charge of controlling the feedbacks to the driver on the steering wheel
 * like the lcd display and led bars accordingly to some given input
 * 
 * */

class HumanInterface
{
private:
    /* data */

protected:
    short currentValue = 0, delta = 0;
    boolean isBrightnessChanged = false;
    

public:
    void Initialize();

    void Update();

    void SetRPMDC(int newRPM_DC);

    /**
     * setting up the brightness depending on the difference from the previous value adding 
     * or subtracting 5% for each step 
     * */
    void SetBrightness(short value);
};

HumanInterface::HumanInterface()
{
    Initialize();
}