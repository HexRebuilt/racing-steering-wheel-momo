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
    unsigned short rpm=0, kmh=0;
    int lcdbrightness = MAX_BRIGHT_LCD, ledbrightness = MAX_BRIGHT_LEDs; //initial percentage
    short currentvalue = 0, delta = 0, brightness = 100; 
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
            //FastLED.show();
        }
        FastLED.show();
        delay(100);
        for (int i = 0; i < NUM_LEDS; i++)
        {
            ledbar[i] = CRGB::Green;
            delay(LED_REFRESHRATE);
            //FastLED.show();
        }
        FastLED.show();
        delay(100);
        for (int i = 0; i < NUM_LEDS; i++)
        {
            ledbar[i] = CRGB::Blue;
            delay(LED_REFRESHRATE);
            //FastLED.show();
        }
        FastLED.show();
        for (int i = 0; i < NUM_LEDS; i++)
        {
            SetLEDs(i*10);
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
        }
        else
        {
            delta = currentvalue - value;
            //delta = delta / (float) SENSITIVITY;
            lcdbrightness-=delta;
        }
        currentvalue = value;
               

        //keeping the LCD brightness in range
        if (lcdbrightness > MAX_BRIGHT_LCD)
        {
            lcdbrightness = MAX_BRIGHT_LCD;
        }
        if (lcdbrightness < MIN_BRIGHT)
        {
            lcdbrightness = MIN_BRIGHT;
        }
        

        
        Serial.print("new lcd-brightness: ");
        Serial.println(lcdbrightness);
        //Serial.print("new led-brightness: ");
        //Serial.println(ledbrightness);

        //add if 0 turnOFF
        lcd.setIntensity(0, lcdbrightness);
        brightness = map (lcdbrightness,MIN_BRIGHT,MAX_BRIGHT_LCD,10,100);
        brightchange = "L16H ";
        brightchange.concat(brightness);
        SetTextLCD(brightchange);
        
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
        //unsigned short offled = map(rpm,0,SHIFTLIGHT_RPM_DC,0,NUM_LEDS);
        //Serial.print("Offled: ");
        //Serial.println(offled);
        
        for (int i = NUM_LEDS-1 ; i >= 0; i--) //the leds are mounted upside down
        {
            Serial.print("led # ");
            Serial.println(i);
            if ((NUM_LEDS - i)*10<=rpmDC)//the leds needs to be on.
            {
                Serial.println("the led needs to be ON");
                //ledbar[i] = CRGB::Yellow;
                //color selector based on the rpm DC
                
                if (rpmDC < YELLOW_RPM_DC && rpmDC> GREEN_RPM_DC)
                {
                    Serial.println("Yellow");
                    ledbar[i] = CRGB::Yellow;
                }
                else
                {
                    Serial.println("Red");
                    ledbar[i] = CRGB::Red;
                }
                if (rpmDC < GREEN_RPM_DC)
                {
                    Serial.println("Green");
                    ledbar[i] = CRGB::Green;
                }

            }
            else{
                ledbar[i] = CRGB::Black;
            }
            delay(LED_REFRESHRATE);
            //FastLED.show();
            //delay(500);
        }
        FastLED.show();   
    }
    

};
