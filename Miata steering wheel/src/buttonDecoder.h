enum Buttons : uint8_t {
  none,
  skip,
  back,
  active,
  call_green,
  call_redd,
  voice_cmd,
  vol_up,
  vol_down,
  upmenu,
  downmenu,
  //those ones are linked to digital pin interrupt for the encoders
  pause,
  lcdset
};

/**
 * It's a function that converts the analogue read to a button identifier
 * see the electric scheme for them to be easier to understand
 * INPUT: analog reading value.
 * */
void AnalogButtonDecoder(int analogIn){
  if (analogIn == 0)
  {
    buttonPressed = none;
    return;
  }
  buttonPressed = (Buttons) map(analogIn,0,1023,none,downmenu);
}
