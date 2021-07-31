/**
 * class to cronometrate the time
 * */

class Timer
{
private:
    long startTime;   // start time for stop watch
    long elapsedTime; // elapsed time for stop watch
    int fractional;   // variable used to store fractional part of time
    int ignoreInput;

public:

    Timer(int inputDelay);
    ~Timer();

    int timePassed()
    {
        elapsedTime = millis() - startTime;
        Serial.print("Execution time: ");
        Serial.print((long)(elapsedTime / 1000L)); // divide by 1000 to convert to seconds - then cast to an int to print
        Serial.print(".");                         // print decimal point

        // use modulo operator to get fractional part of time
        fractional = (long)(elapsedTime % 1000L);

        if (fractional == 0)
            Serial.print("000");  // add three zero's
        else if (fractional < 10) // if fractional < 10 the 0 is ignored giving a wrong time, so add the zeros
            Serial.print("00");   // add two zeros
        else if (fractional < 100)
            Serial.print("0"); // add one zero

        Serial.println(fractional); // print fractional part of time
        Serial.println("");

        return elapsedTime / 1000L;
    }

    void startTimer()
    {
        startTime = millis();
    }

    bool isNewInput(){
        elapsedTime = millis() - startTime;
        if (elapsedTime > ignoreInput)
        {
            //reset start time
            startTimer();
            return true;
        }
        return false;
    }
    
};

Timer::Timer(int inputDelay)
{
    ignoreInput = inputDelay;
    startTime = millis();
}
Timer::~Timer()
{
}