#include <Adafruit_MCP4725.h>


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

  unsigned int dacValue = 0;

  int oldButtons = nobutton, oldRocker = norocker, oldECU = noECU;

  Adafruit_MCP4725 dac;

  /**
   * it works sequentially analyzing all the private enums and return the 
   * first that's different from 0. it offsets the value taking into account the different
   * values present
   * OUTPUT: it's a function that return an integer value for the DAC
   * */
  void OutputValue()
  {
    if (button != nobutton)
    {
      Serial.println("Setting DAC BUTTONS");
      //Serial.println(button);
      buttonIndex = button;
      button = nobutton;
      switchtime = millis();
      return;
    }

    if (rocker != norocker)
    {
      Serial.println("Setting DAC ROCKER");
      //Serial.println(rocker);
      buttonIndex = rocker + (uint8_t)Buttons::source;
      rocker = norocker;
      switchtime = millis();
      return;
    }

    if (volumeEncoder != nochange)
    {
      Serial.println("Setting DAC VOLUME");
      //Serial.println(volumeEncoder);
      buttonIndex = volumeEncoder + (uint8_t)Buttons::source + (uint8_t)Rockers::skip;
      volumeEncoder = nochange;
      switchtime = millis();
      return;
    }
    
    if (ecu != noECU)
    {
      Serial.println("Setting ECU OUTPUT");
      //Serial.println(ecu);
      buttonIndex = ecu + (uint8_t)Encoder::pause + (uint8_t)Buttons::source + (uint8_t)Rockers::skip;
      ecu = noECU;
      switchtime = millis();
      return;
    }
  }

  void TimeToResetButtonInput()
  {
    if (switchtime != 0 && (millis() - switchtime) >= ANALOG_OUTPUT_TIME)
    { 
      // I need to reset the state of the lcd
      Serial.println("Resetting DAC output");
      buttonIndex = 0;
      switchtime = 0;
    }
  }

public:
  InputManager(/* args */);
  ~InputManager();

  void Startup()
  {
    dac.begin(0x60); //default address
    //dac.setVoltage(0,true);
  }

  void SetDAC()
  {
    OutputValue();
    TimeToResetButtonInput();
    dacValue = map(buttonIndex, 0, INPUT_SUM, 0, DAC_MAX);
    dac.setVoltage(dacValue, false);
    delay(DEFAULT_DELAY);
  }

  /**
 * It's a function that converts the analogue read to a button identifier
 * see the electric scheme for them to be easier to understand
 * INPUT: analog reading value.
 * */
  void AnalogButtonDecoder(int analogIn)
  {

    //Serial.println(analogIn);
    //ignore incorrect readings
    if (analogIn < THRESHOLD || oldButtonsAnalogue == analogIn )
    {
      button = nobutton;
      return;
    }

    oldButtonsAnalogue = analogIn;
    Serial.print("BUTTON ADC: ");
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

    Serial.print("BUTTONS Enum value: ");
    Serial.println(button);
  }

  void AnalogRockerDecoder(int analogIn)
  {
    if (analogIn < THRESHOLD || oldRockerAnalogue == analogIn )
    {
      rocker = norocker;
      return;
    }
    Serial.println(analogIn);
    oldRockerAnalogue = analogIn;
    rocker = analogIn > (1023 / ROCKER_RESISTORS) ? back :skip;
  }

  void AnalogECUDecoder(int analogIn)
  {
    if (analogIn < THRESHOLD)
    {
      ecu = noECU;
      return;
    }

    Serial.print("BUTTON ADC: ");
    Serial.println(analogIn);
     
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

    oldECU = ecu; 
    Serial.print("ECU Enum value: ");
    Serial.println(ecu);

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
    //Serial.println(OutputValue());
  }

  void SetPause()
  {
    volumeEncoder = pause;
  }
};

InputManager::InputManager(/* args */)
{
}

InputManager::~InputManager()
{
}
