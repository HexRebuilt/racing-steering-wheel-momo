#include <Adafruit_MCP4725.h>

class InputManager
{
private:
  enum Buttons : uint8_t
  {
    nobutton,
    //active,
    voice_cmd,
    call_red,
    call_green
  };

  enum Rockers : uint8_t
  {
    norocker,
    skip,
    back
  };

  enum Encoder : uint8_t
  {
    nochange,
    vol_up,
    vol_down,
    pause
  };

  #define INPUT_SUM call_red+back+pause

  Buttons button = nobutton;
  Rockers rocker = norocker;
  Encoder volumeEncoder = nochange;
  short volumeCurrentState = 0;

  unsigned int dacValue = 0;

  Adafruit_MCP4725 dac;

  /**
   * it works sequentially analyzing all the private enums and return the 
   * first that's different from 0. it offsets the value taking into account the different
   * values present
   * OUTPUT: it's a function that return an integer value for the DAC
   * */
  uint16_t OutputValue()
  {
    if (button != nobutton)
    {
      return button;
    }

    if (rocker != norocker)
    {
      return rocker + (uint8_t)Buttons::call_red;
    }

    if (volumeEncoder != nochange)
    {
      return volumeEncoder + (uint8_t)Buttons::call_red + (uint8_t) Rockers::back;
    }

    return 0; //no input was created
  }

public:
  InputManager(/* args */);
  ~InputManager();

  void Startup(){
    dac.begin(0x60);//default address
  }

  void SetDAC(){
    //Serial.println(OutputValue());
    dacValue = map (OutputValue(),0,INPUT_SUM,0, DAC_MAX);
    dac.setVoltage(dacValue,false);
    delay(DEFAULT_DELAY);
    button = nobutton;
    rocker = norocker;
    volumeEncoder = nochange;
    
  }

  /**
 * It's a function that converts the analogue read to a button identifier
 * see the electric scheme for them to be easier to understand
 * INPUT: analog reading value.
 * */
  void AnalogButtonDecoder(int analogIn)
  {
    button = nobutton;
    if (analogIn <= (1023/BUTTON_RESISTORS) && analogIn > 10)
    {
      //Serial.println(voice_cmd);
      button = voice_cmd;
    }
    if (analogIn <= ((1023*2)/BUTTON_RESISTORS) && analogIn > (1023/BUTTON_RESISTORS))
    {
      //Serial.println(call_red);
      button = call_red;
    }
    if (analogIn <= ((1023*3)/BUTTON_RESISTORS) && analogIn > ((1023*2)/BUTTON_RESISTORS))
    {
      //Serial.println(call_green);
      button == call_green;
    }
  }

  void AnalogRockerDecoder(int analogIn)
  {
    rocker = norocker;
    if (analogIn > (1023/ROCKER_RESISTORS))
    {
      rocker = skip;
    }
    if (analogIn <= (1023/BUTTON_RESISTORS) && analogIn > 10)
    {
      rocker = back;
    }
  }

  /**
     * this function gives back the analogue step for the output that identifies the volume up 
     * or down singal.
     * before giving up the output it checks the difference in steps and moves accoringly.
     * OUTPUT: VOL_UP and VOL_DOWN in the defines.h file
     * INPUT: current steps position of the rotary encoder to be reached by the current state 
     * 
     * */
  void ChangeVolume(short steps)
  {
    //direction = RotationVerse(steps);
    if (volumeCurrentState == steps) // do nothing
    {
      volumeEncoder = nochange; // 0 is a state where no analogue output is created
    }

    if (volumeCurrentState < steps) // increase volume
    {
      volumeCurrentState++;
      Serial.print("Current volume state:");
      Serial.println(volumeCurrentState);
      volumeEncoder = vol_up;
    }
    else
    {
      volumeCurrentState--;
      Serial.print("Current volume state:");
      Serial.println(volumeCurrentState);
      volumeEncoder = vol_down;
    }
  }
};

/**
 * function that modify the output for the radio depending on the input given by the button presse
 * INPUT: takes the button enum to map it to an output value for the DAC
 * */
//TODO
//map(id,none,pause,0,DAC_MAX);

InputManager::InputManager(/* args */)
{
}

InputManager::~InputManager()
{
}
