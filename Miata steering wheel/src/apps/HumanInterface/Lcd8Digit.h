#include <LedControl.h>

class Lcd8Digit : public HumanInterface
{
private:
    /* data */
    LedControl lcd = LedControl(LCD_DIN, LCD_CLK, LCD_CS, 1);
    int lcdbrightness = MAX_BRIGHT_LCD; //initial level
    int rpm = 0, speed = 0, satellites = 0, hours = 0, minutes =0, seconds =0; //oiltemp =0, watertmp=0;
    short brightness = 100;
    String brightChange = "", hud = "";

    enum State : uint8_t {
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
    
    State DownState (State& toIncrease){
        return State(toIncrease-1);
    }

    State UpState (State& toIncrease){
        return State(toIncrease+1);
    }

public:
    Lcd8Digit(/* args */);

    /**
     * function used to cycle between the different display state depending on a rocker switch
     * */
    void UpMenu(){
        if (currentstate != end_of_states)
        {
            currentstate = UpState(currentstate);
        }
        else{
            currentstate = State::tachometer;
        }
    }

    void DownMenu(){
        if (currentstate != tachometer)
        {
            currentstate = DownState(currentstate);
        }
        else{
            currentstate = State::clock;
        }
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
        //changing the brightness
        brightness = map(lcdbrightness, MIN_BRIGHT_LCD, MAX_BRIGHT_LCD, 10, 100);
        Serial.print("new brightness: ");
        Serial.println(brightness);
        brightChange = "L16H ";
        brightChange.concat(brightness);
        isBrightnessChanged = true;
    }

    void Update()
    {

        if (isBrightnessChanged)
        {
            isBrightnessChanged = false;
            SetTextLCD(brightChange);
            lcd.setIntensity(0, lcdbrightness);
            delay(500);
        }

        hud = "";
        if (rpm < 1000)
        {
            //Serial.println(1000/rpm);
            for (int i = 0; i < 1000/rpm ; i++)
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
};

Lcd8Digit::Lcd8Digit(/* args */)
{
}
