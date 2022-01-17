#define DEFAULT_DELAY   10
#define INPUT_DELAY     50 //ms for ignoring repeated input

//encoders pin
#define VOL_CLK                 13
#define VOL_DATA                8
#define PAUSE_BUTTON            3
#define MENU_CLK                11
#define MENU_DATA               9
#define LCD_MODE_BUTTON         2
#define BRIGHTNESS_SENSITIVITY  1
#define VOL_SENSITIVITY         1

//steering wheel outputs
#define MAX_BRIGHT_LEDS 50 //due to power limitations
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

//radio potentiometer
#define RADIO_MPC4131_CS            42
#define RADIO_MPC4131_ADDRESS       0x00
#define POT_MIN                     35
#define POT_MAX                     127
#define ANALOG_OUTPUT_TIME          20  //ms 
#define ANALOG_OUTPUT_CONFIG_TIME   2000 //ms

/*
//menu context switch pins
#define UP_PIN      18
#define DOWN_PIN    19
*/

//NeoPixel led bar
#define LED_PIN             7
#define NUM_LEDS            8
#define LED_TYPE            WS2812B
#define COLOR_ORDER         GRB
#define LED_DELAY           5//ms
#define GREEN_LED_INDEX     4
#define YELLOW_LED_INDEX    1
#define RED_LED_INDEX       0
#define GREEN_RPM_DC        30 //4000RPM should be more or less 60% of DC
#define YELLOW_RPM_DC       50 //5500RPM should be more or less 80% of DC
#define RED_RPM_DC          80
#define SHIFTLIGHT_RPM_DC   80
#define REDLINE_DUTY_CYCLE  75 //to be tested
#define MINRPM              0 
#define MAXRPM              8000
#define RPMDCPIN            19   //D19 

//GPS stuff
#define GPSBAUD 9600
#define GPSTX   14
#define GPSRX   15

#define DAC_MAX 4095

//analog button chain
#define BUTTON_CHAIN_PIN        A0
#define ROCKER_CHAIN_PIN        A1
#define ECU_CHAIN_PIN           A2
#define BUTTON_RESISTORS        4   //are 3+1 in the chain
#define ROCKER_RESISTORS        2   //2 in the chain
#define ECU_RESISTORS           6   //it includes also the rocker
#define THRESHOLD               60  //to avoid incorrect reading from bad ground
//value range for analog reading
//ECU: launch 506/508, yellow 340/342, white 251/253, menu up 176/178, menu down 152/154
//radio: reply 860/863, close call 650/655, voice comand 427/438, source 175/176
#define ECU_RED_VALUE           510 //launch button
#define ECU_YELLOW_VALUE        340
#define ECU_WHITE_VALUE         250
#define ECU_MENU_UP_VALUE       175
#define ECU_MENU_DOWN_VALUE     150
#define ECU_RANGE               10

#define RADIO_GREEN_VALUE   860    
#define RADIO_RED_VALUE     650
#define RADIO_BLUE_VALUE    440
#define RADIO_BLACK_VALUE   170
#define RADIO_RANGE         15



//set Time zone though eeprom
//4kb of memory available
#define TIME_ZONE_ADDRESS   0

//advanced functions
#define KMPH_PER_VOLUME_CHANGE  10
#define VOLUME_STEPS            2