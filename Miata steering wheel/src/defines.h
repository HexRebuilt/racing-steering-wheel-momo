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
#define PAUSE_BUTTON       47

#define VOL_CLK     45
#define VOL_DATA    43 

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
#define RADIO_OUT A0

//steering wheel outputs 
#define LCD_CS      49
