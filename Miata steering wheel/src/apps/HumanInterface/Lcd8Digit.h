#include <LedControl.h>
#include "storageManager.h"

class Lcd8Digit : public HumanInterface
{
private:
    /* data */
    StorageManager storage;
    LedControl lcd = LedControl(LCD_DIN, LCD_CLK, LCD_CS, 1);
    int lcdbrightness = MAX_BRIGHT_LCD;                                          // initial level
    int rpm = 0, speed = 0, satellites = 0, hours = 0, minutes = 0, seconds = 0; // oiltemp =0, watertmp=0;
    short brightness = 100;
    String brightChange = "", hud = "";
    unsigned long switchtime = 0;
    boolean day = true;
    short encoderValue = 0, encoderDelta = 0;
    boolean isRadioConfigOut = false;

    enum State : uint8_t
    {
        tachometer,
        // oil,
        // water
        // combinedtemperatures
        satelliteNumber,
        clock,
        bright,
        radioOut,
        end_of_states
    };

    enum State currentstate = tachometer;

    void SetTextLCD(String text)
    {
        lcd.clearDisplay(0);
        int i, digit = 7;
        // check if the string is less than 8 characters, in case fill it up with spaces
        // i do not care about the termination character because it cannot be displayed
        if (text.length() < 8)
        {
            for (i = text.length(); i < 8; i++)
            {
                text.concat(' ');
            }
        }

        for (i = 0; i < (int)text.length(); i++)
        {
            const boolean isDot = text.charAt(i + 1) == '.';
            lcd.setChar(0, digit, text.charAt(i), isDot);
            if (isDot)
            {
                i++;
            }
            digit--;
        }
    }

    State DownState(State &toIncrease)
    {
        return State(toIncrease - 1);
    }

    State UpState(State &toIncrease)
    {
        return State(toIncrease + 1);
    }

    // hh-mm-secsec WE DO NOT LIKE SS HERE
    void DisplayClock()
    {
        hud = "";
        if (hours < 10)
        {
            hud.concat(" ");
        }
        hud.concat(hours);
        hud.concat("-");
        if (minutes < 10)
        {
            hud.concat(" ");
        }
        hud.concat(minutes);
        hud.concat("-");
        if (seconds < 10)
        {
            hud.concat(" ");
        }
        hud.concat(seconds);
        SetTextLCD(hud);
    }

    void Satellites()
    {
        hud = " 5A  "; // need to find a way to create a T
        if (satellites < 10)
        {
            hud.concat(" ");
        }
        hud.concat(satellites);
        SetTextLCD(hud);
    }

    /**
     * @brief function that notifies the change in radio output time 
     * 
     */
    void RadioConfigTime(){
        isRadioConfigOut = true;
        SetTextLCD("0U1. 5EC");
    }
    // changing the brightness
    void BrightChange()
    {
        brightChange = "L16H ";
        brightChange.concat(brightness);
        isBrightnessChanged = false;
        SetTextLCD(brightChange);
        lcd.setIntensity(0, lcdbrightness);
    }

    void SetTachometer()
    {
        hud = "";
        // engine off
        if (rpm == 0)
        {
            hud = "  ";
        }
        // like 800rpm
        if (rpm < 1000)
        {
            hud.concat(" ");
        }
        hud.concat(rpm);
        hud.concat(" ");
        if (speed < 100)
        {
            hud.concat(" ");
        }
        if (speed < 10)
        {
            hud.concat(" ");
        }
        hud.concat(speed);
        // Serial.println(hud);
        SetTextLCD(hud);
    }

    // function that checks if the difference between the switchtime and the current time is less than
    //  the time time to reset
    //. if not it does reset it with the lcd menu state
    void TimeToReset()
    {
        if ((millis() - switchtime) >= MILLISECONDS_PER_MENU)
        { // i need to reset the state of the lcd
            Serial.println("Resetting LCD state");
            currentstate = tachometer;
        }
    }

public:
    Lcd8Digit(/* args */);

    /**
     * function used to cycle between the different display state depending on a rocker switch
     * */
    void UpMenu()
    {
        if (currentstate != end_of_states)
        {
            currentstate = UpState(currentstate);
        }
        else
        {
            currentstate = State::tachometer;
        }
        switchtime = millis();
    }

