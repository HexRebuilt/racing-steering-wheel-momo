#include <LedControl.h>

class Lcd8Digit : public HumanInterface
{
private:
    /* data */
    LedControl lcd = LedControl(LCD_DIN, LCD_CLK, LCD_CS, 1);
    int lcdbrightness = MAX_BRIGHT_LCD;                                          //initial level
    int rpm = 0, speed = 0, satellites = 0, hours = 0, minutes = 0, seconds = 0; //oiltemp =0, watertmp=0;
    short brightness = 100;
    String brightChange = "", hud = "";
    unsigned long switchtime = 0;

    enum State : uint8_t
    {
        tachometer,
        //oil,
        //water
        //combinedtemperatures
        bright,
        satelliteNumber,
        clock,
        end_of_states
    };

    enum State currentstate = tachometer;

    void SetTextLCD(String text)
    {
        lcd.clearDisplay(0);
        int i, digit = 7;
        //check if the string is less than 8 characters, in case fill it up with spaces
        //i do not care about the termination character because it cannot be displayed
        if (text.length() < 8)
        {
            for (i = text.length(); i < 8; i++)
            {
                text.concat(' ');
            }
        }

        for (i = 0; i < text.length(); i++)
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

    void Satellites(){
        hud =" 5A  ";//need to find a way to create a T
        if (satellites<10)
        {
            hud.concat(" ");
        }
        hud.concat(satellites);
        SetTextLCD(hud);
    }

    void BrightChange()
    {
        //changing the brightness
        brightChange = "L16H ";
        brightChange.concat(brightness);
        if (isBrightnessChanged)
        {
            isBrightnessChanged = false;
            SetTextLCD(brightChange);
            lcd.setIntensity(0, lcdbrightness);
            //delay(500);
        }
    }

public:
    Lcd8Digit(/* args */);

    //function that checks if the difference between the switchtime and the current time is less than
    // the time time to reset
    //. if not it does reset it with the lcd menu state
    void TimeToReset()
    {
        if ((millis() - switchtime) >= MILLISECONDS_PER_MENU)
        { //i need to reset the state of the lcd
            Serial.println("Resetting LCD state");
            currentstate = tachometer;
        }
    }
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
            currentstate = State::clock;
        }
        switchtime = millis();
    }

    void Initialize()
    {
        //lcd initialization
        lcd.shutdown(0, false);
        lcd.setScanLimit(0, 8); //8 digit
        lcd.setIntensity(0, MAX_BRIGHT_LCD);
        lcd.clearDisplay(0);
        Serial.println("LCD configuration DONE");
        SetTextLCD("-HELL0-");
    }

    void SetBrightness(short value)
    {
        brightness = map(lcdbrightness, MIN_BRIGHT_LCD, MAX_BRIGHT_LCD, 10, 100);
        Serial.print("new brightness: ");
        Serial.println(brightness);

        if (currentValue == value)
        {
            return; //no action needed
        }

        if (currentValue < value) //need to increase brightness
        {
            delta = value - currentValue;
            //delta = delta / (float) SENSITIVITY;
            lcdbrightness += delta;
        }
        else
        {
            delta = currentValue - value;
            //delta = delta / (float) SENSITIVITY;
            lcdbrightness -= delta;
        }
        currentValue = value;

        //keeping the LCD brightness in range
        if (lcdbrightness > MAX_BRIGHT_LCD)
        {
            lcdbrightness = MAX_BRIGHT_LCD;
        }
        if (lcdbrightness < MIN_BRIGHT_LCD)
        {
            lcdbrightness = MIN_BRIGHT_LCD;
        }
        isBrightnessChanged = true;
        currentstate = bright; //need a context switch
        switchtime = millis();
        
    }

    void SetTachometer()
    {
        hud = "";
        if (rpm < 1000)
        {
            //Serial.println(1000/rpm);
            for (int i = 0; i < 1000 / rpm; i++)
            {
                hud.concat(" ");
            }
        }
        hud.concat(rpm);
        hud.concat(" ");
        if (speed < 100)
        {
            hud.concat(" ");
        }
        hud.concat(speed);
        //Serial.println(hud);
        SetTextLCD(hud);
    }

    void SetSpeed(int newSpeed)
    {
        speed = newSpeed;
    }

    void SetRPMDC(int newRPM_DC)
    {
        rpm = map(newRPM_DC, 0, SHIFTLIGHT_RPM_DC, 0, MAXRPM);
    }

    /**
     * Function that sets the time
     * INPUT: hours,minutes,seconds
     * */
    void SetTime(int h, int m, int s)
    {
        hours = h;
        minutes = m;
        seconds = s;
    }

    /**
     * function that use the internal enum variable to choose what to display
     * */
    void Update()
    {
        //Serial.println(currentstate);
        switch (currentstate)
        {
        case tachometer:
            SetTachometer();
            break;
        case bright:
            //Serial.println("bright");
            BrightChange();
            TimeToReset();
            break;
        case satelliteNumber:
            //Serial.println("satellites");
            Satellites();
            TimeToReset();
            break;
        case clock:
            //Serial.println("clock");
            DisplayClock();
            TimeToReset();
            break;
        default:
            //Serial.println("tachometer");
            currentstate = tachometer;
            SetTachometer();
            break;
        }
    }
};

Lcd8Digit::Lcd8Digit(/* args */)
{
}
