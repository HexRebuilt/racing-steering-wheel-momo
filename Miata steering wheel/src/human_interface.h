#include <Arduino.h>
#include <LedControl.h>
#define MAX_BRIGHT_LEDs 255
#define MAX_BRIGHT_LCD  15
#define MIN_BRIGHT      1 //to be modified based on needs


/**
 * this class in in charge of controlling the feedbacks to the driver on the steering wheel
 * like the lcd display and led bars accordingly to some given input
 * 
 * */

class HumanInterface
{
private:
    /* data */
    int rpm=0, kmh=0;
    int brightness = 100; //initial percentage 
    LedControl lcd=LedControl(LCD_DIN,LCD_CLK,LCD_CS,1);

    void SetTextLCD(String text)
    {   
        lcd.clearDisplay(0);
        int i=0, digit=7;
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
        digit=7;
        while(digit >= 0) //writing text from the last digit to the first
        {
            Serial.print("char:");
            Serial.print(text[7-i]);
            Serial.print(" @:");
            Serial.print(7-i);
            Serial.print(" digit:");
            Serial.println(digit);
            //check for the '.' character
            if (text.charAt(text.length()-i)!='.')
            {
                lcd.setChar(0, digit, text[7-i], false);
            }
            else
            {
                i--;
                digit--;
                Serial.print("DOT found writing ");
                Serial.print(text[7-i]);
                Serial.print(". @ digit:");
                Serial.println(digit);
                lcd.setChar(0, digit, text[7-i], true); //rewrite the previous char with the dot

            }

            i--;
            digit--;

            delay(250);
        }
    }

public:
    void Initialize()
    {
        //lcd initialization
        lcd.shutdown(0, false);
        lcd.setScanLimit(0, 8); //8 digit
        lcd.setIntensity(0, MAX_BRIGHT_LCD);
        lcd.clearDisplay(0);

        SetTextLCD("-HELL0-");
        delay(500);
        SetTextLCD("c1A0123.4");
        

    }



};
