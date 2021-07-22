class InputManager
{
private:
  enum Buttons : uint8_t
  {
    nobutton,
    active,
    call_green,
    call_redd,
    voice_cmd,
  };

  enum Rockers : uint8_t
  {
    norocker,
    skip,
    back,
    upmenu,
    downmenu
  };

  enum Encoder : uint8_t
  {
    nochange,
    vol_up,
    vol_down,
    pause
  };

  Buttons button = nobutton;
  Rockers rocker = norocker;
  Encoder volumeEncoder = nochange;

  short volumeCurrentState = 0;

  uint8_t outputValue = 0;

public:
  InputManager(/* args */);
  ~InputManager();

  /**
   * it works sequentially analyzing all the private enums and return the 
   * first that's different from 0. it offsets the value taking into account the different
   * values present
   * OUTPUT: it's a function that return an integer value for the DAC
   * */

  int outputRadio(){
    if (button != nobutton)
    {
      return button;
    }

    if (rocker != norocker)
    {
      return rocker + (uint8_t) Buttons::voice_cmd;
    }
    
    if (volumeEncoder != nochange)
    {
      return volumeEncoder + (uint8_t) Buttons::voice_cmd + (uint8_t) Encoder::pause;
    }

    return 0; //no input was created
    
  }

  /**
 * It's a function that converts the analogue read to a button identifier
 * see the electric scheme for them to be easier to understand
 * INPUT: analog reading value.
 * */
  void AnalogButtonDecoder(int analogIn)
  {
    button = (Buttons)map(analogIn, 0, 1023, nobutton, voice_cmd);
  }

  void AnalogRockerDecoder(int analogIn)
  {

    rocker = (Rockers)map(analogIn, 0, 1023, norocker, downmenu);
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
