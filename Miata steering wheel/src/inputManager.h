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
    active
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
    white,
    yellow
  };


#define INPUT_SUM active + skip + pause

  Buttons button = nobutton;
  Rockers rocker = norocker;
  Ecu ecu = noECU;
  Encoder volumeEncoder = nochange;
  short volumeCurrentState = 0;
  uint8_t buttonIndex = 0;
  unsigned long switchtime = 0;

  unsigned int dacValue = 0;

  int oldButtonsAnalogue = 0, oldRockerAnalogue = 0, oldECUAnalogue =0;

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
      buttonIndex = rocker + (uint8_t)Buttons::active;
      rocker = norocker;
      switchtime = millis();
      return;
    }

    if (volumeEncoder != nochange)
    {
      Serial.println("Setting DAC VOLUME");
      //Serial.println(volumeEncoder);
      buttonIndex = volumeEncoder + (uint8_t)Buttons::active + (uint8_t)Rockers::skip;
      volumeEncoder = nochange;
      switchtime = millis();
      return;
    }
    
    if (ecu != noECU)
    {
      Serial.println("Setting ECU OUTPUT");
      //Serial.println(ecu);
      buttonIndex = ecu + (uint8_t)Encoder::pause + (uint8_t)Buttons::active + (uint8_t)Rockers::skip;
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
    if (analogIn < THRESHOLD || oldButtonsAnalogue == analogIn )
    {
      button = nobutton;
      return;
    }

    oldButtonsAnalogue = analogIn;
    Serial.println(analogIn);
    for (uint8_t i = BUTTON_RESISTORS; i > 0; i--)
    {

      if (analogIn > ((1023 * (BUTTON_RESISTORS - i)) / BUTTON_RESISTORS))
      {
        //Serial.print(analogIn);
        //Serial.print(" vs ");
        //Serial.println(((1023 * (BUTTON_RESISTORS - i)) / BUTTON_RESISTORS));

        button = (Buttons)i;
        //Serial.println(button);
      }
    }
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
    if (analogIn < THRESHOLD || oldECUAnalogue == analogIn)
    {
      ecu = noECU;
      return;
    }
    Serial.println(analogIn);
    oldECUAnalogue = analogIn;
    ecu = analogIn > (1023 / ECU_RESISTORS) ? white : yellow;
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
