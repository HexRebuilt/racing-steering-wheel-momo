#include "mpc4131.h"

class InputManager
{
private:
  enum Buttons : uint8_t
  {
    nobutton,
    call_green,
    call_red,
    voice_cmd,
    source
  };

  enum Rockers : uint8_t
  {
    norocker,
    back,
    skip
  };

  enum Encoder : uint8_t
  {
    nochange,
    vol_up,
    vol_down,
    pause
  };

  enum Ecu : uint8_t
  {
    noECU,
    red,
    white,
    yellow,
    upMenu,
    downMenu
  };

#define INPUT_SUM source + skip + pause

  Buttons button = nobutton;
  Rockers rocker = norocker;
  Ecu ecu = noECU;
  Encoder volumeEncoder = nochange;
  short volumeCurrentState = 0;
  uint8_t buttonIndex = 0;
  unsigned long switchtime = 0;

  unsigned int potValue = 0;
  int speed = 0, lastSpeedVolumeChange = 0;
  bool volumeChange = false;

  bool menuForward = false, menuBackward = false; 
  bool ecuSkip = false, buttonSkip = false, rockerSkip = false;
  
  MPC4131 radioOut;
  

  /**
   * it works sequentially analyzing all the private enums and return the 
   * first that's different from 0. it offsets the value taking into account the different
   * values present
   * OUTPUT: it's a function that return an integer value for the pot
   * */
  void POTOutputValue()
  {
    if (button != nobutton)
    {
      Serial.println("Setting pot BUTTONS");
      //Serial.println(button);
      buttonIndex = button;
      button = nobutton;
      switchtime = millis();
      return;
    }

    if (rocker != norocker)
    {
      Serial.println("Setting pot ROCKER");
      //Serial.println(rocker);
      buttonIndex = rocker + (uint8_t)Buttons::source;
      rocker = norocker;
      switchtime = millis();
      return;
    }

    if (volumeEncoder != nochange)
    {
      Serial.println("Setting pot VOLUME");
      //Serial.println(volumeEncoder);
      buttonIndex = volumeEncoder + (uint8_t)Buttons::source + (uint8_t)Rockers::skip;
      volumeEncoder = nochange;
      switchtime = millis();
      return;
    }
  }

  void TimeToResetButtonInput()
  {
    if (switchtime != 0 && (millis() - switchtime) >= ANALOG_OUTPUT_TIME)
    {
      // I need to reset the state of the lcd
      Serial.println("Resetting pot output");
      buttonIndex = 0;
      switchtime = 0;
    }
  }

public:
  InputManager(/* args */);
  ~InputManager();

  void Startup()
  {
    Serial.println("POT initialization");
    //configuring the MPC4131
    radioOut.Startup();
    Serial.println("POT configuration DONE");
    
  }

  /**
   * Function that updates the speed and relative to it changes the volume for the Radio
   * */
  void SetSpeed(int newSpeed)
  {
    speed = newSpeed;

    if (speed > 60)
    {
      if (speed > lastSpeedVolumeChange + KMPH_PER_VOLUME_CHANGE)
      {
        ChangeVolume(VOLUME_STEPS);
        volumeChange = true;
      }

      if (speed < lastSpeedVolumeChange - KMPH_PER_VOLUME_CHANGE)
      {
        ChangeVolume(-VOLUME_STEPS);
        volumeChange = true;
      }
      if (volumeChange)
      {
        volumeChange = false;
        lastSpeedVolumeChange = speed;
        SetPot();
      }
    }
  }

/**
 * It sets the digital potentiometer output value
 * */
  void SetPot()
  {
    POTOutputValue();
    TimeToResetButtonInput();
    potValue = map(buttonIndex, 0, INPUT_SUM, POT_MIN, POT_MAX);
    if (potValue != POT_MIN)
    {
      Serial.print("pot Value: ");
      Serial.println(potValue);
    }
    radioOut.digitalPotWrite(potValue);
    delay(DEFAULT_DELAY);
  }