    void DownMenu()
    {
        if (currentstate != tachometer)
        {
            currentstate = DownState(currentstate);
        }
        else
        {
            currentstate = State::bright;
        }
        switchtime = millis();
    }

    void Initialize()
    {
        // lcd initialization
        lcd.shutdown(0, false);
        lcd.setScanLimit(0, 8); // 8 digit
        lcd.setIntensity(0, MAX_BRIGHT_LCD);
        lcd.clearDisplay(0);
        Serial.println("LCD configuration DONE");
        SetTextLCD("-HELL0-");
        // memory config
        storage.Startup();
    }

    void SetSpeed(int newSpeed)
    {
        speed = newSpeed;
    }

    void SetRPM(int newRPM)
    {
        rpm = newRPM;
    }

    /**
     * Function that sets the time
     * INPUT: hours,minutes,seconds
     * */
    void SetTime(int h, int m, int s)
    {
        hours = h + storage.GetTimeZone();
        if (hours > 24)
        {
            hours = storage.GetTimeZone();
        }

        minutes = m;
        seconds = s;
    }

    /**
     * function that use the internal enum variable to choose what to display
     * */
    void Update()
    {
        isRadioConfigOut = false;
        // Serial.println(currentstate);
        switch (currentstate)
        {
        case tachometer:
            SetTachometer();
            break;
        case satelliteNumber:
            // Serial.println("satellites");
            Satellites();
            TimeToReset();
            break;
        case clock:
            // Serial.println("clock");
            DisplayClock();
            TimeToReset();
            break;
        case bright:
            // Serial.println("bright");
            BrightChange();
            TimeToReset();
            break;
        case radioOut:
            RadioConfigTime();
            //NOT CALLING FOR TIMETORESET BECAUSE NEEDS TO STAY IN THIS POSITION
            break;
        default:
            // Serial.println("tachometer");
            currentstate = tachometer;
            SetTachometer();
            break;
        }
    }

    /**
     * @brief funciton that goves back to the main the state of the config timing
     */
    boolean IsRadioConfigOut(){
        return isRadioConfigOut;
    }

    /**
     * function that take a delta value and addes to the bright value
     * @param delta +/-1 depending on the rotation
     * */
    void SetBrightness(short delta)
    {
        Serial.print("Delta brightness: ");
        Serial.println(delta);
        lcdbrightness += delta;
        // keeping the LCD brightness in range
        if (lcdbrightness > MAX_BRIGHT_LCD)
        {
            lcdbrightness = MAX_BRIGHT_LCD;
        }
        if (lcdbrightness < MIN_BRIGHT_LCD)
        {
            lcdbrightness = MIN_BRIGHT_LCD;
        }
        Serial.print("lcdbrightness value: ");
        Serial.println(lcdbrightness);
        // isBrightnessChanged = true;
        currentstate = bright; // need a context switch
        switchtime = millis();

        brightness = map(lcdbrightness, MIN_BRIGHT_LCD, MAX_BRIGHT_LCD, 10, 100);
        Serial.print("new brightness: ");
        Serial.println(brightness);
    }

    /**
     * @brief function that based on the current state changes the behaviour of the encoder stpes
     * if i'm on the tachometer it will change the brightness, otherwise will change the value of the presented value
     * when possible
     * @param encoder value given by the encoder wheel
     */
    void ModifyValues(short encoder)
    {
        
        // Calculating the delta
        encoderDelta = encoder - encoderValue;
        encoderValue = encoder; // updating the stored value

        switch (currentstate)
        {
        default:
            Serial.println("Default changing brightness");
            SetBrightness(encoderDelta);
            break;
        case clock:
            Serial.println("Chaniging timezone");
            short newTimezone = storage.GetTimeZone() + encoderDelta;
            storage.SetTimeZone(newTimezone);
            switchtime = millis();
            break;
        }
        encoderDelta=0;
    }

    void SetSatellites(int newSat)
    {
        satellites = newSat;
    }

    short GetBrightness()
    {
        return brightness;
    }
};

Lcd8Digit::Lcd8Digit(/* args */)
{
}
