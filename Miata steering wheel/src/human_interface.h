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
        int i, digit=7;
        //check if the string is less than 8 characters, in case fill it up with spaces
        //i do not care about the termination character because it cannot be displayed
        if (text.length()<8)
        {
            for (i = text.length(); i < 8; i++)
            {
                text.concat(' ');
            }
        }

        i = 7;
        
        /*Serial.println(text.length());
        for (i = text.length(); i >= 0 ; i--)
        {
            Serial.println(text[text.length()-i]);
            //Serial.println(i);
            Serial.println(digit);
            Serial.println("----------------");
            
            lcd.setChar(0, digit, text[text.length()-i], text.charAt(text.length()-i-1)=='.' );
            if (text.charAt(text.length()-i-1)=='.')
            {
                Serial.println(text.charAt(text.length()-i-1));
                i--;
            }
            
            digit--;
        }*/
        

        
        while(digit >= 0) //writing text from the last digit to the first
        {
            /*
            Serial.print("char:");
            Serial.print(text[7-i]);
            Serial.print(" @:");
            Serial.print(7-i);
            Serial.print(" digit:");
            Serial.println(digit);
            
            Serial.print("i : ");
            Serial.println(i);
            Serial.print("text.charAt(text.length()-i :");
            Serial.println(text.charAt(text.length()-i));
            */

            //check for the '.' character
            if (text.charAt(text.length()-i)!='.')
            {
                lcd.setChar(0, digit, text[7-i], false);
            }
            else
            {
                //i--;
                //digit--;
                
                Serial.print("DOT found writing ");
                
                Serial.print(text[7-i]);
                Serial.print(". @ digit:");
                Serial.println(digit);
                
                lcd.setChar(0, digit, text[7-i], true); //rewrite the previous char with the dot
                break;
            }

            i--;
            digit--;

            //delay(250);
        }
        
    }


    void LedSetup(){
        FastLED.addLeds<LED_TYPE,LED_PIN,COLOR_ORDER>(ledbar,NUM_LEDS);
        FastLED.setBrightness(1);
        for (int i = 0; i < NUM_LEDS; i++)
        {
            ledbar[i] = CRGB::Red;
            delay(LED_REFRESHRATE);
            FastLED.show();
        }
        delay(100);
        for (int i = 0; i < NUM_LEDS; i++)
        {
            ledbar[i] = CRGB::Green;
            delay(LED_REFRESHRATE);
            FastLED.show();
        }
        delay(100);
        for (int i = 0; i < NUM_LEDS; i++)
        {
            ledbar[i] = CRGB::Blue;
            delay(LED_REFRESHRATE);
            FastLED.show();
        }
        FastLED.show();
        for (int i = 0; i < NUM_LEDS; i++)
        {
            SetLEDs(i+1*10);
            delay(50);
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
        delay(500);
        SetTextLCD("c1A.01.23");
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
        Serial.print("RPM DC: ");
        Serial.println(rpmDC);
        //MAP does not work
        offled = map(rpmDC,0,SHIFTLIGHT_RPM_DC,1,NUM_LEDS);
        Serial.print("Offled: ");
        Serial.println(offled);
        
        for (int i = NUM_LEDS-1 ; i >= 0; i--) //the leds are mounted upside down
        {
            Serial.print("led # ");
            Serial.println(NUM_LEDS-i);
            Serial.print("OFFLED # ");
            Serial.println(offled);
            
            if ((NUM_LEDS - i) >= offled)//the leds needs to be on.
            {
                Serial.println("the led needs to be ON");
                //ledbar[i] = CRGB::Yellow;
                //color selector based on the rpm DC
                
                if (rpmDC < YELLOW_RPM_DC && rpmDC> GREEN_RPM_DC)
                {
                    Serial.println("Yellow");
                    ledbar[NUM_LEDS - i] = CRGB::Yellow;
                }
                else
                {
                    Serial.println("Red");
                    ledbar[NUM_LEDS - i] = CRGB::Red;
                }
                if (rpmDC < GREEN_RPM_DC)
                {
                    Serial.println("Green");
                    ledbar[NUM_LEDS - i] = CRGB::Green;
                }

            }
            else{
                ledbar[NUM_LEDS - i] = CRGB::Black;
            }
            delay(LED_REFRESHRATE);
        }
        FastLED.show();   
    }
    

};
