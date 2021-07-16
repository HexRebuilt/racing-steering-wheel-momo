#include <Arduino.h>
#include <LedControl.h>
#include <FastLED.h>



/**
 * this class in in charge of controlling the feedbacks to the driver on the steering wheel
 * like the lcd display and led bars accordingly to some given input
 * 
 * */

class HumanInterface
{
private:
    /* data */
    int lcdbrightness = MAX_BRIGHT_LCD, ledbrightness = MAX_BRIGHT_LEDs; //initial percentage
    short currentvalue = 0, delta = 0, brightness = 100;
    unsigned char offled = NUM_LEDS; 
    LedControl lcd=LedControl(LCD_DIN,LCD_CLK,LCD_CS,1);
    String brightchange="";
    CRGB ledbar[NUM_LEDS];

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

    void LedSetup()
    {
        FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(ledbar, NUM_LEDS);
        FastLED.setBrightness(1);
        for (int i = 0; i < NUM_LEDS; i++)
        {
            ledbar[i] = CRGB::Red;
            delay(LED_REFRESHRATE);
            FastLED.show();
        }
        delay(300);
        for (int i = 0; i < NUM_LEDS; i++)
        {
            ledbar[i] = CRGB::Green;
            delay(LED_REFRESHRATE);
            FastLED.show();
        }
        delay(300);
        for (int i = 0; i < NUM_LEDS; i++)
        {
            ledbar[i] = CRGB::Blue;
            delay(LED_REFRESHRATE);
            FastLED.show();
        }
        FastLED.show();
        delay(300);
        for (int i = 0; i < NUM_LEDS; i++)
        {
            SetLEDs((i + 1) * 10 +5);
            delay(100);
        }

        Serial.println("LED configuration DONE");
    }

public:
    void Initialize()
    {
        //lcd initialization
        lcd.shutdown(0, false);
        lcd.setScanLimit(0, 8); //8 digit
        lcd.setIntensity(0, MAX_BRIGHT_LCD);
        lcd.clearDisplay(0);
        Serial.println("LCD configuration DONE");

        SetTextLCD("-HELL0-");
       //initialize ledbar
        LedSetup();
    }

    /**
     * setting up the brightness depending on the difference from the previous value adding 
     * or subtracting 5% for each step 
     * */
    void SetBrightness(short value)
    {
        if (currentvalue == value)
        {
            return; //no action needed
        }

        if (currentvalue < value)//need to increase brightness
        {
            delta = value - currentvalue;
            //delta = delta / (float) SENSITIVITY;
            lcdbrightness+=delta;
            ledbrightness+=delta*LED_BRIGHT_STEP;
        }
        else
        {
            delta = currentvalue - value;
            //delta = delta / (float) SENSITIVITY;
            lcdbrightness-=delta;
            ledbrightness-=delta*LED_BRIGHT_STEP;
        }
        currentvalue = value;
               
        //keeping the LED brightness in range
        if (ledbrightness > MAX_BRIGHT_LEDs)
        {
            ledbrightness = MAX_BRIGHT_LEDs;
        }
        if (ledbrightness < MIN_BRIGHT_LEDs)
        {
            ledbrightness = MIN_BRIGHT_LEDs;
        }

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
        lcd.setIntensity(0, lcdbrightness);
        brightness = map (lcdbrightness,MIN_BRIGHT_LCD,MAX_BRIGHT_LCD,10,100);
        Serial.print("new brightness: ");
        Serial.println(brightness);
        brightchange = "L16H ";
        brightchange.concat(brightness);
        SetTextLCD(brightchange);
        FastLED.setBrightness(ledbrightness);
        FastLED.show();
        
    }

    /**
     * Function that sets the color and state of the led depending on the RPM.
     * INPUT: Duty-Cycle taken from the engine rpm
     * */
    void SetLEDs(unsigned short rpmDC)
    {
        Serial.println(rpmDC);
        
        //shiftlight needs to happen asap
        if (rpmDC >= SHIFTLIGHT_RPM_DC)
        {
            for (int i = 0; i < NUM_LEDS; i++)
            {
                ledbar[i] = CRGB::Blue;
                delay(LED_REFRESHRATE);
            }
            FastLED.show();
            delay(100);
            for (int i = 0; i < NUM_LEDS; i++)
            {
                ledbar[i] = CRGB::Black;
                delay(LED_REFRESHRATE);
            }
            FastLED.show();
            return;
        }
        

        offled = map(rpmDC, 0, SHIFTLIGHT_RPM_DC, NUM_LEDS, 0);

        for (int i = 0; i <= offled; i++)
        {
            ledbar[i] = CRGB::Black;
            delay(LED_REFRESHRATE);
        }

        for (int i = offled; i < NUM_LEDS; i++) //the leds are mounted upside down
        {
            //those are the leds on
            ledbar[i] = CRGB::Purple;
            //adding the color logic
            if (i > GREEN_LED_INDEX)
            {
                ledbar[i] = CRGB::Green;
                delay(LED_REFRESHRATE);
                continue;
            }
            if (i <= GREEN_LED_INDEX && i > YELLOW_LED_INDEX)
            {
                ledbar[i] = CRGB::Yellow;
                delay(LED_REFRESHRATE);
                continue;
            }
            if (i <= YELLOW_LED_INDEX)
            {
                ledbar[i] = CRGB::Red;
                delay(LED_REFRESHRATE);
                continue;
            }
            

            delay(LED_REFRESHRATE);
        }

        FastLED.show();
    }
};