  /**
 * It's a function that converts the analogue read to a button identifier
 * see the electric scheme for them to be easier to understand
 * INPUT: analog reading value.
 * */
  void AnalogButtonDecoder(int analogIn)
  {
 
    if (analogIn < THRESHOLD)
    {
      button = nobutton;
      buttonSkip = false;
      return;
    }

    if (!buttonSkip)
    {
      Serial.print("Analogue in: ");
      Serial.println(analogIn);
      if (analogIn >= RADIO_GREEN_VALUE - RADIO_RANGE && analogIn <= RADIO_GREEN_VALUE + RADIO_RANGE)
      {
        button = call_green;
      }

      if (analogIn >= RADIO_RED_VALUE - RADIO_RANGE && analogIn <= RADIO_RED_VALUE + RADIO_RANGE)
      {
        button = call_red;
      }

      if (analogIn >= RADIO_BLUE_VALUE - RADIO_RANGE && analogIn <= RADIO_BLUE_VALUE + RADIO_RANGE)
      {
        button = voice_cmd;
      }

      if (analogIn >= RADIO_BLACK_VALUE - RADIO_RANGE && analogIn < RADIO_BLACK_VALUE + RADIO_RANGE)
      {
        button = source;
      }

      if (button != 0)
      {
        Serial.print("BUTTONS Enum value: ");
        Serial.println(button);
        buttonSkip = true;
        return;
      }
    }
  }

  void AnalogRockerDecoder(int analogIn)
  {
    if (analogIn < THRESHOLD)
    {
      rocker = norocker;
      rockerSkip = false;
      return;
    }

    if (!rockerSkip)
    {
      rocker = analogIn > (700) ? back : skip;

      //rocker = analogIn > (1023 / ROCKER_RESISTORS) ? back : skip;

      if (rocker != 0)
      {

        Serial.print("Rocker Enum value: ");
        Serial.println(rocker);
        rockerSkip = true;
        return;
      }
    }
  }

  void AnalogECUDecoder(int analogIn)
  {
    if (analogIn < THRESHOLD)
    {
      ecu = noECU;
      ecuSkip = false; //i've reset the input
      return;
    }

    if (!ecuSkip) //ignore repeted press
    {
      if (analogIn >= ECU_RED_VALUE - ECU_RANGE && analogIn < ECU_RED_VALUE + ECU_RANGE)
      {
        ecu = red;
      }

      if (analogIn >= ECU_WHITE_VALUE - ECU_RANGE && analogIn < ECU_WHITE_VALUE + ECU_RANGE)
      {
        ecu = white;
      }

      if (analogIn >= ECU_YELLOW_VALUE - ECU_RANGE && analogIn < ECU_YELLOW_VALUE + ECU_RANGE)
      {
        ecu = yellow;
      }

      if (analogIn >= ECU_MENU_UP_VALUE - ECU_RANGE && analogIn < ECU_MENU_UP_VALUE + ECU_RANGE)
      {
        ecu = upMenu;
      }

      if (analogIn >= ECU_MENU_DOWN_VALUE - ECU_RANGE && analogIn < ECU_MENU_DOWN_VALUE + ECU_RANGE)
      {
        ecu = downMenu;
      }

      if (ecu != 0)
      {
        Serial.print("ECU Enum value: ");
        Serial.println(ecu);
        ecuSkip = true;
        return;
      }
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
      Serial.println("Vol UP");
      volumeEncoder = vol_up;
    }
    else
    {
      volumeCurrentState--;
      Serial.println("Vol DOWN");
      volumeEncoder = vol_down;
    }
    //Serial.println(potOutputValue());
  }

  void SetPause()
  {
    volumeEncoder = pause;
  }

  void OutputECU(){
    switch (ecu)
    {
    case red:
      /* code */
      break;
    case white:
      /* code */
      break;
    case yellow:
      /* code */
      break;
    case upMenu:
      menuForward = true;
      break;
    case downMenu:
      menuBackward = true;
      break;
    default:
      break;
    }
    //reset the ecu enum variable after elaboration
    ecu = noECU;
  }

  bool isMenuUp(){
    if (menuForward)
    {
      menuForward = false;
      return true;
    }
    return false;
  }
  
  bool IsMenuDown(){
    if (menuBackward)
    {
      menuBackward = false;
      return true;
    }
    return false;
  }

};

InputManager::InputManager(/* args */)
{
}

InputManager::~InputManager()
{
}
