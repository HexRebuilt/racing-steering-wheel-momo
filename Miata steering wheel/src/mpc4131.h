class MPC4131
{
private:
    /* data */
public:
    MPC4131(/* args */);
    ~MPC4131();

    void Startup()
    {
        pinMode(RADIO_MPC4131_CS, OUTPUT);
        SPI.begin();
        digitalPotWrite(0);
        //Test();
    }

    void digitalPotWrite(unsigned int value)
    {
        //Serial.print("POT VALUE: ");
        //Serial.println(value);

        digitalWrite(RADIO_MPC4131_CS, LOW);
        SPI.transfer(RADIO_MPC4131_ADDRESS);
        SPI.transfer(value);
        digitalWrite(RADIO_MPC4131_CS, HIGH);
    }

    void Test()
    {
        /*
        pin reference between UNO and MEGA
        11 ----------- 51
        12 ----------- 50
        13 ----------- 52
        */

        while (1)
        {
            for (int i = 0; i <= 128; i++)
            {
                digitalPotWrite(i);
                delay(500);
            }
            delay(5000);
            for (int i = 128; i >= 0; i--)
            {
                digitalPotWrite(i);
                delay(500);
            }
        }
    }
};

MPC4131::MPC4131(/* args */)
{
}

MPC4131::~MPC4131()
{
}

