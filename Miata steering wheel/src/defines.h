#define DEFAULT_DELAY 10

//encoders pin
#define VOL_CLK                 13
#define VOL_DATA                8
#define PAUSE_BUTTON            3
#define BRIGHTNESS_CLK          11
#define BRIGNTNESS_DATA         9
#define LCD_MODE_BUTTON         2
#define BRIGHTNESS_SENSITIVITY  1
#define VOL_SENSITIVITY         1

//steering wheel outputs
#define MAX_BRIGHT_LEDS 25 //due to power limitations
#define MAX_BRIGHT_LCD  15
#define MIN_BRIGHT_LCD  1 //to be modified based on needs
#define MIN_BRIGHT_LEDS 1 //to be modified based on needs
#define SENSITIVITY     2
#define LED_BRIGHT_STEP 1 // 255/MAXBRIGHTLCD=17 steps
#define MILLISECONDS_PER_MENU 3000

//LCD spi related constants
#define LCD_DIN                 22
#define LCD_CLK                 23
#define LCD_CS                  24
#define DIGIT_UPDATE_MS         5

//menu context switch pins
#define UP_PIN      18
#define DOWN_PIN    19


//NeoPixel led bar
#define LED_PIN             7
#define NUM_LEDS            8
#define LED_TYPE            WS2812B
#define COLOR_ORDER         GRB
#define LED_DELAY     5//ms
#define GREEN_LED_INDEX     4
#define YELLOW_LED_INDEX    1
#define RED_LED_INDEX       0
#define GREEN_RPM_DC        30 //4000RPM should be more or less 60% of DC
#define YELLOW_RPM_DC       50 //5500RPM should be more or less 80% of DC
#define RED_RPM_DC          80
#define SHIFTLIGHT_RPM_DC   80
#define MINRPM              5 
#define MAXRPM              7200     

//GPS stuff
#define GPSBAUD 9600
#define GPSTX   14
#define GPSRX   15

#define DAC_MAX 4095

//analog button chain
#define BUTTON_CHAIN_PIN    A0
#define ROCKER_CHAIN_PIN    A1