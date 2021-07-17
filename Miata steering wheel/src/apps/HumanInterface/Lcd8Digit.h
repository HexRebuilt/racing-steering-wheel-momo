#include <LedControl.h>

class Lcd8Digit : public HumanInterface
{
private:
    /* data */
    LedControl lcd = LedControl(LCD_DIN, LCD_CLK, LCD_CS, 1);
    int lcdbrightness = MAX_BRIGHT_LCD; //initial level
    int rpm = 0, speed = 0;
    short brightness = 100;
    String brightChange = "", hud = "";

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
            delay(DIGIT_UPDATE_MS);
        }
    }

public:
    Lcd8Digit(/* args */);

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
        if (currentvalue == value)
        {
            return; //no action needed
        }

        if (currentvalue < value) //need to increase brightness
        {
            delta = value - currentvalue;
            //delta = delta / (float) SENSITIVITY;
            lcdbrightness += delta;
        }
        else
        {
            delta = currentvalue - value;
            //delta = delta / (float) SENSITIVITY;
            lcdbrightness -= delta;
        }
        currentvalue = value;

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
            delay(300);
        }

        hud = "";
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
        hud.concat(speed);
        Serial.println(hud);
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
