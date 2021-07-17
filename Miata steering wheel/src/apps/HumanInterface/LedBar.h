#include <FastLED.h>

class LedBar : public HumanInterface
{
private:
    /* data */
    int ledbrightness = MAX_BRIGHT_LEDS; //initial level
    CRGB leds[NUM_LEDS];
    uint8_t offled = NUM_LEDS;
    int rpmDC = 0;

public:
    LedBar(/* args */);

    void Initialize()
    {
        FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
        FastLED.setBrightness(MAX_BRIGHT_LEDS);
        for (int i = 0; i < NUM_LEDS; i++)
        {
            SetRPMDC((i + 1) * 10 + 5);
            Update();
            delay(50);
        }

        Serial.println("LED configuration DONE");
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
            ledbrightness += delta * LED_BRIGHT_STEP;
        }
        else
        {
            delta = currentValue - value;
            //delta = delta / (float) SENSITIVITY;
            ledbrightness -= delta * LED_BRIGHT_STEP;
        }
        currentValue = value;

        //keeping the LED brightness in range
        if (ledbrightness > MAX_BRIGHT_LEDS)
        {
            ledbrightness = MAX_BRIGHT_LEDS;
        }
        if (ledbrightness < MIN_BRIGHT_LEDS)
        {
            ledbrightness = MIN_BRIGHT_LEDS;
        }

        //changing the brightness
        FastLED.setBrightness(ledbrightness);
        
    }

    /**
     * Function that sets the color and state of the led depending on the RPM.
     * */
    void Update()
    {
        //Serial.println(rpmDC);
        if (rpmDC == 0 || rpmDC > 100 || rpmDC < 0) //if i am outside my scope i exit
        {
            return;
        }

        //shiftlight needs to happen asap
        if (rpmDC >= SHIFTLIGHT_RPM_DC)
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

        offled = (uint8_t)map(rpmDC, 0, SHIFTLIGHT_RPM_DC, NUM_LEDS, 0);

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

    void SetRPMDC(int newRPM_DC)
    {
        rpmDC = newRPM_DC;
    }
};

LedBar::LedBar(/* args */)
{
}
