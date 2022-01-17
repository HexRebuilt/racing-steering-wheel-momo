/**
 * @file rpmReader.h
 * @author HexRebuilt (you@domain.com)
 * @brief class that read the rpm duty cycle from the tachometer and translates it into a RPM value
 * @version 0.1
 * @date 2022-01-17
 *
 * @copyright Copyright (c) 2022
 *
 */

class RpmReader
{
private:
    int dcpin;
    // Interrupt variables
    volatile unsigned long fall_Time = 0; // Placeholder for microsecond time when last falling edge occured.
    volatile unsigned long rise_Time = 0; // Placeholder for microsecond time when last rising edge occured.
    volatile byte dutyCycle = 0;          // Duty Cycle %
    volatile unsigned long lastRead = 0;  // Last interrupt time (needed to determine interrupt lockup due to 0% and 100% duty cycle)
    unsigned short rpm = 0, oldRpm = 0;

public:
    RpmReader(int pin);
    short GetRPM()
    {
        static unsigned long oldLastRead = lastRead;
        if (oldLastRead != lastRead) //interrupt has happen in the meantime
        {
            Serial.print("TACH DC: ");
            Serial.println(dutyCycle);
            oldLastRead = lastRead;
        }
        else
        { // No interrupt since last read so must be 0% or 100%
            if (digitalRead(dcpin) == LOW)
            {
                Serial.println("0%");
                dutyCycle = 0;
            }
            else
            {
                Serial.println("100%");
                dutyCycle = 100;
            }
        }
        
        rpm = map(dutyCycle, 0, REDLINE_DUTY_CYCLE, MINRPM, MAXRPM);
        if (oldRpm != rpm)
        {
            Serial.print("RPM: ");
            Serial.println(rpm);
        }
        return rpm;
    }

    void DutyCyclePinChange()
    {                        // Pin dcpin (Interrupt 0) service routine
        lastRead = micros(); // Get current time
        if (digitalRead(dcpin) == LOW)
        {
            // Falling edge
            fall_Time = lastRead; // Just store falling edge and calculate on rising edge
        }
        else
        {
            // Rising edge
            unsigned long total_Time = rise_Time - lastRead; // Get total cycle time
            unsigned long on_Time = fall_Time - rise_Time;   // Get on time during this cycle
            total_Time = total_Time / on_Time;               // Divide it down
            dutyCycle = 100 / total_Time;                    // Convert to a percentage
            rise_Time = lastRead;                            // Store rise time
        }
    }
};

RpmReader::RpmReader(int pin)
{
    dcpin = pin;
}

/*



void setup() {
  Serial.begin(115200);
  pinMode(2,INPUT);
  Serial.println(F("ISR Pin 2 Configured For Input."));
  attachInterrupt(0,PinChangeISR0,CHANGE);
  Serial.println(F("Pin 2 ISR Function Attached."));
}

void loop() {
  static unsigned long oldLastRead = lastRead;
  Serial.print("Duty Cycle = ");
  if (oldLastRead != lastRead) {
    Serial.print(dutyCycle);
    oldLastRead = lastRead;
  }
  else { // No interrupt since last read so must be 0% or 100%
    if (digitalRead(2) == LOW){
      Serial.print("0");
    }
    else {
      Serial.print("100");
    }
  }
  Serial.println("%");
  delay(100);
}

*/