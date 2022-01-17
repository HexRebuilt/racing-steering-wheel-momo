#include <FastLED.h>

class LedBar : public HumanInterface
{
private:
    /* data */
    int ledbrightness = MAX_BRIGHT_LEDS; //initial level
    CRGB leds[NUM_LEDS];
    uint8_t offled = NUM_LEDS;
    int rpm = 0;

public:
    LedBar(/* args */);

    void Initialize()
    {
        FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
        FastLED.setBrightness(MAX_BRIGHT_LEDS);
        for (int i = 0; i < NUM_LEDS; i++)
        {
            SetRPM((i + 1) * 10 + 5);
            Update();
            delay(50);
        }

        Serial.println("LED configuration DONE");
    }

    void SetBrightness(short value)
    {
        ledbrightness = map(value,MIN_BRIGHT_LEDS, MAX_BRIGHT_LEDS, 10, 100);
        //changing the brightness
        FastLED.setBrightness(ledbrightness);
        
    }

    /**
     * Function that sets the color and state of the led depending on the RPM.
     * */
    void Update()
    {
        //Serial.println(rpm);
        if (rpm == 0 || rpm > MAXRPM || rpm < 0) //if i am outside my scope i exit
        {
            for (int i = 0; i < NUM_LEDS; i++)
            {
                leds[i] = CRGB::Black;
                delay(LED_DELAY);
            }
            FastLED.show();
            return;
        }

        //shiftlight needs to happen asap
        if (rpm >= SHIFTLIGHT_RPM)
        {
            //Serial.println("SHIFTLIGHT");
            for (int i = 0; i < NUM_LEDS; i++)
            {
                leds[i] = CRGB::Blue;
                delay(LED_DELAY);
            }
            FastLED.show();
            delay(100);
            for (int i = 0; i < NUM_LEDS; i++)
            {
                leds[i] = CRGB::Black;
                delay(LED_DELAY);
            }
            FastLED.show();
            return;
        }

        offled = (uint8_t)map(rpm, MINRPM, SHIFTLIGHT_RPM, NUM_LEDS, 0);
        //Serial.println(offled);
        for (int i = 0; i < offled; i++)
        {
            leds[i] = CRGB::Black;
            delay(LED_DELAY);
        }
        for (int i = offled; i < NUM_LEDS; i++) //the leds are mounted upside down
        {
            //those are the leds on
            leds[i] = CRGB::Purple;
            //adding the color logic
            if (i > GREEN_LED_INDEX)
            {
                leds[i] = CRGB::Green;
                delay(LED_DELAY);
                continue;
            }
            if (i <= GREEN_LED_INDEX && i > YELLOW_LED_INDEX)
            {
                leds[i] = CRGB::Yellow;
                delay(LED_DELAY);
                continue;
            }
            else
            {
                leds[i] = CRGB::Red;
                delay(LED_DELAY);
                continue;
            }
        }
        FastLED.show();
    }

    void SetRPM(int newRPM)
    {
        rpm = newRPM;
    }
};

LedBar::LedBar(/* args */)
{
}
