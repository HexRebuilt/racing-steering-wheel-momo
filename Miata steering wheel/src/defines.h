#define DEFAULT_DELAY 1
//#define DEBUG

//list of buttons and pins
#define ACTIVE_BUTTON       PH6 
#define SKIP_BUTTON         PH5 
#define BACK_BUTTON         PH4 
#define CALL_ON_BUTTON      PH3 
#define CALL_OFF_BUTTON     PG5 
#define VOICE_CMD_BUTTON    PE5 
#define SOURCE_BUTTON       PE4 
#define PAUSE_BUTTON        47
#define LCD_MODE_BUTTON     46

//encoders pin
#define VOL_CLK             45
#define VOL_DATA            43 
#define BRIGHTNESS_CLK      42
#define BRIGNTNESS_DATA     44

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
#define RADIO_OUT 6 //d6 is where the digital potentiometer will be connected to
#define MPC4131

//steering wheel outputs
#define MAX_BRIGHT_LEDs 15 //due to power limitations
#define MAX_BRIGHT_LCD  15
#define MIN_BRIGHT_LCD  1 //to be modified based on needs
#define MIN_BRIGHT_LEDs 1 //to be modified based on needs
#define SENSITIVITY     2
#define LED_BRIGHT_STEP 1 // 255/MAXBRIGHTLCD=17 steps

//LCD spi related constants
#define LCD_DIN     51
#define LCD_CLK     52
#define LCD_CS      53

//NeoPixel led bar
#define LED_PIN             7
#define NUM_LEDS            8
#define LED_TYPE            WS2812B
#define COLOR_ORDER         RGB
#define LED_REFRESHRATE     5
#define GREEN_LED_INDEX     4
#define YELLOW_LED_INDEX    6
#define RED_LED_INDEX       8
#define GREEN_RPM_DC        40 //4000RPM should be more or less 60% of DC
#define YELLOW_RPM_DC       60 //5500RPM should be more or less 80% of DC
#define SHIFTLIGHT_RPM_DC   80      
