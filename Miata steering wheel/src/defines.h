#define DEFAULT_DELAY 10


//list of buttons and pins
#define ACTIVE_BUTTON       PH6 
#define SKIP_BUTTON         PH5 
#define BACK_BUTTON         PH4 
#define CALL_ON_BUTTON      PH3 
#define CALL_OFF_BUTTON     PG5 
#define VOICE_CMD_BUTTON    PE5 
#define SOURCE_BUTTON       PE4 

//encoders pin
#define VOL_CLK                 13
#define VOL_DATA                8
#define PAUSE_BUTTON            3
#define BRIGHTNESS_CLK          11
#define BRIGNTNESS_DATA         9
#define LCD_MODE_BUTTON         2
#define BRIGHTNESS_SENSITIVITY  1
#define VOL_SENSITIVITY         1

//list of RADIO related costants
#define RADIO_BUTTONS   8 //total # of radio buttons - no encoders clk and data
#define RADIO_INPUTS    10// all radio inputs

#define NOTHING     0
#define ACTIVE      1 
#define SKIP        2 
#define BACK        3 
#define CALL_ON     4 
#define CALL_OFF    5 
#define VOICE_CMD   6 
#define SOURCE      7 
#define PAUSE       8
#define VOL_UP      9
#define VOL_DOWN    10 


//list of output pins
#define RADIO_OUT   6 //d6 is where the digital potentiometer will be connected to
#define MPC4131
#define NO_OUT      0

//steering wheel outputs
#define MAX_BRIGHT_LEDS 25 //due to power limitations
#define MAX_BRIGHT_LCD  15
#define MIN_BRIGHT_LCD  1 //to be modified based on needs
#define MIN_BRIGHT_LEDS 1 //to be modified based on needs
#define SENSITIVITY     2
#define LED_BRIGHT_STEP 1 // 255/MAXBRIGHTLCD=17 steps
#define SECONDS_PER_MENU   5//seconds defined before resetting the menu to the main one
#define MILLISECONDS_PER_MENU 5000

//LCD spi related constants
#define LCD_DIN         14 //51
#define LCD_CLK         15 //52
#define LCD_CS          16 //53
#define DIGIT_UPDATE_MS 5

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
#define GPSBaud 9600
